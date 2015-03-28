/*!
 * FastCGI Poll "FastPoll"
 * Copyright (C) 2015 "asc"
 * License: MIT (see LICENSE.md)
 */

#include <fcgi_stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define UNUSED __attribute__((unused))
 ; /* sublime bug */

/**
 * fastpoll context
 */
struct fsp {
  /* persistent data etc... */
};

void fsp_init(struct fsp*);
void fsp_clear(struct fsp*);
void fsp_serv(struct fsp*);
void fsp_resp(struct fsp*, uint32_t, const char*);
void fsp_puts(struct fsp*, const char*);

/* routes */
void fsp_rt_home(struct fsp*);
void fsp_rt_vote(struct fsp*);
void fsp_rt_poll(struct fsp*);
void fsp_rt_e404(struct fsp*);

#define dupenv(n) strdup(getenv(n))

/**
 * initializer
 * @param app  application context
 */
void fsp_init(struct fsp *app UNUSED)
{
  /* todo */
}

/**
 * destructor
 * @param app  application context
 */
void fsp_clear(struct fsp *app UNUSED)
{
  /* todo */
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
  printf("\r\n\r\n");
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

#define TPL_DOC_BEG ""                                                         \
"<!DOCTYPE html>"                                                              \
"<html lang=\"en\">"                                                           \
  "<head>"                                                                     \
    "<meta charset=\"utf-8\">"                                                 \
    "<meta name=\"viewport\" content=\"inital-scale=1.0, "                     \
      "width=device-width\">"                                                  \
    ""                                                                         \
    "<link rel=\"stylesheet\" href=\"css/main.css\">"                          \
    "<link rel=\"stylesheet\" href=\"css/mobi.css\" "                          \
      "media=\"all and (max-width: 799px)\">"                                  \
    ""                                                                         \
    "<title>FasterPoll</title>"                                                \
  "</head>"                                                                    \
  "<body>"

#define TPL_HEADER ""                                                          \
"<header id=\"head\">"                                                         \
  "<div class=\"page\">"                                                       \
    "<h1>FasterPoll</h1>"                                                      \
    "<nav id=\"tnav\">"                                                        \
      "<a href=\"index.html\">Home</a>"                                        \
      "<a href=\"vote.html\">Vote</a>"                                         \
      "<a href=\"result.html\">Result</a>"                                     \
    "</nav>"                                                                   \
  "</div>"                                                                     \
"</header>"

#define TPL_MAIN_BEG ""                                                        \
"<main role=\"main\" id=\"main\">"                                             \
  "<div class=\"page\">"

#define TPL_MAIN_END ""                                                        \
  "</div>"                                                                     \
"</main>"

#define TPL_FOOTER ""                                                          \
"<footer id=\"foot\">"                                                         \
  "<p class=\"page copy\">&copy; 2015 FastPoll</p>"                            \
  "<p class=\"page\"><a href=\"#\">View source on Github</a></p>"              \
"</footer>"

#define TPL_DOC_END ""                                                         \
  "<script src=\"js/main.js\" defer></script>"                                 \
  "</body>"                                                                    \
"</html>"

#define TPL_BEG  \
  TPL_DOC_BEG    \
    TPL_HEADER   \
    TPL_MAIN_BEG
    
#define TPL_END  \
    TPL_MAIN_END \
    TPL_FOOTER   \
  TPL_DOC_END

/**
 * home route
 * @param app
 */
void fsp_rt_home(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, 
    TPL_BEG
    "<h2>Hello world!</h2>"
    "<div class=\"cbox view\">"
      "<p>It fucking works!</p>"
    "</div>"
    TPL_END
  );
}

/**
 * vote route
 * @param app
 */
void fsp_rt_vote(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>Vote</h1>");
}

/**
 * poll route
 * @param app
 */
void fsp_rt_poll(struct fsp *app)
{
  fsp_resp(app, 200, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>Poll</h1>");
}

/**
 * 404 not found route
 * @param app
 */
void fsp_rt_e404(struct fsp *app)
{
  fsp_resp(app, 404, "text/html; Charset=UTF-8");
  fsp_puts(app, "<!DOCTYPE html><h1>404 - Not Found</h1>");
}

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
  fsp_init(&app);
  
  /* fcgi loop */
  while (FCGI_Accept() >= 0)
    fsp_serv(&app);
  
  /* shutdown app */
  fsp_clear(&app);
  return 0;
}
