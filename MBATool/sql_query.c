#include <string.h>
#include <sqlite3.h>
#include "sql_query.h"

int execute_query( sqlite3 *db, char *query, int (*apply_bindings)(sqlite3_stmt *, const void *), const void *data )
{
     sqlite3_stmt *statement;
     int           rc;

     if ( (rc = sqlite3_prepare_v2( db, query, strlen(query), &statement, NULL )) != SQLITE_OK ) return rc;

     if ( (rc = apply_bindings( statement, data )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_step( statement )) != SQLITE_DONE ) return rc;

     return sqlite3_finalize( statement );
}
