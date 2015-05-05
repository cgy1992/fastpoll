/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stddef.h> /* size_t, NULL ... */
#include <stdlib.h> /* calloc, realloc, free ... */
#include <string.h> /* memset */
#include <stdio.h>

#include "pool.h"

#define MAX(a,b) a > b ? a : b
  ;

/* initial item size */
#define ITEM_SIZE 512
 
/* shrink memory usage (slow) */
#define SHRINK_USAGE 0

/* computes a chunk size */
#define CHUNK_SIZE(x) MAX(ITEM_SIZE, x)

/**
 * creates a bucket
 *
 * @return a pool-item or NULL if allocation failed
 */
static inline struct fsp_pool_item *
create_item(const size_t size)
{
  struct fsp_pool_item *item;
  item = malloc(sizeof (struct fsp_pool_item));
  
  /* check if alloc failed */
  if (!item) return NULL;
  
  /* alloc bucket */
  item->mem = calloc(1, size);
  
  /* check if alloc failed */
  if (!item->mem) {
    /* free bucket itself */
    free(item);
    return NULL;
  }
  
  item->ptr = item->mem;
  item->next = NULL;
  item->avail = size;
  return item;
}

/**
 * initialize a memory pool
 *
 * @param  fsp_pool context
 * @return          true on success, false on failure
 */
bool fsp_pool_init(struct fsp_pool *pool)
{
  assert(pool != NULL);
  /* set defaults */
  pool->head = create_item(ITEM_SIZE);
  if (!pool->head)
    /* allocation failed */
    return false;
  pool->size = ITEM_SIZE;
  return true;
}

/**
 * take memory
 *
 * @param  fsp_pool context
 * @param  size_t   number of bytes
 * @return          pointer to memory
 */
void *fsp_pool_take(struct fsp_pool *pool, 
                    const size_t mem_size)
{
  assert(pool != NULL);
  
  size_t align = mem_size;
  
  /* align to a 8-byte boundary */
  if (align % 8 != 0)
    align += 8 - align % 8;
  
  struct fsp_pool_item *item = NULL;
  /* check current head */
  if (pool->head->avail >= align)
    item = pool->head;
  
#if SHRINK_USAGE
  else {
    /* check other items */
    struct fsp_pool_item *curr;
    for (curr = pool->head->next; 
         curr != NULL; 
         curr = curr->next)
      if (curr->avail >= align) {
        item = curr;
        break;
      }
  }
#endif
  
  if (item == NULL) {
    /* alloc new item */
    size_t item_size = CHUNK_SIZE(align);
    item = create_item(item_size);
    if (!item) return NULL;
    item->next = pool->head;
    pool->head = item;
    pool->size += item_size;
  }
  
  /* update memory-pointer */
  void *mem_ptr = item->ptr;
  item->ptr += align;
  item->avail -= align;
  memset(mem_ptr, 0, align);
  
  return mem_ptr;
}

/**
 * destroys a memory pool
 *
 * @param fsp_pool context
 */
void fsp_pool_destroy(struct fsp_pool *pool)
{
  struct fsp_pool_item *item = NULL, 
                       *next = NULL;
                       
  for (item = pool->head;
       item != NULL;) {
    next = item->next;
    free(item->mem);
    free(item);
    item = next;
  }
}
