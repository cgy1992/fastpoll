#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stdbool.h>

#include "fsp.h"

/* database context */
struct fsp_db {
  bool is_open;
};

/**
 * open databse
 * @param  db     context
 * @return        status
 */
bool fsp_db_open(struct fsp_db*);

/**
 * close and cleanup database
 * @param db   context
 */
void fsp_db_close(struct fsp_db*);
