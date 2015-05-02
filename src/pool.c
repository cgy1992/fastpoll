/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stddef.h> /* size_t, NULL ... */
#include <stdlib.h> /* calloc, realloc, free ... */
#include <string.h> /* memset */

#include "pool.h"

/* initial item size */
#define ITEM_SIZE 512
 
/* shrink memory usage (slow) */
#define SHRINK_USAGE 0

/**
 * creates a bucket
 *
 * @return a pool-item or NULL if allocation failed
 */
static inline struct fsp_pool_item *create_item()
{
  struct fsp_pool_item *item;
  item = malloc(sizeof (struct fsp_pool_item));
  
  /* check if alloc failed */
  if (!item) return NULL;
  
  /* alloc bucket */
  item->mem = calloc(1, ITEM_SIZE);
  
  /* check if alloc failed */
  if (!item->mem) {
    /* free bucket itself */
    free(item);
    return NULL;
  }
  
  item->ptr = item->mem;
  item->next = NULL;
  item->avail = ITEM_SIZE;
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
  pool->head = create_item();
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
  struct fsp_pool_item *item = NULL;
  /* check current head */
  if (pool->head->avail >= mem_size)
    item = pool->head;
  
#if SHRINK_USAGE
  else {
    /* check other items */
    struct fsp_pool_item *curr;
    for (curr = pool->head->next; 
         curr != NULL; 
         curr = curr->next)
      if (curr->avail >= mem_size) {
        item = curr;
        break;
      }
  }
#endif
  
  if (item == NULL) {
    /* alloc new item */
    item = create_item();
    if (!item) return NULL;
    item->next = pool->head;
    pool->head = item;
    pool->size += ITEM_SIZE;
  }
  
  /* update memory-pointer */
  void *mem_ptr = item->ptr;
  item->ptr += mem_size;
  item->avail -= mem_size;
  memset(mem_ptr, 0, mem_size);
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
