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

void fsp_req_init(struct fsp_app *app)
{
  if(NULL == app)
  {
    return;
  }

  app->requests.list = calloc(1, sizeof(struct fsp_req) * REQUEST_LIMIT);
  app->requests.count = 0;

  pthread_mutex_init(&app->requests.mtx, 0);
}

void fsp_req_cleanup(struct fsp_app *app)
{
  if(NULL == app)
  {
    return;
  }

  pthread_mutex_destroy(&app->requests.mtx);

  if(app->requests.list != NULL)
  {
    free(app->requests.list);

    app->requests.list = NULL;
  }

  app->requests.count = 0;
}

void fsp_reqs_insert(struct fsp_app *app, struct fcgx_req *req)
{
  pthread_mutex_lock(&app->requests.mtx);

  while(app->requests.count == REQUEST_LIMIT) {
    //sleep(1);
  }
  
  for(unsigned i=0; i<REQUEST_LIMIT; i++) {
    if(app->requests.list[i].state == FSP_RS_CLOSED) {
      app->requests.count++;

      app->requests.list[i].req = req;
      app->requests.list[i].state = FSP_RS_OPEN;

      break;
    }
  }

  pthread_mutex_unlock(&app->requests.mtx);
}

struct fsp_req* fsp_reqs_get(struct fsp_app *app) // make inline
{ 
  struct fsp_req *req = NULL;

  if(app->requests.count > 0) {
    pthread_mutex_lock(&app->requests.mtx);

    for(unsigned i=0; i<REQUEST_LIMIT; i++) {
      if(app->requests.list[i].state == FSP_RS_OPEN)
      {
        req = &app->requests.list[i];
        req->state = FSP_RS_PROC;

        break;
      }
    }
    
    pthread_mutex_unlock(&app->requests.mtx);
  }

  return req;
}

void* fsp_thrd_req_manage(struct fsp_app *app)
{
  struct fcgx_req *req;
  //pthread_mutex_t mtx;
  int rc = 0; /* used for fcgx_req_accept() */
  
  if(app == NULL)
  {
    return NULL;
  }

  //pthread_mutex_init(&mtx, 0);

  for(;;) {
    req = calloc(1, sizeof(struct fcgx_req));

    if (fcgx_req_init(req, app->sid, 0) != 0) {
      //error(-1, FSP_ERR_FCGI_REQ, "error: could not initialize fcgi request");
      return NULL;
    }

    //pthread_mutex_lock(&mtx);
    rc = fcgx_req_accept(req);
    //pthread_mutex_unlock(&mtx);
      
    if (rc < 0) {
      fcgx_req_finish(req);
      free(req);
      //error(-1, FSP_ERR_FCGI_ACCEPT, "error: unable to accept requests, thread will exit");
      return NULL;
    }

    printf("received request: %p\n", req->out);
    fsp_reqs_insert(app, req);
  }

  //pthread_mutex_destroy(&mtx);
  
  return app;
}

void* fsp_thrd_req_serve(struct fsp_app *app)
{
  struct fsp_req *r;

  if(app == NULL)
  {
    return NULL;
  }

  for (;;) {
    if((r = fsp_reqs_get(app)) == NULL) {
      //sleep(1);
      continue;
    }

    printf("handling request (state: %d, out: %p)\n", r->state, r->req->out);
    
    fcgx_fputs("Status: 200\r\n", r->req->out);
    fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", r->req->out);
    fcgx_fputs("\r\n", r->req->out);
    fcgx_fputs("hello world!\r\n", r->req->out);
    //fcgx_fprintf(r->req.out, "thread: %ld", tid);
    
    printf("handled request\n");

    r->state = FSP_RS_SERVED;
  }
  
  
  return app;
}

void* fsp_thrd_req_cleanup(struct fsp_app *app)
{
  if(app == NULL)
  {
    return NULL;
  }

  while(NULL != app->requests.list){
    for(unsigned i=0; i<REQUEST_LIMIT && app->requests.count > 0; i++)
    {
      if(app->requests.list[i].state == FSP_RS_SERVED)
      {
        printf("cleaning up request #%d\n", i+1);
        fcgx_req_finish(app->requests.list[i].req);
        free(app->requests.list[i].req);

        memset(&app->requests.list[i], 0, sizeof(struct fsp_req)); // just to be sure everything is reset..
        app->requests.count--;
      }
    }

    //sleep(1);
  }

  return app;
}
