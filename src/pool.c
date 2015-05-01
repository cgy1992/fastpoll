/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stddef.h> /* size_t, NULL ... */
#include <stdlib.h> /* calloc, realloc, free ... */

#include "pool.h"

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
  pool->mem = NULL;
  pool->ptr = NULL;
  pool->size = 0;
  pool->avail = 0;
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
  /* note: "<=" because one byte is reserved */
  if (pool->avail <= mem_size) {
    /* new size is: size + (additional_required_bytes) + 1 */
    size_t add_size = mem_size - pool->avail + 1,
           new_size = pool->size + add_size;
    /* realloc memory-pool */
    void *tmp = realloc(pool->mem, new_size);
    /* handle error */
    if (!tmp) return NULL;
    /* update pointers */
    pool->mem = tmp;
    pool->ptr = tmp + pool->size + 1;    
    pool->size = new_size;
    pool->avail = add_size;
    /* this is our additional reserved byte :-) */
    *(uint8_t *)(pool->ptr - 1) = 0;
  }
  /* use offset pointer */
  void *mem_ptr = pool->ptr;
  pool->ptr += mem_size;
  return mem_ptr;
}

/**
 * destroys a memory pool
 *
 * @param fsp_pool context
 */
void fsp_pool_destroy(struct fsp_pool *pool)
{
  /* yup, that's it */
  free(pool->mem);
}
