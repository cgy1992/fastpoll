/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "app.h"
#include "fcgx.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
 
/**
 * global initializer
 * 
 * @param    app-context
 * @return   true on success, false on failure
 */
FSP_API bool fsp_app_init(struct fsp_app *app FSP_UNUSED)
{
  if(app == NULL)
  {
    return false;
  }

  /* init fcgi handler */
  fcgx_init();

  /* init requests */
  fsp_req_init(app);

  return true;
}

/**
 * starts the main app routines
 * 
 * @param    app-context
 */
FSP_API void fsp_app_start(struct fsp_app *app)
{
  if(app == NULL)
  {
    return;
  }
  
  /* open unix socket */
  umask(0);  
  app->sid = fcgx_open_socket(FSP_SOCKET_PATH, 2000);
  
  /* spawn threads */
  pthread_create(&app->tid[0], NULL, (FSP_THREAD)fsp_thrd_req_manage, (void *)app);

  for (long i = 1; i < FSP_THREAD_COUNT; ++i)
    pthread_create(&app->tid[i], NULL, (FSP_THREAD)fsp_thrd_req_serve, (void *) app);
  
  /* start server on main thread */
  //fsp_thrd_req_cleanup(app);
  //fsp_thrd_req_serve(app);
  
  /* join threads */
  for (long i = 1; i < FSP_THREAD_COUNT; ++i)
    pthread_join(app->tid[i], NULL);
}

/**
 * stops the main app routines
 * 
 * @param    app-context
 */
FSP_API void fsp_app_stop(struct fsp_app *app FSP_UNUSED)
{

}

/**
 * global destructor
 * 
 * @param    app-context
 */
FSP_API void fsp_app_destroy(struct fsp_app *app)
{
  if(app == NULL)
  {
    return;
  }

  fsp_req_cleanup(app);
}
