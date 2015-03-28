#pragma once

/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

#include <stdint.h>
#include <stdbool.h>

#define UNUSED __attribute__((unused))
 ; /* sublime bug */

#define DB_NAME "fastpoll"
#define DB_USER "fastpoll"
#define DB_PASS "JG)(Updfddkl3=§("

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
