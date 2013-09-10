#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "division.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_teams_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_team_s *division_team = (division_team_s *)data;

     BIND_INT( statement, 1, division_team->division_id );
     BIND_INT( statement, 2, division_team->team_id     );

     return SQLITE_OK;
}


int division_teams_t_create( sqlite3 *db, const division_team_s *division_team )
{
     static char query[]   = "INSERT INTO Division_Teams_T ( Division_Id, Team_Id ) "
          /**/                                     "VALUES ( ?,"         "?"     ")";

     return execute_query( db, query, division_teams_t_create_bindings, division_team, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_teams_t_read_by_division_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *division_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *division_id );
}

static int division_teams_t_read_by_division_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     division_team_s division_team = { 0 };

     division_team.division_id = sqlite3_column_int( statement, 0 );
     division_team.team_id     = sqlite3_column_int( statement, 1 );

     if ( add_to_data_list( data_list, &division_team, sizeof(division_team_s), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int division_teams_t_read_by_division( sqlite3 *db, const int division_id, data_list_s *division_teams )
{
     static char query[] = "SELECT Division_Id, Team_Id FROM Division_Teams_T WHERE Division_Id = ?";

     return execute_query( db, query, division_teams_t_read_by_division_bindings, &division_id, division_teams_t_read_by_division_retrieve, division_teams );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_teams_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_team_s *division_team = (division_team_s *)data;

     BIND_INT(  statement,  1, division_team->division_id );
     BIND_INT(  statement,  2, division_team->team_id     );

     return SQLITE_OK;
}


int division_teams_t_delete( sqlite3 *db, const division_team_s *division_team )
{
     static char query[] = "DELETE FROM Division_Teams_T WHERE Division_Id = ? AND Team_Id = ?";

     return execute_query( db, query, division_teams_t_delete_bindings, division_team, NULL, NULL );
}
