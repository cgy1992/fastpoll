#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"

// #include "db.h"
// #include "mem.h"

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
};

/* used in fsp_app_handle(...) */
struct fsp_req;

/**
 * global initializer
 * 
 * @param    app-context
 * @return   true on success, false on failure
 */
FSP_API bool fsp_app_init(struct fsp_app*);

/**
 * handles a request
 * 
 * @param    app-context
 * @param    request-context
 */
FSP_API void fsp_app_handle(struct fsp_app*, struct fsp_req*);

/**
 * global destructor
 * 
 * @param    app-context
 */
FSP_API void fsp_app_destroy(struct fsp_app*);
