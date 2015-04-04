#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

/* note: do not include fgci_stdio.h here */

#include <stddef.h>  /* size_t, NULL etc. */
#include <stdint.h>  /* int32_t etc. */
#include <stdbool.h> /* bool, true, false */

#ifdef _WIN32
 #ifndef __MINGW32__
  #error get a real c compiler and try again.
 #endif
 /* required to make func-defs visible in windows dll's */
 #define FSP_API __declspec(dllexport)
#else
 #define FSP_API
#endif

/* debug-mode: 1 = on, 0 = off */
#define FSP_DEBUG 1

/* lazy macro, allow unused vars */ 
#define FSP_UNUSED __attribute__((unused))
