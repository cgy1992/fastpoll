#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */
#include "fsp.h"
#include "fcgx.h"

/* used for requests */
struct fsp_app;

/* used in fsp_app_handle(...) */
struct fsp_req {
  struct fcgx_req req;
  bool being_served;
};

struct fsp_req_list {
  struct fsp_req *stack, *ptr;
  size_t count;
};

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

