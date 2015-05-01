#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"

/**
 * simple memory pool
 */
struct fsp_pool {
  /* allocated memory */
  void *mem;
  /* pointer to the next available offset
     in the memory-pointer */
  void *ptr;
  /* available bytes */
  size_t avail;
  /* overall allocated bytes */
  size_t size;
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
