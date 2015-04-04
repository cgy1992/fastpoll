#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stdint.h>
#include <stdbool.h>

#define UNUSED __attribute__((unused))
 ; /* sublime bug */

#include "fsp_db.h"

/**
 * fastpoll context
 */
struct fsp {
  /* persistent data etc... */
  struct fsp_db db;
};

/**
 * initializer
 * @param app  application context
 */
bool fsp_init(struct fsp*);

/**
 * destructor
 * @param app  application context
 */
void fsp_clear(struct fsp*);

/**
 * request handler (server)
 * @param app  application context
 */
void fsp_serv(struct fsp*);

/**
 * start response
 * @param app 
 * @param sc   status code
 * @param ct   content-type
 */
void fsp_resp(struct fsp*, uint32_t, const char*);

/**
 * output data 
 * @param app 
 * @param tx    data
 */
void fsp_puts(struct fsp*, const char*);

/**
 * output data 
 * @param app 
 * @param format
 * @param args 
 */
void fsp_printf(struct fsp*, const char*, ...);
