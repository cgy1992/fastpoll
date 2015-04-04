#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

/* not really an abstraction, but replaceable */

#include <fcgi_config.h>
#include <fcgiapp.h>

#define fcgx_is_cgi             FCGX_IsCGI
#define fcgx_init               FCGX_Init
#define fcgx_open_socket        FCGX_OpenSocket
 
#define fcgx_req                FCGX_Request
#define fcgx_req_init           FCGX_InitRequest
#define fcgx_req_accept         FCGX_Accept_r
#define fcgx_req_finish         FCGX_Finish_r
#define fcgx_req_param(r,n)     FCGX_GetParam(n, (r)->envp)
 
#define fcgx_free               FCGX_Free
#define fcgx_accept             FCGX_Accept
#define fcgx_finish             FCGX_Finish
 
#define fcgx_start_filter_data  FCGX_StartFilterData
#define fcgx_set_exit_status    FCGX_SetExitStatus
 
#define fcgx_fgetc              FCGX_GetChar
#define fcgx_fungetc            FCGX_UnGetChar
#define fcgx_fgets              FCGX_GetStr
#define fcgx_fgetln             FCGX_GetLine
#define fcgx_feof               FCGX_HasSeenEOF
#define fcgx_fputc              FCGX_PutChar
#define fcgx_fwrite             FCGX_PutStr
#define fcgx_fputs              FCGX_PutS
#define fcgx_fprintf            FCGX_FPrintF
#define fcgx_fvprintf           FCGX_VFPrintF
#define fcgx_fflush             FCGX_FFlush
#define fcgx_fclose             FCGX_FClose
 
#define fcgx_error              FCGX_GetError
#define fcgx_error_clear        FCGX_ClearError
#define fcgx_create_writer      FCGX_CreateWriter
#define fcgx_free_stream        FCGX_FreeStream
#define fcgx_shutdown_pending   FCGX_ShutdownPending
