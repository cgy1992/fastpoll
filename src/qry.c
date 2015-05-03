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
  const bool neg = str[0] == '-';
  const size_t max_len = 10u + neg;
  /* basic check, more than 10 (or 11) 
     digits is too much for a 32bit number */
  if (len > max_len)
    return false;
  const char *end = str + len - 1,
             *ptr = str + neg;
  for (; ptr < end; ++ptr)
    if (*ptr < '0' || *ptr > '9')
      return false;
  if (len == max_len) {
    static const char 
      max_int[] = "2147483647",
      min_int[] = "2147483648";
    const char *cmp = neg ? min_int : max_int;
    for (size_t i = neg; i < max_len; ++i) 
      if (str[i] < cmp[i])
        return true;
    return 0 == strncmp(cmp, str + neg, 10);    
  }
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
    return false;
  
  strncpy(item->name, str, len);
  item->name_len = len;
  return true;
}

/**
 * compare an item-name
 *
 * @param  item     the item
 * @param  name     
 * @param  name_len 
 * @return          true on a match, false otherwise
 */
static inline bool cmp_name(const struct fsp_qry_item *item,
                            const char *name,
                            const size_t name_len)
{
  return item->name_len == name_len &&
    0 == strncmp(item->name, name, name_len);
}

/**
 * parses a single item
 *
 * @param  fsp_qry      context
 * @return              true on success, false on failure
 */
static bool parse_item(struct fsp_qry*);

/**
 * parses item offsets and returns a (sub-)item accordingly
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item base item (without offsets)
 * @return              NULL on failure, an allocated sub-item on success
 */
static struct fsp_qry_item *parse_item_offsets(
  struct fsp_qry*, struct fsp_qry_item*);

/**
 * parses an item value
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item the item
 * @return              true on success, false on failure
 */
static bool parse_item_value(
  struct fsp_qry*, struct fsp_qry_item*);

/**
 * creates a item
 *
 * @param  fsp_qry context
 * @return         NULL on failure
 */
static struct fsp_qry_item *create_item(struct fsp_qry *qry)
{
  struct fsp_qry_item *item;
  if (!(item = fsp_pool_take(&(qry->pool), ITEM_SIZE)))
    return NULL;
  
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
  if (!fsp_pool_init(&(qry->pool)))
    return false;
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
    return true;
  qry->raw = inp;
  qry->ptr = qry->raw;
  while (parse_item(qry)) {
    if (*(qry->ptr) != '&')
      break;
    if (!*(++(qry->ptr)))
      break;
  }
  return true;
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
static bool parse_item(struct fsp_qry *qry)
{
  struct fsp_qry_item *base = NULL, /* base item */
                      *item = NULL; /* computed item */
  
  const char *pos = qry->ptr;
  
  for (; *(qry->ptr) != 0; ++(qry->ptr))
    if (*(qry->ptr) == '[' ||
        *(qry->ptr) == '&' ||
        *(qry->ptr) == '=')
      break;
      
  size_t name_len = qry->ptr - pos;
  if (name_len == 0)
    return false;
  
  struct fsp_qry_item *curr;
  for (curr = qry->list; 
       curr != NULL; 
       curr = curr->next) 
    if (cmp_name(curr, pos, name_len)) {
      base = curr;
      break;
    }
    
  if (base == NULL) {
    if (!(base = create_item(qry)))
      return false;
    
    if (!set_name(qry, base, pos, name_len))
      return false;
    
    if (!qry->list)
      qry->list = base;
    else {
      base->next = qry->list;
      base->next->prev = base;
      qry->list = base;
    }
  }
  
  item = base;
  
  if (*(qry->ptr) == '[' &&
      !(item = parse_item_offsets(qry, base)))
    return false;
    
  if (*(qry->ptr) == '=' && 
      !parse_item_value(qry, item))
    return false;
  
  return true;
}

/* note: never free() memory inside a pool! */
#define ERASE_STR(s) memset(s, 0, strlen(s))
  ;

#define ADD_TO_LIST(item, list) { \
  if (list == NULL)               \
    list = item;                  \
  else {                          \
    item->next = list;            \
    list->prev = item;            \
    list = item;                  \
  }                               \
}

/**
 * parses item offsets and returns a (sub-)item accordingly
 *
 * @param  fsp_qry      context
 * @param  fsp_qry_item base item (without offsets)
 * @return              NULL on failure, an allocated sub-item on success
 */
static struct fsp_qry_item *
parse_item_offsets(struct fsp_qry *qry, 
                   struct fsp_qry_item *item)
{
  /* tail-call like loop */  
  while (*(qry->ptr) == '[') {
    struct fsp_qry_item *new_item = NULL;
    const char *pos = ++(qry->ptr); /* skip '[' */
    
    for (; *(qry->ptr) != 0 && *(qry->ptr) != ']'; ++(qry->ptr))
      /* empty loop body*/ ;
      
    size_t name_len = qry->ptr - pos;
    if (item->type != FSP_QRY_MAP) {
      /* note: previous value gets lost */
      item->type = FSP_QRY_MAP;
      if (item->value.str_val != NULL) {
        ERASE_STR(item->value.str_val);
        item->value.str_val = NULL;
      }
    }
    
    if (name_len > 0) {
      /**
       * search for an existing item in the bucket 
       * linked-list or create a new one
       */
      struct fsp_qry_item *curr;
      for (curr = item->value.map_val; 
           curr != NULL;
           curr = curr->next)
        if (cmp_name(curr, pos, name_len)) {
          new_item = curr;
          break;
        }
        
      if (new_item == NULL) {
        if (!(new_item = create_item(qry)))
          return NULL;
        
        if (!(set_name(qry, new_item, pos, name_len)))
          return NULL;
        
        /* add new-item to bucket linked-list */
        ADD_TO_LIST(new_item, item->value.map_val);
        
        if (looks_numeric(new_item->name, name_len)) {
          /* used for the next push'ed item */
          int32_t index_val = atoi(new_item->name);
          if (index_val > item->index_max)
            item->index_max = index_val;
        }
      }
    } else {
      /**
       * empty offset -> "push" a new item onto the
       * bucket linked-list
       */
      if (!(new_item = create_item(qry)))
        return NULL;
      
      if (!(new_item->name = fsp_pool_take(&(qry->pool), 12)))
        return NULL;
      
      if (item->index_max < 0x7fffffff)
        ++item->index_max;
      
      snprintf(new_item->name, 11, "%d", item->index_max);
      
      /* add new-item to bucket linked-list */
      ADD_TO_LIST(new_item, item->value.map_val);
    }
    
    item = new_item;
    ++(qry->ptr); /* skip ']' */
  }
  
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
  ++(qry->ptr); /* skip '=' */
  const char *pos = qry->ptr; 
  for (; *(qry->ptr) != 0 && *(qry->ptr) != '&'; ++(qry->ptr))
    /* empty loop body */ ;
    
  size_t value_len = qry->ptr - pos;
  char *value_ptr = NULL;
  
  if (!(value_ptr = fsp_pool_take(&(qry->pool), value_len + 1)))
    return false;
  
  strncpy(value_ptr, pos, value_len);
  item->type = FSP_QRY_STR;
  item->value.str_val = value_ptr;
  return true;
}
