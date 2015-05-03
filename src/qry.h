#pragma once

/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include "fsp.h"
#include "pool.h"

struct fsp_qry_item;

/**
 * query item type
 */
enum fsp_qry_type {
  FSP_QRY_UNK = 0, /* unknown */
  FSP_QRY_STR,     /* key=value */
  FSP_QRY_MAP      /* key[hello]=world&key[42]=answer */
};

/**
 * query-string
 */
struct fsp_qry {
  /* memory pool */
  struct fsp_pool pool;
  /* raw query-string*/
  const char *raw;
  /* offset pointer to *raw */
  const char *ptr;
  /* query items linked-list */
  struct fsp_qry_item *list;  
};

/**
 * query item
 */
struct fsp_qry_item {
  /* query field name */
  char *name;
  /* name length */
  size_t name_len;
  /* query field type */
  enum fsp_qry_type type;
  /* query value */
  union value {
    /* FSP_QRY_STR */
    char *str_val;
    /* FSP_QRY_MAP */
    struct fsp_qry_item *map_val;
  } value;
  /* counter to keep track of numeric offsets */
  int32_t index_max;
  /* next item in the list */
  struct fsp_qry_item *next;
  /* previous item in the list */
  struct fsp_qry_item *prev;
};

/**
 * initializes the query-parser
 *
 * @param  fsp_qry context
 * @return         true on success, false on failure
 */
bool fsp_qry_init(struct fsp_qry*);

/**
 * parse a query-string using the given cgi request
 *
 * @param  fsp_qry     context
 * @param  const char* input
 * @return          true on success, false on failure
 */
bool fsp_qry_parse(struct fsp_qry*, const char*);

/**
 * release all memory consumed by the query-string context
 *
 * @param fsp_qry context
 */
void fsp_qry_destroy(struct fsp_qry*);
