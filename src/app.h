#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"
#include "req.h"

// #include "db.h"
// #include "mem.h"

#include <pthread.h> 

/**
 * application context
 * 
 * holds global structures to shared interfaces like
 *   - database
 *   - memcache
 *   - ...
 */
struct fsp_app {
#if 0
  /* global database handle */
  struct fsp_db db;
  /* global memcache handle */
  struct fsp_mem mem;
#endif
  struct fsp_req_list requests;

  pthread_t tid[FSP_THREAD_COUNT];
  int sid;
};

/**
 * global initializer
 * 
 * @param    app-context
 * @return   true on success, false on failure
 */
FSP_API bool fsp_app_init(struct fsp_app*);

/**
 * starts the main app routines
 * 
 * @param    app-context
 */
FSP_API void fsp_app_start(struct fsp_app*);

/**
 * stops the main app routines
 * 
 * @param    app-context
 */
FSP_API void fsp_app_stop(struct fsp_app*);

/**
 * global destructor
 * 
 * @param    app-context
 */
FSP_API void fsp_app_destroy(struct fsp_app*);
