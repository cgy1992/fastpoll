#pragma once

/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

struct fsp_db {
  /* todo */  
};

bool fsp_db_open(struct fsp_db*, const char*,
                                 const char*,
                                 const char*);

void fsp_db_close(struct fsp_db*);
