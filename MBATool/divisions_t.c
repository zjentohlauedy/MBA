#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int divisions_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_s *division = (division_s *)data;

     BIND_INT( statement, 1, division->division_id );
     BIND_TXT( statement, 2, division->name        );

     return SQLITE_OK;
}


int divisions_t_create( sqlite3 *db, const division_s *division )
{
     static char query[]   = "INSERT INTO Divisions_T ( Division_Id, Name )"
          /**/                                "VALUES ( ?,"         "?"  ")";

     return execute_query( db, query, divisions_t_create_bindings, division, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int divisions_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const division_s *division = (division_s *)data;

     return sqlite3_bind_int( statement, 1, division->division_id );
}

static int divisions_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     division_s *division = (division_s *)data;

     cpytxt( division->name,             sqlite3_column_text( statement, 0 ) );

     return SQLITE_OK;
}

int divisions_t_read( sqlite3 *db, division_s *division )
{
     static char query[] = "SELECT Name FROM Divisions_T WHERE Division_Id = ?";

     return execute_query( db, query, divisions_t_read_bindings, division, divisions_t_read_retrieve, division );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int divisions_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_s *division = (division_s *)data;

     BIND_TXT( statement, 1, division->name );

     BIND_INT( statement, 2, division->division_id );

     return SQLITE_OK;
}


int divisions_t_update( sqlite3 *db, const division_s *division )
{
     static char query[]   = "UPDATE Divisions_T "
          /**/
          /**/               "SET    Name = ? "
          /**/
          /**/               "WHERE  Division_Id = ?";

     return execute_query( db, query, divisions_t_update_bindings, division, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int divisions_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const division_s *division = (division_s *)data;

     return sqlite3_bind_int( statement, 1, division->division_id );
}


int divisions_t_delete( sqlite3 *db, const division_s *division )
{
     return execute_query( db, "DELETE FROM Divisions_T WHERE Division_Id = ?", divisions_t_delete_bindings, division, NULL, NULL );
}
