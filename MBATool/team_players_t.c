#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_players_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_player_s *team_player = (team_player_s *)data;

     BIND_INT(  statement,  1, team_player->team_id   );
     BIND_INT(  statement,  2, team_player->season    );
     BIND_INT(  statement,  3, team_player->player_id );

     return SQLITE_OK;
}


int team_players_t_create( sqlite3 *db, const team_player_s *team_player )
{
     static char query[]   = "INSERT INTO Team_Players_T ( Team_Id, Season, Player_Id ) "
          /**/                                   "VALUES ( ?,"     "?,"    "?"       ")";

     return execute_update_old( db, query, team_players_t_create_bindings, team_player, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Team:
static int team_players_t_read_by_team_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_players_t_read_by_team_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_player_s team_player = { 0 };

     team_player.team_id   = sqlite3_column_int(  statement, 0 );
     team_player.season    = sqlite3_column_int(  statement, 1 );
     team_player.player_id = sqlite3_column_int(  statement, 2 );

     if ( add_to_data_list( data_list, &team_player, sizeof(team_player_s), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_players_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_players )
{
     static char query[] = "SELECT Team_Id, Season, Player_Id FROM Team_Players_T WHERE Team_Id = ?";

     return execute_query( db, query, team_players_t_read_by_team_bindings, &team_id, team_players_t_read_by_team_retrieve, team_players );
}

// By Team and Season:
static int team_players_t_read_by_team_and_season_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_player_s *team_player = (team_player_s *)data;

     BIND_INT( statement, 1, team_player->team_id );
     BIND_INT( statement, 2, team_player->season );

     return SQLITE_OK;
}

static int team_players_t_read_by_team_and_season_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_player_s team_player = { 0 };

     team_player.team_id   = sqlite3_column_int(  statement, 0 );
     team_player.season    = sqlite3_column_int(  statement, 1 );
     team_player.player_id = sqlite3_column_int(  statement, 2 );

     if ( add_to_data_list( data_list, &team_player, sizeof(team_player_s), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_players_t_read_by_team_and_season( sqlite3 *db, const team_player_s *team_player, data_list_s *team_players )
{
     static char query[] = "SELECT Team_Id, Season, Player_Id FROM Team_Players_T WHERE Team_Id = ? AND Season = ?";

     return execute_query( db, query, team_players_t_read_by_team_and_season_bindings, team_player, team_players_t_read_by_team_and_season_retrieve, team_players );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_players_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_player_s *team_player = (team_player_s *)data;

     BIND_INT(  statement,  1, team_player->team_id   );
     BIND_INT(  statement,  2, team_player->season    );
     BIND_INT(  statement,  3, team_player->player_id );

     return SQLITE_OK;
}


int team_players_t_delete( sqlite3 *db, const team_player_s *team_player )
{
     static char query[] = "DELETE FROM Team_Players_T WHERE Team_Id = ? AND Season = ? AND Player_Id = ?";

     return execute_update_old( db, query, team_players_t_delete_bindings, team_player, NULL, NULL );
}
