/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stddef.h> /* size_t, NULL ... */
#include <stdlib.h> /* malloc, realloc, free ... */
#include <string.h> /* memset, strcat ... */
#include <stdio.h> /* snprintf */

#include "qry.h"

#define ITEM_SIZE sizeof (struct fsp_qry_item)

/* sublime bug */
 ;

/**
 * simple routine to check if a string "looks" numeric (signed 32bit)
 *
 * @param  str input
 * @param  len length
 * @return     true if it does
 */
static bool looks_numeric(const char *str, 
                          const size_t len)
{
  bool neg = str[0] == '-';
  /* basic check, more than 10 (or 11) 
     digits is too much for a 32bit number */
  if (len > (10u + neg))
    return false;
  const char *end = str + len - 1,
             *ptr = str + neg;
  for (; ptr < end; ++str)
    if (*ptr < '0' || *ptr > '9')
      return false;
  /* overflow check */
  if (len == (10u + neg)) {
    static const char 
      max_int[] = "2147483647",
      min_int[] = "2147483648";
    /* pointer to the correct value */
    const char *cmp = neg ? min_int : max_int;
    /* check digit for digit */
    for (size_t i = neg, e = 10u + neg; i < e; ++i) 
      if (str[i] < cmp[i])
        /* at least one digit is lower than the max-value */
        return true;
    /* last check */
    return 0 == strncmp(cmp, str + neg, 10);    
  }
  /* looks good */
  return true;
}

/**
 * sets the item name
 *
 * @param  qry   context
 * @param  item 
 * @param  str  
 * @param  len  name length
 * @return      true on success, false on failure
 */
static inline bool set_name(struct fsp_qry *qry,
                            struct fsp_qry_item *item, 
                            const char *str, 
                            const size_t len)
{
  if (!(item->name = fsp_pool_take(&(qry->pool), len + 1)))
    /* very unlikely */
    return false;
  /* strncpy sets the null-byte */
  strncpy(item->name, str, len);
  return true;
}

/**
 * parses a list of items
 *
 * @param  fsp_qry context
 * @return         NULL on failure
 */
static struct fsp_qry_item *parse_items(struct fsp_qry*);

/**
 * parses a single item
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item pointer to a item
 * @return              NULL on failure
 */
static struct fsp_qry_item *parse_item(struct fsp_qry*);

/**
 * parses item offsets and returns a (sub-)item accordingly
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item base item (without offsets)
 * @return              NULL on failure, an allocated sub-item on success
 */
static struct fsp_qry_item *parse_item_offsets(struct fsp_qry*, 
                                               struct fsp_qry_item*);

/**
 * parses an item value
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item the item
 * @return              true on success, false on failure
 */
static bool parse_item_value(struct fsp_qry*, struct fsp_qry_item*);

/**
 * creates a item
 *
 * @param  fsp_qry context
 * @return         NULL on failure
 */
static struct fsp_qry_item *create_item(struct fsp_qry *qry)
{
  struct fsp_qry_item *item;
  /* allocate */
  if (!(item = fsp_pool_take(&(qry->pool), ITEM_SIZE)))
    return NULL;
  /* set defaults */
  item->name = NULL;
  item->type = FSP_QRY_UNK;
  item->index_max = 0;
  item->next = NULL;
  item->prev = NULL;
  return item;
}

/**
 * initializes the query-parser
 *
 * @param  fsp_qry context
 * @return         true on success, false on failure
 */
bool fsp_qry_init(struct fsp_qry *qry)
{
  assert(qry != NULL);
  /* initialize memory pool */
  if (!fsp_pool_init(&(qry->pool)))
    return false;
  /* set defaults */
  qry->raw = NULL;
  qry->ptr = NULL;
  qry->list = NULL;
  return true;
}

/**
 * parse a query-string using the given cgi request
 *
 * @param  fsp_qry     context
 * @param  const char* input
 * @return          true on success, false on failure
 */
bool fsp_qry_parse(struct fsp_qry *qry, const char *inp)
{
  assert(qry != NULL);
  if (*inp == 0)
    /* nothing to do! */
    return true;
  /* assign values */
  qry->raw = inp;
  qry->ptr = qry->raw;
  /* begin parse */
  qry->list = parse_items(qry);
  return qry->list != NULL;
}

/**
 * release all memory consumed by the query-string context
 *
 * @param fsp_qry context
 */
void fsp_qry_destroy(struct fsp_qry *qry)
{
  assert(qry != NULL);
  fsp_pool_destroy(&(qry->pool));
}

/**
 * parses a single item
 *
 * @param  fsp_qry      context
 * @return              true on success, false on error
 */
static struct fsp_qry_item *parse_item(struct fsp_qry *qry)
{
  struct fsp_qry_item *base = NULL, /* base item (gets returned) */
                      *item = NULL; /* computed item */
  /* look for the next name */
  const char *pos = qry->ptr;
  for (; qry->ptr != 0; ++(qry->ptr))
    if (*(qry->ptr) == '[' ||
        *(qry->ptr) == '&' ||
        *(qry->ptr) == '=')
      break;
  /* check name length */
  size_t name_len = qry->ptr - pos;
  if (name_len == 0)
    /* error or end of input*/
    return NULL;
  /* check if the given name is already assigned */
  struct fsp_qry_item *curr;
  for (curr = qry->list; 
       curr != NULL; 
       curr = curr->next)
    if (0 == strncmp(curr->name, pos, name_len)) {
      /* override this item */
      base = curr;
      break;
    }
  /* assign new item if necessary */
  if (base == NULL) {
    if (!(base = create_item(qry)))
      return NULL;
    if (!set_name(qry, base, pos, name_len))
      return NULL;
  }
  /* set item */
  item = base;
  /* parse offsets (if any) */
  if (*(qry->ptr) == '[' &&
      !(item = parse_item_offsets(qry, base)))
    /* error while parsing */
    return NULL;
  /* parse value (if any) */
  if (*(qry->ptr) == '=' && 
      !parse_item_value(qry, item))
    return NULL;
  /* otherwise, we're done here */
  return base;
}

/**
 * parses item offsets and returns a (sub-)item accordingly
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item base item (without offsets)
 * @return              NULL on failure, an allocated sub-item on success
 */
static struct fsp_qry_item *parse_item_offsets(struct fsp_qry *qry, 
                                               struct fsp_qry_item *item)
{
  /* tail-call like loop */  
  while (*(qry->ptr) == '[') {
    /* pointer to the new (or reused) item */
    struct fsp_qry_item *new_item = NULL;
    /* read offset */
    const char *pos = ++(qry->ptr); /* skip '[' */
    for (; *(qry->ptr) != 0 && *(qry->ptr) != ']'; ++(qry->ptr))
      /* empty loop body*/ ;
    size_t name_len = qry->ptr - pos;
    /* convert the current item to a map */
    if (item->type != FSP_QRY_MAP)
      /* ignore previous value, no need to free() anything */
      item->type = FSP_QRY_MAP;
    /* check if that offset exists in the current item-map */
    if (name_len > 0) {
      struct fsp_qry_item *curr;
      for (curr = item->value.map_val; 
           curr != NULL;
           curr = curr->next)
        if (0 == strncmp(curr->name, pos, name_len)) {
          /* found an item for this offset */
          new_item = curr;
          break;
        }
      /* no item found -> create a new one */
      if (new_item == NULL) {
        if (!(new_item = create_item(qry)))
          /* could not create a new item */
          return NULL;
        /* add it to the end of the list */
        new_item->prev = item->value.map_val->next;
        item->value.map_val->next = new_item;
        /* assign name */
        if (!(new_item->name = fsp_pool_take(&(qry->pool), 
                                             name_len + 1)))
          return NULL;
        strncpy(new_item->name, pos, name_len);
        /* if the name is numeric, adjust index_max too */
        if (looks_numeric(new_item->name, name_len)) {
          /* note: atoi SHOULD be safe now */
          int32_t index_val = atoi(new_item->name);
          if (index_val > item->index_max)
            item->index_max = index_val;
        }
      }
    } else {
      /* no offset-name, acts as push() */
      if (!(new_item = create_item(qry)))
        return NULL;
      /* avoid overflow */
      if (item->index_max < 0x7fffffff)
        ++item->index_max;
      if (!(new_item->name = fsp_pool_take(&(qry->pool), 11)))
        return NULL;
      /* stringify index and store it as name */
      snprintf(new_item->name, 10, "%d", item->index_max);
    }
    /* start all over again */
    item = new_item;
    ++qry->ptr; /* skip ']' */     
  }
  /* "item" should now point to the correct sub-item */
  return item;
}

/**
 * parses a list of items
 *
 * @param  fsp_qry context
 * @return         NULL on failure
 */
static struct fsp_qry_item *parse_items(struct fsp_qry *qry)
{
  /* parse first item */
  struct fsp_qry_item *item = parse_item(qry),
                      *curr = item,
                      *prev = NULL;
  if (!item) return NULL;
  /* parse following items */
  while (*(qry->ptr) == '&') {
    ++(qry->ptr); /* skip '&' */
    curr->prev = prev;
    curr->next = parse_item(qry);
    if (!(curr->next)) break;
    prev = curr;
    curr = curr->next;
  }
  /* top of the list */
  return item;
}

/**
 * parses an item value
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item the item
 * @return              true on success, false on failure
 */
static bool parse_item_value(struct fsp_qry *qry, 
                             struct fsp_qry_item *item)
{
  /* read offset */
  ++(qry->ptr); /* skip '=' */
  const char *pos = qry->ptr; 
  for (; *(qry->ptr) != 0 && *(qry->ptr) != '&'; ++(qry->ptr))
    /* empty loop body */ ;
  size_t value_len = qry->ptr - pos;
  char *value_ptr;
  if (!(value_ptr = fsp_pool_take(&(qry->pool), value_len + 1)))
    return false;
  strncpy(value_ptr, pos, value_len);
  item->value.str_val = value_ptr;
  return true;
}
