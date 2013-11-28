#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "league.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_teams_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_team_s *league_team = (league_team_s *)data;

     BIND_INT( statement, 1, league_team->league_id );
     BIND_INT( statement, 2, league_team->team_id   );

     return SQLITE_OK;
}


int league_teams_t_create( sqlite3 *db, const league_team_s *league_team )
{
     static char query[]   = "INSERT INTO League_Teams_T ( League_Id, Team_Id ) "
          /**/                                   "VALUES ( ?,"       "?"     ")";

     return execute_update_old( db, query, league_teams_t_create_bindings, league_team, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_teams_t_read_by_league_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *league_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *league_id );
}

static int league_teams_t_read_by_league_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     league_team_s league_team = { 0 };

     league_team.league_id = sqlite3_column_int( statement, 0 );
     league_team.team_id   = sqlite3_column_int( statement, 1 );

     if ( add_to_data_list( data_list, &league_team, sizeof(league_team_s), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int league_teams_t_read_by_league( sqlite3 *db, const int league_id, data_list_s *league_teams )
{
     static char query[] = "SELECT League_Id, Team_Id FROM League_Teams_T WHERE League_Id = ?";

     return execute_query( db, query, league_teams_t_read_by_league_bindings, &league_id, league_teams_t_read_by_league_retrieve, league_teams );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_teams_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_team_s *league_team = (league_team_s *)data;

     BIND_INT(  statement,  1, league_team->league_id );
     BIND_INT(  statement,  2, league_team->team_id   );
 
     return SQLITE_OK;
}


int league_teams_t_delete( sqlite3 *db, const league_team_s *league_team )
{
     static char query[] = "DELETE FROM League_Teams_T WHERE League_Id = ? AND Team_Id = ?";

     return execute_update_old( db, query, league_teams_t_delete_bindings, league_team, NULL, NULL );
}
