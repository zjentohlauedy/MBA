#include <string.h>
#include <sqlite3.h>
#include "sql_query.h"


int execute_update(
     /**/  sqlite3  *db,
     /**/  char     *query,
     /**/  int     (*apply_bindings)(sqlite3_stmt *, const void *),
     const void     *data )
{
     sqlite3_stmt *statement;
     int           rc;

     if ( (rc = sqlite3_prepare_v2( db, query, strlen(query), &statement, NULL )) != SQLITE_OK ) return rc;

     if ( apply_bindings != NULL  &&  (rc = apply_bindings( statement, data )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_step( statement )) != SQLITE_DONE ) return rc;

     return sqlite3_finalize( statement );
}

int execute_update_old(
     /**/  sqlite3  *db,
     /**/  char     *query,
     /**/  int     (*apply_bindings)(sqlite3_stmt *, const void *),
     const void     *data,
     /**/  int     (*retrieve_data)(sqlite3_stmt *, const void *),
     /**/  void     *output )
{
     return execute_update( db, query, apply_bindings, data );
}

int execute_query(
     /**/  sqlite3  *db,
     /**/  char     *query,
     /**/  int     (*apply_bindings)(sqlite3_stmt *, const void *),
     const void     *data,
     /**/  int     (*retrieve_data)(sqlite3_stmt *, const void *),
     /**/  void     *output )
{
     sqlite3_stmt *statement;
     int           rc;

     if ( (rc = sqlite3_prepare_v2( db, query, strlen(query), &statement, NULL )) != SQLITE_OK ) return rc;

     if ( apply_bindings != NULL  &&  (rc = apply_bindings( statement, data )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_step( statement )) == SQLITE_DONE )
     {
          if ( (rc = sqlite3_finalize( statement )) != SQLITE_OK ) return rc;

          return SQLITE_NOTFOUND;
     }

     while ( rc == SQLITE_ROW )
     {
          if ( retrieve_data != NULL  &&  (rc = retrieve_data( statement, output )) != SQLITE_OK ) return rc;

          rc = sqlite3_step( statement );
     }

     if ( rc != SQLITE_DONE ) return rc;

     return sqlite3_finalize( statement );
}

