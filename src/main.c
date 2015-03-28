/*!
 * FastCGI Poll "FasterPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENCE.md)
 */

#include <fcgi_stdio.h>
#include <stdint.h>

#define UNUSED __attribute__((unused))


/**
 * fasterpoll context
 */
struct fsp {
  uint32_t len; /* content length */
};

void fsp_init(struct fsp*);
void fsp_serv(struct fsp*);
void fsp_resp(struct fsp*, uint32_t, const char*);
void fsp_puts(struct fsp*, const char*);

/**
 * initializer
 * @param app  application context
 */
void fsp_init(struct fsp *app UNUSED)
{
  /* todo */
}

/**
 * request handler (server)
 * @param app  application context
 */
void fsp_serv(struct fsp *app)
{
  /* todo */
  fsp_resp(app, 200, "text/html; Charset=UTF8");
  fsp_puts(app, "<!DOCTYPE html>\nhello world!");
}

/**
 * start response
 * @param app 
 * @param sc   status code
 * @param ct   content-type
 */
void fsp_resp(struct fsp *app UNUSED, uint32_t sc, const char *ct)
{
  printf("Status: %u\r\n", sc);
  printf("Content-Type: %s\r\n", ct);
  printf("\r\n\r\n");
}

/**
 * output data 
 * @param app 
 * @param tx    data
 */
void fsp_puts(struct fsp *app UNUSED, const char *tx)
{
  printf("%s", tx);
}

/**
 * main-loop
 * @param  argc
 * @param  argv
 * @return      exit-code
 */
int main(int argc UNUSED, char **argv UNUSED) 
{
  struct fsp app;
  fsp_init(&app);
  
  /* fcgi loop */
  while (FCGI_Accept() >= 0)
    fsp_serv(&app);
  
  return 0;
}
