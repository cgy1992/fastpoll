/*!
 * FastPoll - FastCGI Poll
 * @copyright 2015 The FastPoll authors
 */

#include <mysql.h>

#include "fsp.h"
#include "fsp_db.h"

#define DB_NAME "fastpoll"
#define DB_USER "fastpoll"
#define DB_PASS "1234"

/**
 * init and open databse
 * @param  db     context
 * @return        status
 */
bool fsp_db_open(struct fsp_db *db)
{
  db->is_open = true;
  return true;
}

/**
 * close and cleanup database
 * @param db   context
 */
void fsp_db_close(struct fsp_db *db)
{
  db->is_open = false;
}
