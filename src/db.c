/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <stdio.h>

#include "db.h"

#define DB_HOST "localhost"
#define DB_USER "fastpoll"
#define DB_PASS "42T9wt2Pca5ZqmZR"
#define DB_NAME "fastpoll"

/**
 * initializer
 *   
 * @param    db context
 * @return   true on success, false on failure
 */
bool fsp_db_init(struct fsp_db *db)
{
  mtx_init(&db->query_mtx, 0);
  mysql_init(&db->dbh);
  
  /* set options */
  my_bool rc = 1;
  mysql_options(&db->dbh, MYSQL_OPT_RECONNECT, &rc);
  mysql_options(&db->dbh, MYSQL_SET_CHARSET_NAME, "utf8");
  
  /* connect */
  if (!mysql_real_connect(&db->dbh, DB_HOST, DB_USER, 
                          DB_PASS, DB_NAME, 0, 0, 0)) {
    fputs(mysql_error(&db->dbh), stderr);
    fputs("\n", stderr);
    return false;
  }
  
  /* looks good */
  return true;
}

/**
 * destructor
 * 
 * @param   db context
 */
void fsp_db_destroy(struct fsp_db *db)
{
  mtx_destroy(&db->query_mtx);
  mysql_close(&db->dbh);
}
