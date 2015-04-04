/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <pthread.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 

#include "fsp.h"
#include "app.h"
#include "fcgx.h"

/* number of threads */
#define THREAD_COUNT 20

/* unix socket-path */
#define SOCKET_PATH "/var/run/fastpoll.sock"

/* socket fd */
static int sock_id;

/**
 * pthread callback
 * 
 * @param  a  global application context
 * @return    nothing at all
 */
static void *serve(void *a)
{
  /* restore void-pointer */
  //struct fsp_app *app __attribute__((unused)) = a;
  long tid = (long) a;
  
  /* init request */
  struct fcgx_req req;
  
  if (fcgx_req_init(&req, sock_id, 0) != 0) {
    puts("error: could not initialize fcgi request");
    return 0;
  }
  
  int rc; /* used for fcgx_req_accept() */
  pthread_mutex_t mtx;
  
  pthread_mutex_init(&mtx, 0);
  
  /* start accept loop */
  for (;;) {
    pthread_mutex_lock(&mtx);
    rc = fcgx_req_accept(&req);
    pthread_mutex_unlock(&mtx);
    
    if (rc < 0) {
      puts("warn: cannot accept requests, thread will exit");
      break;
    }
    
    //printf("new request on thread %ld\n", tid);
    
    fcgx_fputs("Status: 200\r\n", req.out);
    fcgx_fputs("Content-Type: text/plain; Charset=UTF-8\r\n", req.out);
    fcgx_fputs("\r\n", req.out);
    fcgx_fputs("hello world!\r\n", req.out);
    fcgx_fprintf(req.out, "thread: %ld", tid);
    
    fcgx_req_finish(&req);
    sleep(1);
  }
  
  pthread_mutex_destroy(&mtx);
  return 0;
}

/**
 * installs the fcgi handler and worker-threads
 * 
 * @param app   global application context
 */
static void listen(struct fsp_app *app FSP_UNUSED)
{
  pthread_t id[THREAD_COUNT]; 

  /* init fcgi handler */
  fcgx_init();
  
  /* open unix socket */
  umask(0);  
  sock_id = fcgx_open_socket(SOCKET_PATH, 2000);
  
  /* spawn threads */
  for (long i = 1; i < THREAD_COUNT; ++i)
    pthread_create(&id[i], NULL, serve, (void *) i);
  
  /* start server on main thread */
  serve((void *) 0l);
  
  /* join threads */
  for (long i = 1; i < THREAD_COUNT; ++i)
    pthread_join(id[i], NULL);
}

/**
 * main
 * 
 * @return  exit code
 */
int main(void)
{
  /* global app context */
  struct fsp_app app;
  
  if (!fsp_app_init(&app))
    return 1;
  
  /* fcgi handler */
  listen(&app);
  
  /* shutdown */
  fsp_app_destroy(&app);
  return 0;
}
