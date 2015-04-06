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

  fsp_queue_init(&app->requests.queue);
  pthread_mutex_init(&app->requests.mtx, 0);
}

void fsp_req_cleanup(struct fsp_app *app)
{
  if(NULL == app)
  {
    return;
  }

  pthread_mutex_destroy(&app->requests.mtx);
  fsp_queue_free(&app->requests.queue);
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

    if(req == NULL) {
#ifdef FSP_DEBUG
      printf("(-- ERROR: allocation failed %s in %s at %d --)\n", __FILE__, __FUNCTION__, __LINE__);
#endif

      continue;
    }

    if (fcgx_req_init(req, app->sid, 0) != 0) {
      //error(-1, FSP_ERR_FCGI_REQ, "error: could not initialize fcgi request");
      free(req);
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

    //pthread_mutex_lock(&app->requests.mtx);
    fsp_queue_push(&app->requests.queue, req);
    //pthread_mutex_unlock(&app->requests.mtx);
  }

  //pthread_mutex_destroy(&mtx);
  
  return app;
}

void* fsp_thrd_req_serve(struct fsp_app *app)
{
  struct fcgx_req *r;

  if(app == NULL)
  {
    return NULL;
  }

  for (;;) {

    //pthread_mutex_lock(&app->requests.mtx);
    /*if(fsp_queue_empty(&app->requests.queue) == true) {
      usleep(150);
      continue;
    }*/

    
    r = fsp_queue_pop(&app->requests.queue);
    
    if(r == NULL) {
      usleep(50);
      continue;
    }
    //pthread_mutex_unlock(&app->requests.mtx);

    printf("handling request (%p)\n", r);

    if(r->out != NULL) {
      printf(" (out: %p)\n", r->out);
      
      fcgx_fputs("Status: 200\r\n", r->out);
      fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", r->out);
      fcgx_fputs("\r\n", r->out);
      fcgx_fputs("hello world!\r\n", r->out);
      //fcgx_fprintf(r->req.out, "thread: %ld", tid);
      
      printf("handled request\n");
    }

    if(r != NULL) {
      fcgx_req_finish(r);
      free(r);
    }
  }
  
  
  return app;
}

void* fsp_thrd_req_cleanup(struct fsp_app *app)
{
  if(app == NULL)
  {
    return NULL;
  }

  /*while(NULL != app->requests.list){
    for(unsigned i=0; i<REQUEST_LIMIT && app->requests.count > 0; i++)
    {
      if(app->requests.list[i].state == FSP_RS_SERVED)
      {
        printf("cleaning up request #%d\n", i+1);
        

        memset(&app->requests.list[i], 0, sizeof(struct fsp_req)); // just to be sure everything is reset..
        app->requests.count--;
      }
    }

    //sleep(1);
  }*/

  return app;
}
