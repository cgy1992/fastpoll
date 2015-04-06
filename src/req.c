#include "app.h"
#include "req.h"

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

//#include <error.h>

#define REQUEST_LIMIT 100

/**
 * requests specified initializer for queues, etc. 
 * 
 * @param    app-context
 */
void fsp_req_init(struct fsp_app *app)
{
  if(NULL == app)
  {
    return;
  }

  fsp_queue_init(&app->requests);
}

/**
 * requests are so very, very dirty.. cleanup (queues, ..)
 * 
 * @param    app-context
 */
void fsp_req_cleanup(struct fsp_app *app)
{
  if(NULL == app)
  {
    return;
  }

  fsp_queue_free(&app->requests);
}

/**
 * thread routine for request acception (accept, verify, ..) 
 * 
 * @param    app-context
 * @return   -
 */
void* fsp_thrd_req_accept(struct fsp_app *app)
{
  struct fcgx_req *req;
  int rc; /* used for fcgx_req_accept() */
  
  if(app == NULL)
  {
    return NULL;
  }

  for(;;) {
    req = malloc(sizeof(struct fcgx_req));

    if(req == NULL) {
#if FSP_DEBUG == 1
      printf("(-- ERROR: allocation failed %s in %s at %d --)\n", __FILE__, __FUNCTION__, __LINE__);
#endif

      return NULL;
    }

    if (fcgx_req_init(req, app->sid, 0) != 0) {
      //error(-1, FSP_ERR_FCGI_REQ, "error: could not initialize fcgi request");
      free(req);
      return NULL;
    }

    rc = fcgx_req_accept(req);
      
    if (rc < 0) {
      fcgx_req_finish(req);
      free(req);
      //error(-1, FSP_ERR_FCGI_ACCEPT, "error: unable to accept requests, thread will exit");
      return NULL;
    }

#if FSP_DEBUG == 1
    printf("received request: %p\n", req->out);
#endif

    fsp_queue_push(&app->requests, req);
  }
  
  return app;
}

/**
 * thread routine for request handling (header, content, cleanup, ..) 
 * 
 * @param    app-context
 * @return   -
 */
void* fsp_thrd_req_handle(struct fsp_app *app)
{
  struct fcgx_req *r;

  if(app == NULL)
  {
    return NULL;
  }

  for (;;) {
    while(fsp_queue_empty(&app->requests) == true) {
      usleep(200);
    }
    
    r = fsp_queue_pop(&app->requests);

    if(r == NULL)
      continue;

#if FSP_DEBUG == 1
    printf("handling request (%p)\n", r);
#endif

    fcgx_fputs("Status: 200\r\n", r->out);
    fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", r->out);
    fcgx_fputs("\r\n", r->out);
    fcgx_fputs("hello world!\r\n", r->out);
    //fcgx_fprintf(r->req.out, "thread: %ld", tid);

#if FSP_DEBUG == 1      
    printf("handled request\n");
#endif

    fcgx_req_finish(r);
    free(r);
  }
  
  
  return app;
}
