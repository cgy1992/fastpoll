/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "app.h"
#include "req.h"
 
/**
 * global initializer
 * 
 * @param    app-context
 * @return   true on success, false on failure
 */
FSP_API bool fsp_app_init(struct fsp_app *app FSP_UNUSED)
{
  return true;
}

/**
 * handles a request
 * 
 * @param    app-context
 * @param    request-context
 */
FSP_API void fsp_app_handle(struct fsp_app *app FSP_UNUSED, 
                            struct fsp_req *req)
{
  /* todo */
}

/**
 * global destructor
 * 
 * @param    app-context
 */
FSP_API void fsp_app_destroy(struct fsp_app *app FSP_UNUSED)
{
  /* todo */
}
