/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <fcgi_stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fsp.h"
#include "fsp_rt.h"
#include "fsp_db.h"

#define dupenv(n) strdup(getenv(n))

/**
 * initializer
 * @param app  application context
 */
bool fsp_init(struct fsp *app UNUSED)
{
  /* setup db */
  if (!fsp_db_open(&app->db)) {
    fputs("database error", stderr);
    return false;
  }
  
  return true;
}

/**
 * destructor
 * @param app  application context
 */
void fsp_clear(struct fsp *app UNUSED)
{
  /* close database */
  fsp_db_close(&app->db);
}

/**
 * request handler (server)
 * @param app  application context
 */
void fsp_serv(struct fsp *app)
{
  /* route */
  char *req_uri = dupenv("REQUEST_URI");
  
  /* hide query-string in uri */
  char *qsp = strchr(req_uri, '?');
  if (qsp != 0) *qsp = 0;
  
  /* get uri-length */
  size_t rlen = strlen(req_uri);
  
  /**
   * note: those routes must be configured in nginx
   * location ~ /(?:vote|poll)?$ { ... }
   */
  
  if (strncmp(req_uri, "/", rlen) == 0)
    /* serve home (index) */
    fsp_rt_home(app);
  else if (strncmp(req_uri, "/vote", rlen) == 0)
    /* serve vote page */
    fsp_rt_vote(app);
  else if (strncmp(req_uri, "/poll", rlen) == 0)
    /* serve poll (result / overview) */
    fsp_rt_poll(app);
  else
    /* serve 404 */
    fsp_rt_e404(app);
  
  free(req_uri);
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
  printf("\r\n");
}

/**
 * output data 
 * @param app 
 * @param tx    data
 */
void fsp_puts(struct fsp *app UNUSED, const char *tx)
{
  puts(tx);
}
