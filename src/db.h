#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <mysql.h>
#include <threads.h>

#include "fsp.h"

/* database context */
struct fsp_db {
  /* mysql handle */
  MYSQL dbh;
  /* mysql_query mutex */
  mtx_t query_mtx;
};

/**
 * initializer
 *   
 * @param    db context
 * @return   true on success, false on failure
 */
bool fsp_db_init(struct fsp_db*);

/**
 * destructor
 * 
 * @param   db context
 */
void fsp_db_destroy(struct fsp_db*);
