/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

#include "fsp.h"

/**
 * home route
 * @param app
 */
void fsp_rt_home(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>Home</h1>");
}

/**
 * vote route
 * @param app
 */
void fsp_rt_vote(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>Vote</h1>");
}

/**
 * poll route
 * @param app
 */
void fsp_rt_poll(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>Poll</h1>");
}

/**
 * 404 not found route
 * @param app
 */
void fsp_rt_e404(struct fsp *app)
{
  fsp_resp(app, 404, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>404 - Not Found</h1>");
}
