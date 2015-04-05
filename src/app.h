#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"
#include "fcgx.h"
 
#include "db.h"
//#include "mem.h"

/**
 * application context
 * 
 * holds global structures to shared interfaces like
 *   - database
 *   - memcache
 *   - ...
 */
struct fsp_app {
  /* global database handle */
  struct fsp_db db;
  /* global memcache handle */
  //struct fsp_mem mem;
};

/**
 * global initializer
 * 
 * @param    app-context
 * @return   true on success, false on failure
 */
bool fsp_app_init(struct fsp_app*);

/**
 * handles a request
 * 
 * @param    app-context
 * @param    request-context
 */
void fsp_app_process(struct fsp_app*, struct fcgx_req*);

/**
 * home route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_home(struct fsp_app*, struct fcgx_req*);

/**
 * vote route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_vote(struct fsp_app*, struct fcgx_req*);

/**
 * poll route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_poll(struct fsp_app*, struct fcgx_req*);

/**
 * error route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_e404(struct fsp_app*, struct fcgx_req*);

/**
 * global destructor
 * 
 * @param    app-context
 */
void fsp_app_destroy(struct fsp_app*);
