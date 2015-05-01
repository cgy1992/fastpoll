/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <string.h>

#include "app.h"
#include "fcgx.h"
#include "tpl.h"
#include "qry.h"
 
/* controller function type */
typedef void(*fsp_rt_t)(struct fsp_app*, struct fcgx_req*);

/* controller functions */
static void rt_home(struct fsp_app*, struct fcgx_req*);
static void rt_vote(struct fsp_app*, struct fcgx_req*);
static void rt_poll(struct fsp_app*, struct fcgx_req*);

/* routes */
static const struct {
  /* path (without query-string) */
  char *name;
  /* path length */
  size_t nlen;
  /* controller function */
  fsp_rt_t func;
}
             
routes[] = {
  { "/",     1, rt_home },
  { "/vote", 5, rt_vote },
  { "/poll", 5, rt_poll },
  /* end */
  { 0, 0, 0 }
};

/* ------------------------------------ */

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
  fcgx_fputs("Status: 404\r\n", req->out);
  fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs("404 - Not found", req->out);
}

/**
 * global destructor
 * 
 * @param    app-context
 */
void fsp_app_destroy(struct fsp_app *app)
{
  fsp_db_destroy(&app->db);
  //fsp_mem_destroy(&app->mem);
}

/* ------------------------------------ */

static void dump_qry_list(struct fsp_qry_item *list, 
                          struct fcgx_req *req, 
                          int tab)
{
  struct fsp_qry_item *item;
  
  for (item = list; 
       item != NULL; 
       item = item->next) {
    fcgx_fputs("<br>\n", req->out);
    for (int i = 0; i < tab; ++i)
      fcgx_fputs("&nbsp;&nbsp;", req->out);
    fcgx_fputs("name: ", req->out);
    fcgx_fputs(item->name, req->out);
    fcgx_fputs(", value: ", req->out);
    if (item->type == FSP_QRY_STR)
      fcgx_fputs(item->value.str_val, req->out);
    else {
      fcgx_fputs("{", req->out);
      dump_qry_list(item->value.map_val, req, tab + 1);
      fcgx_fputs("<br>", req->out);
      for (int i = 0; i < tab; ++i)
        fcgx_fputs("&nbsp;&nbsp;", req->out);
      fcgx_fputs("}", req->out);
    }
  }
} 

/* home route controller */
static void rt_home(struct fsp_app *app FSP_UNUSED, 
                    struct fcgx_req *req)
{
  /* not much to do here */
  fcgx_fputs("Status: 200\r\n", req->out);
  fcgx_fputs("Content-Type: text/html; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs("<!DOCTYPE html>", req->out);
  
  const char *query_str = fcgx_req_param(req, "QUERY_STRING");
  struct fsp_qry qry;
  fsp_qry_init(&qry);
  
  if (fsp_qry_parse(&qry, query_str))
    dump_qry_list(qry.list, req, 0);
  else
    fcgx_fputs("could not parse query string", req->out);
  
  //fcgx_fputs(FSP_TPL_HOME, req->out);
}

/* home route controller */
static void rt_vote(struct fsp_app *app FSP_UNUSED, 
                    struct fcgx_req *req)
{
  /* not much to do here */
  fcgx_fputs("Status: 200\r\n", req->out);
  fcgx_fputs("Content-Type: text/html; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  fcgx_fputs(FSP_TPL_HOME, req->out);
}

/* home route controller */
static void rt_poll(struct fsp_app *app FSP_UNUSED, 
                    struct fcgx_req *req)
{
  char *req_mth = fcgx_req_param(req, "REQUEST_METHOD");
  
  fcgx_fputs("Status: 200\r\n", req->out);
  fcgx_fputs("Content-Type: text/html; Charset=UTF-8\r\n", req->out);
  fcgx_fputs("\r\n", req->out);
  
  if (strncmp(req_mth, "POST", 4) == 0) {
    /* new poll */
    fcgx_fputs("new poll", req->out);
  } else {
    /* view poll */
    fcgx_fputs("view poll", req->out);
  }
}
