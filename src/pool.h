#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"

struct fsp_pool_item;

/**
 * simple memory pool
 */
struct fsp_pool {
  /* current pool-item */
  struct fsp_pool_item *head;
  /* overall allocated bytes */
  size_t size;
};

/**
 * memry item (bucket)
 */
struct fsp_pool_item {
  /* allocated memory */
  void *mem;
  /* pointer to the next free slot */
  void *ptr;
  /* available bytes */
  size_t avail;
  /* next item */
  struct fsp_pool_item *next;
};

/**
 * initialize a memory pool
 *
 * @param  fsp_pool context
 * @return          true on success, false on failure
 */
bool fsp_pool_init(struct fsp_pool*);

/**
 * take memory
 *
 * @param  fsp_pool context
 * @param  size_t   number of bytes
 * @return          pointer to memory
 */
void *fsp_pool_take(struct fsp_pool*, const size_t);

/**
 * destroys a memory pool
 *
 * @param fsp_pool context
 */
void fsp_pool_destroy(struct fsp_pool*);
