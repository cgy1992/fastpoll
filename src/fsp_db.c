/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

#include "fsp.h"
#include "fsp_db.h"

bool fsp_db_open(struct fsp_db *db UNUSED, 
                 const char *name UNUSED,
                 const char *user UNUSED,
                 const char *pass UNUSED)
{
  return true;
}

void fsp_db_close(struct fsp_db *db UNUSED)
{
  
}
