#ifndef __INC_SQL_QUERY_H__
#define __INC_SQL_QUERY_H__

#include <string.h>
#include <sqlite3.h>

#define cpytxt( DEST, SRC )   strncpy( (DEST), (char *)(SRC), sizeof(DEST) )

#define BIND_DBL( STMT, NN, VALUE ) if ( (rc = sqlite3_bind_double( (STMT), (NN), (VALUE)                               )) != SQLITE_OK ) return rc;
#define BIND_INT( STMT, NN, VALUE ) if ( (rc = sqlite3_bind_int(    (STMT), (NN), (VALUE)                               )) != SQLITE_OK ) return rc;
#define BIND_TXT( STMT, NN, VALUE ) if ( (rc = sqlite3_bind_text(   (STMT), (NN), (VALUE), strlen(VALUE), SQLITE_STATIC )) != SQLITE_OK ) return rc;


int execute_update(
     /**/  sqlite3  *db,
     /**/  char     *query,
     /**/  int     (*apply_bindings)(sqlite3_stmt *, const void *),
     const void     *data );

int execute_query(
     /**/  sqlite3  *db,
     /**/  char     *query,
     /**/  int     (*apply_bindings)(sqlite3_stmt *, const void *),
     const void     *data,
     /**/  int     (*retrieve_data)(sqlite3_stmt *, const void *),
     /**/  void     *output );


// *** DEPRECATED ***
int execute_update_old(
     /**/  sqlite3  *db,
     /**/  char     *query,
     /**/  int     (*apply_bindings)(sqlite3_stmt *, const void *),
     const void     *data,
     /**/  int     (*retrieve_data)(sqlite3_stmt *, const void *),
     /**/  void     *output );


#endif
