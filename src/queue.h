#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"

#include <pthread.h>

struct queue 
{
  void **data, **front, **back;
  size_t size;

  pthread_mutex_t mtx;
};

/**
 * initializes the queue 
 * 
 * @param    queue
 */
void fsp_queue_init(struct queue*) FSP_UNUSED;

/**
 * free's and the queue.
 * 
 * be aware that the queue does NOT free the elements in 
 * itself since it doesn't know wether the element is allocated or not.
 * 
 * @param    queue
 */
void fsp_queue_free(struct queue*) FSP_UNUSED;

/**
 * appends the element to the back of the queue. 
 *
 * queue may perform some optimizations.
 * 
 * @param    queue
 * @param    element
 */
void fsp_queue_push(struct queue*, void*) FSP_UNUSED;

/**
 * returns the front element if there is one, otherwise NULL'll be returned.
 * 
 * @param    queue
 * @return   element
 */
void *fsp_queue_pop(struct queue*) FSP_UNUSED;

/**
 * returns confirmation if queue is empty
 * 
 * @param    queue
 * @return   true if empty or false if not
 */
bool fsp_queue_empty(struct queue*) FSP_UNUSED;

/**
 * returns the element count
 * 
 * @param    queue
 * @return   element count
 */
size_t fsp_queue_size(struct queue*) FSP_UNUSED;
