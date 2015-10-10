#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_versus_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_versus_stats_s *team_versus_stats = (team_versus_stats_s *)data;

     BIND_INT( statement, 1, team_versus_stats->team_id      );
     BIND_INT( statement, 2, team_versus_stats->season       );
     BIND_INT( statement, 3, team_versus_stats->season_phase );
     BIND_INT( statement, 4, team_versus_stats->opponent     );
     BIND_INT( statement, 5, team_versus_stats->wins         );
     BIND_INT( statement, 6, team_versus_stats->losses       );
     BIND_INT( statement, 7, team_versus_stats->runs_scored  );
     BIND_INT( statement, 8, team_versus_stats->runs_allowed );

     return SQLITE_OK;
}


int team_versus_stats_t_create( sqlite3 *db, const team_versus_stats_s *team_versus_stats )
{
     static char query[]   = "INSERT INTO Team_Versus_Stats_T ( Team_Id, Season, Season_Phase, Opponent, Wins, Losses, Runs_Scored, Runs_Allowed )"
          /**/                                        "VALUES ( ?,"     "?,"    "?,"          "?,"      "?,"  "?,"    "?,"         "?"          ")";

     return execute_update_old( db, query, team_versus_stats_t_create_bindings, team_versus_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int team_versus_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_versus_stats_s *team_versus_stats = (team_versus_stats_s *)data;

     BIND_INT( statement,  1, team_versus_stats->team_id      );
     BIND_INT( statement,  2, team_versus_stats->season       );
     BIND_INT( statement,  3, team_versus_stats->season_phase );
     BIND_INT( statement,  4, team_versus_stats->opponent     );

     return SQLITE_OK;
}

static int team_versus_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_versus_stats_s *team_versus_stats = (team_versus_stats_s *)data;

     team_versus_stats->wins         = sqlite3_column_int( statement,  0 );
     team_versus_stats->losses       = sqlite3_column_int( statement,  1 );
     team_versus_stats->runs_scored  = sqlite3_column_int( statement,  2 );
     team_versus_stats->runs_allowed = sqlite3_column_int( statement,  3 );

     return SQLITE_OK;
}

int team_versus_stats_t_read( sqlite3 *db, team_versus_stats_s *team_versus_stats )
{
     static char query[]   = "SELECT Wins, Losses, Runs_Scored, Runs_Allowed "
          /**/               "FROM   Team_Versus_Stats_T "
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? "
          /**/               "AND    Opponent     = ? ";

     return execute_query( db, query, team_versus_stats_t_read_bindings, team_versus_stats, team_versus_stats_t_read_retrieve, team_versus_stats );
}

// By Team:
static int team_versus_stats_t_read_by_team_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_versus_stats_t_read_by_team_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_versus_stats_s team_versus_stats = { 0 };

     team_versus_stats.team_id      = sqlite3_column_int( statement,  0 );
     team_versus_stats.season       = sqlite3_column_int( statement,  1 );
     team_versus_stats.season_phase = sqlite3_column_int( statement,  2 );
     team_versus_stats.opponent     = sqlite3_column_int( statement,  3 );
     team_versus_stats.wins         = sqlite3_column_int( statement,  4 );
     team_versus_stats.losses       = sqlite3_column_int( statement,  5 );
     team_versus_stats.runs_scored  = sqlite3_column_int( statement,  6 );
     team_versus_stats.runs_allowed = sqlite3_column_int( statement,  7 );

     if ( add_to_data_list( data_list, &team_versus_stats, sizeof(team_versus_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_versus_stats_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_versus_stats )
{
     static char query[] = "SELECT Team_Id, Season, Season_Phase, Opponent, Wins, Losses, Runs_Scored, Runs_Allowed FROM Team_Versus_Stats_T WHERE Team_Id = ?";

     return execute_query( db, query, team_versus_stats_t_read_by_team_bindings, &team_id, team_versus_stats_t_read_by_team_retrieve, team_versus_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_versus_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_versus_stats_s *team_versus_stats = (team_versus_stats_s *)data;

     BIND_INT(  statement,  1, team_versus_stats->wins         );
     BIND_INT(  statement,  2, team_versus_stats->losses       );
     BIND_INT(  statement,  3, team_versus_stats->runs_scored  );
     BIND_INT(  statement,  4, team_versus_stats->runs_allowed );

     BIND_INT(  statement,  5, team_versus_stats->team_id      );
     BIND_INT(  statement,  6, team_versus_stats->season       );
     BIND_INT(  statement,  7, team_versus_stats->season_phase );
     BIND_INT(  statement,  8, team_versus_stats->opponent     );

     return SQLITE_OK;
}


int team_versus_stats_t_update( sqlite3 *db, const team_versus_stats_s *team_versus_stats )
{
     static char query[]   = "UPDATE Team_Versus_Stats_T "
          /**/
          /**/               "SET    Wins         = ?,"
          /**/                      "Losses       = ?,"
          /**/                      "Runs_Scored  = ?,"
          /**/                      "Runs_allowed = ? "
          /**/
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? "
          /**/               "AND    Opponent     = ? ";

     return execute_update_old( db, query, team_versus_stats_t_update_bindings, team_versus_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_versus_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_versus_stats_s *team_versus_stats = (team_versus_stats_s *)data;

     BIND_INT( statement,  1, team_versus_stats->team_id      );
     BIND_INT( statement,  2, team_versus_stats->season       );
     BIND_INT( statement,  3, team_versus_stats->season_phase );
     BIND_INT( statement,  4, team_versus_stats->opponent     );

     return SQLITE_OK;
}


int team_versus_stats_t_delete( sqlite3 *db, const team_versus_stats_s *team_versus_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Versus_Stats_T WHERE Team_Id = ? AND Season = ? AND Season_Phase = ? AND Opponent = ?", team_versus_stats_t_delete_bindings, team_versus_stats, NULL, NULL );
}
