/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

#include <fcgi_stdio.h>
 
#include "fsp.h"

/**
 * main
 * @param  argc
 * @param  argv
 * @return      exit-code
 */
int main(int argc UNUSED, char **argv UNUSED) 
{
  /* init app */
  struct fsp app;
  
  if (!fsp_init(&app))
    return 1;
  
  /* fcgi loop */
  while (FCGI_Accept() >= 0)
    fsp_serv(&app);
  
  /* shutdown app */
  fsp_clear(&app);
  return 0;
}
