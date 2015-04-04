#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */
#include "fsp.h"
#include "fcgx.h"

#include <pthread.h>

/* used for requests */
struct fsp_app;

enum FSP_REQ_STATE
{
  FSP_RS_CLOSED = 0,
  FSP_RS_OPEN,
  FSP_RS_PROC,
  FSP_RS_SERVED
};

/* used in fsp_app_handle(...) */
struct fsp_req {
  struct fcgx_req *req;
  char state;
};

struct fsp_req_list {
  struct fsp_req *list;
  unsigned count;

  pthread_mutex_t mtx;
};

void fsp_req_init(struct fsp_app*);
void fsp_req_cleanup(struct fsp_app*);

void fsp_reqs_insert(struct fsp_app*, struct fcgx_req*);
struct fsp_req* fsp_reqs_get(struct fsp_app*);

void* fsp_thrd_req_manage(struct fsp_app*) FSP_UNUSED;
void* fsp_thrd_req_serve(struct fsp_app*) FSP_UNUSED;
void* fsp_thrd_req_cleanup(struct fsp_app*) FSP_UNUSED;

/*
  einzel-thread modell:
    20 threads; 200 anfragen: 10 anfragen pro thread, >> 20 << sekunden pro thread für alle anfragen

    statische beantwortungszeit von 2 sekunden pro thread je anfrage, 
    d.h. immer 20 sekunden pro thread für 10 anfragen bei statischer bearbeitung
      
  last-aufteilungs modell:
    20 threads; 200 anfragen; [v]orbereitungsthread (9, 0,5s), [b]earbeitungsthread(10, 0,5s), [a]ufräumthread(1, 1,0s)

    v und b haben hohe priorität, a nicht, da unessentiell für die beantwortung an sich
    
    Ax = A für anzahl threads, x ist der thread-typ {a,b,v}
    Tx = T für zeitaufwand pro thread je anfrage, ..

    beantwortungszeit = max((Anzahl Anfragen * Tv / Av), (Anzahl Anfragen * Tb / Ab))
                      => max((200 * 0,5s / 9), (200 * 0,5s / 10))
                      = max((100s / 9), (10s))

*/

