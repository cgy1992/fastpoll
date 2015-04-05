/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <string.h>

#include "app.h"
#include "fcgx.h"

/* predefined application routes */
static const 
  struct {
    /* path (without query-string) */
    char *name;
    /* path length */
    size_t nlen;
    /* controller function */
    void(*func)(struct fsp_app*, struct fcgx_req*);
  }
           
  routes[] = {
    { "/",     1, fsp_app_rt_home },
    { "/vote", 5, fsp_app_rt_vote },
    { "/poll", 5, fsp_app_rt_poll },
    /* end */
    { 0, 0, 0 }
  };
 
/**
 * global initializer
 * 
 * @param    app-context
 * @return   true on success, false on failure
 */
bool fsp_app_init(struct fsp_app *app)
{  
  /* init database */
  if (!fsp_db_init(&app->db))
    return false;
  
  /* init memcache */
  //if (!fsp_mem_init(&app->mem))
  //  return false;
  
  /* looks good */
  return true;
}

/**
 * handles a request
 * 
 * @param    app-context
 * @param    request-context
 */
void fsp_app_process(struct fsp_app *app, 
                     struct fcgx_req *req)
{
  /* get request uri */
  char *req_uri = fcgx_req_param(req, "REQUEST_URI");
  size_t req_len = 0;
  
  /* find query-string */
  char *qs_pos = strchr(req_uri, '?');
  
  if (qs_pos != 0) 
    /* ignore query-string in strncmp() */
    req_len = qs_pos - req_uri;
  else
    /* full search */
    req_len = strlen(req_uri);
  
  /* route */  
  for (size_t i = 0; routes[i].nlen != 0; ++i) {
    /* compare length first and abort early if necessary,
       otherwise "/v" would match "/vote" and so on */
    if (routes[i].nlen != req_len)
      continue;
    
    /* compare names and call the controller on a match */
    if (strncmp(req_uri, routes[i].name, req_len) == 0) {
      (routes[i].func)(app, req);
      return; /* done, get out */
    }
  }
  
  /* no route found -> error 404 */
  fsp_app_rt_e404(app, req);
}

/**
 * home route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_home(struct fsp_app *app FSP_UNUSED, 
                     struct fcgx_req *req)
{
  fcgx_fputs("Status: 200\r\n", req->out);
  fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs("home", req->out);  
}

/**
 * vote route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_vote(struct fsp_app *app FSP_UNUSED, 
                     struct fcgx_req *req)
{
  fcgx_fputs("Status: 200\r\n", req->out);
  fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs("vote", req->out);  
}

/**
 * poll route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_poll(struct fsp_app *app FSP_UNUSED, 
                     struct fcgx_req *req)
{
  fcgx_fputs("Status: 200\r\n", req->out);
  fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs("poll", req->out);  
}

/**
 * error route 
 * 
 * @param app  application context
 * @param req  request context
 */
void fsp_app_rt_e404(struct fsp_app *app FSP_UNUSED, 
                     struct fcgx_req *req)
{
  fcgx_fputs("Status: 404\r\n", req->out);
  fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs("404", req->out);  
}

/**
 * global destructor
 * 
 * @param    app-context
 */
void fsp_app_destroy(struct fsp_app *app FSP_UNUSED)
{
  fsp_db_destroy(&app->db);
  //fsp_mem_destroy(&app->mem);
}
