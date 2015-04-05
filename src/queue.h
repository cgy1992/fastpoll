#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"

struct queue 
{
  void **data, **front, **back;
  size_t size;
};

void fsp_queue_init(struct queue*) FSP_UNUSED;
void fsp_queue_free(struct queue*) FSP_UNUSED;

void fsp_queue_push(struct queue*, void*) FSP_UNUSED;
void *fsp_queue_pop(struct queue*) FSP_UNUSED;

bool fsp_queue_empty(struct queue*) FSP_UNUSED;
size_t fsp_queue_size(struct queue*) FSP_UNUSED;
