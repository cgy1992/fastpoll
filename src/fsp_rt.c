/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

#include "fsp.h"
#include "template_def.h"

/**
 * home route
 * @param app
 */
void fsp_rt_home(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, FSP_TPL_DOC_BEG);
  fsp_puts(app,   FSP_TPL_HEADER);
  fsp_puts(app,   FSP_TPL_MAIN_BEG);
  
  fsp_puts(app,     FSP_TPL_PAGE_HOME);

  fsp_puts(app,   FSP_TPL_MAIN_END);
  fsp_puts(app,   FSP_TPL_FOOTER);
  fsp_puts(app, FSP_TPL_DOC_END);
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
  fsp_puts(app, FSP_TPL_DOC_BEG);
  fsp_puts(app,   FSP_TPL_HEADER);
  fsp_puts(app,   FSP_TPL_MAIN_BEG);
  
  fsp_printf(app,   FSP_TPL_PAGE_RESULT_BEG, "Testpoll");

  struct fsp_tpl_page_result_entry_ctx entry = {
    .entry_name = "Foo",
    .entry_per = 50,
    .entry_id = 0,
    .entry_value = 5
  };

  fsp_render_tpl_page_result_entry(&entry);
  fsp_render_tpl_page_result_entry(&entry);

  fsp_puts(app,     FSP_TPL_PAGE_RESULT_END);

  fsp_puts(app,   FSP_TPL_MAIN_END);
  fsp_puts(app,   FSP_TPL_FOOTER);
  fsp_puts(app, FSP_TPL_DOC_END);
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
