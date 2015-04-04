/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stdio.h> 

#include "fsp.h"
#include "app.h"

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
  
  fsp_app_start(&app);
  
  /* shutdown */
  fsp_app_destroy(&app);
  return 0;
}
