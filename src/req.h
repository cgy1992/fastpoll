#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */
#include "fsp.h"
#include "fcgx.h"
#include "queue.h"

#include <pthread.h>

/* used for requests */
struct fsp_app;

/**
 * requests specified initializer for queues, etc. 
 * 
 * @param    app-context
 */
void fsp_req_init(struct fsp_app*);

/**
 * requests are so very, very dirty.. cleanup (queues, ..)
 * 
 * @param    app-context
 */
void fsp_req_cleanup(struct fsp_app*);

/**
 * thread routine for request acception (accept, verify, ..) 
 * 
 * @param    app-context
 * @return   -
 */
void* fsp_thrd_req_accept(struct fsp_app*) FSP_UNUSED;

/**
 * thread routine for request handling (header, content, cleanup, ..) 
 * 
 * @param    app-context
 * @return   -
 */
void* fsp_thrd_req_handle(struct fsp_app*) FSP_UNUSED;
