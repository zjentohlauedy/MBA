#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "league.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_divisions_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_division_s *league_division = (league_division_s *)data;

     BIND_INT( statement, 1, league_division->league_id   );
     BIND_INT( statement, 2, league_division->division_id );

     return SQLITE_OK;
}


int league_divisions_t_create( sqlite3 *db, const league_division_s *league_division )
{
     static char query[]   = "INSERT INTO League_Divisions_T ( League_Id, Division_Id ) "
          /**/                                       "VALUES ( ?,"       "?"         ")";

     return execute_query( db, query, league_divisions_t_create_bindings, league_division, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_divisions_t_read_by_league_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *league_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *league_id );
}

static int league_divisions_t_read_by_league_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     league_division_s league_division = { 0 };

     league_division.league_id   = sqlite3_column_int( statement, 0 );
     league_division.division_id = sqlite3_column_int( statement, 1 );

     if ( add_to_data_list( data_list, &league_division, sizeof(league_division), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int league_divisions_t_read_by_league( sqlite3 *db, const int league_id, data_list_s *league_divisions )
{
     static char query[] = "SELECT League_Id, Division_Id FROM League_Divisions_T WHERE League_Id = ?";

     return execute_query( db, query, league_divisions_t_read_by_league_bindings, &league_id, league_divisions_t_read_by_league_retrieve, league_divisions );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_divisions_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_division_s *league_division = (league_division_s *)data;

     BIND_INT( statement, 1, league_division->league_id   );
     BIND_INT( statement, 2, league_division->division_id );

     return SQLITE_OK;
}


int league_divisions_t_delete( sqlite3 *db, const league_division_s *league_division )
{
     static char query[] = "DELETE FROM League_Divisions_T WHERE League_Id = ? AND Division_Id = ?";

     return execute_query( db, query, league_divisions_t_delete_bindings, league_division, NULL, NULL );
}
