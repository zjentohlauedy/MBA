#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_stats_s *team_stats = (team_stats_s *)data;

     BIND_INT( statement,  1, team_stats->team_id         );
     BIND_INT( statement,  2, team_stats->season          );
     BIND_INT( statement,  3, team_stats->season_phase    );
     BIND_INT( statement,  4, team_stats->wins            );
     BIND_INT( statement,  5, team_stats->losses          );
     BIND_INT( statement,  6, team_stats->home_wins       );
     BIND_INT( statement,  7, team_stats->home_losses     );
     BIND_INT( statement,  8, team_stats->road_wins       );
     BIND_INT( statement,  9, team_stats->road_losses     );
     BIND_INT( statement, 10, team_stats->division_wins   );
     BIND_INT( statement, 11, team_stats->division_losses );
     BIND_INT( statement, 12, team_stats->league_wins     );
     BIND_INT( statement, 13, team_stats->league_losses   );
     BIND_INT( statement, 14, team_stats->runs_scored     );
     BIND_INT( statement, 15, team_stats->runs_allowed    );

     return SQLITE_OK;
}


int team_stats_t_create( sqlite3 *db, const team_stats_s *team_stats )
{
     static char query[]   = "INSERT INTO Team_Stats_T ( Team_Id, Season, Season_Phase, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Division_Wins, Division_Losses, League_Wins, League_Losses, Runs_Scored, Runs_Allowed )"
          /**/                                 "VALUES ( ?,"     "?,"    "?,"          "?,"  "?,"    "?,"       "?,"         "?,"       "?,"         "?,"           "?,"             "?,"         "?,"           "?,"         "?"          ")";

     return execute_update_old( db, query, team_stats_t_create_bindings, team_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int team_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_stats_s *team_stats = (team_stats_s *)data;

     BIND_INT( statement,  1, team_stats->team_id      );
     BIND_INT( statement,  2, team_stats->season       );
     BIND_INT( statement,  3, team_stats->season_phase );

     return SQLITE_OK;
}

static int team_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_stats_s *team_stats = (team_stats_s *)data;

     team_stats->wins            = sqlite3_column_int( statement,  0 );
     team_stats->losses          = sqlite3_column_int( statement,  1 );
     team_stats->home_wins       = sqlite3_column_int( statement,  2 );
     team_stats->home_losses     = sqlite3_column_int( statement,  3 );
     team_stats->road_wins       = sqlite3_column_int( statement,  4 );
     team_stats->road_losses     = sqlite3_column_int( statement,  5 );
     team_stats->division_wins   = sqlite3_column_int( statement,  6 );
     team_stats->division_losses = sqlite3_column_int( statement,  7 );
     team_stats->league_wins     = sqlite3_column_int( statement,  8 );
     team_stats->league_losses   = sqlite3_column_int( statement,  9 );
     team_stats->runs_scored     = sqlite3_column_int( statement, 10 );
     team_stats->runs_allowed    = sqlite3_column_int( statement, 11 );

     return SQLITE_OK;
}

int team_stats_t_read( sqlite3 *db, team_stats_s *team_stats )
{
     static char query[] = "SELECT Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Division_Wins, Division_Losses, League_Wins, League_Losses, Runs_Scored, Runs_Allowed "
          /**/               "FROM   Team_Stats_T "
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, team_stats_t_read_bindings, team_stats, team_stats_t_read_retrieve, team_stats );
}

// By Team:
static int team_stats_t_read_by_team_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_stats_t_read_by_team_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_stats_s team_stats = { 0 };

     team_stats.team_id         = sqlite3_column_int( statement,  0 );
     team_stats.season          = sqlite3_column_int( statement,  1 );
     team_stats.season_phase    = sqlite3_column_int( statement,  2 );
     team_stats.wins            = sqlite3_column_int( statement,  3 );
     team_stats.losses          = sqlite3_column_int( statement,  4 );
     team_stats.home_wins       = sqlite3_column_int( statement,  5 );
     team_stats.home_losses     = sqlite3_column_int( statement,  6 );
     team_stats.road_wins       = sqlite3_column_int( statement,  7 );
     team_stats.road_losses     = sqlite3_column_int( statement,  8 );
     team_stats.division_wins   = sqlite3_column_int( statement,  9 );
     team_stats.division_losses = sqlite3_column_int( statement, 10 );
     team_stats.league_wins     = sqlite3_column_int( statement, 11 );
     team_stats.league_losses   = sqlite3_column_int( statement, 12 );
     team_stats.runs_scored     = sqlite3_column_int( statement, 13 );
     team_stats.runs_allowed    = sqlite3_column_int( statement, 14 );

     if ( add_to_data_list( data_list, &team_stats, sizeof(team_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_stats_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_stats )
{
     static char query[] = "SELECT Team_Id, Season, Season_Phase, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Division_Wins, Division_Losses, League_Wins, League_Losses, Runs_Scored, Runs_Allowed FROM Team_Stats_T WHERE Team_Id = ?";

     return execute_query( db, query, team_stats_t_read_by_team_bindings, &team_id, team_stats_t_read_by_team_retrieve, team_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_stats_s *team_stats = (team_stats_s *)data;

     BIND_INT(  statement,  1, team_stats->wins            );
     BIND_INT(  statement,  2, team_stats->losses          );
     BIND_INT(  statement,  3, team_stats->home_wins       );
     BIND_INT(  statement,  4, team_stats->home_losses     );
     BIND_INT(  statement,  5, team_stats->road_wins       );
     BIND_INT(  statement,  6, team_stats->road_losses     );
     BIND_INT(  statement,  7, team_stats->division_wins   );
     BIND_INT(  statement,  8, team_stats->division_losses );
     BIND_INT(  statement,  9, team_stats->league_wins     );
     BIND_INT(  statement, 10, team_stats->league_losses   );
     BIND_INT(  statement, 11, team_stats->runs_scored     );
     BIND_INT(  statement, 12, team_stats->runs_allowed    );

     BIND_INT(  statement, 13, team_stats->team_id      );
     BIND_INT(  statement, 14, team_stats->season       );
     BIND_INT(  statement, 15, team_stats->season_phase );

     return SQLITE_OK;
}


int team_stats_t_update( sqlite3 *db, const team_stats_s *team_stats )
{
     static char query[]   = "UPDATE Team_Stats_T "
          /**/
          /**/               "SET    Wins            = ?,"
          /**/                      "Losses          = ?,"
          /**/                      "Home_Wins       = ?,"
          /**/                      "Home_Losses     = ?,"
          /**/                      "Road_Wins       = ?,"
          /**/                      "Road_Losses     = ?,"
          /**/                      "Division_Wins   = ?,"
          /**/                      "Division_Losses = ?,"
          /**/                      "League_Wins     = ?,"
          /**/                      "League_Losses   = ?,"
          /**/                      "Runs_Scored     = ?,"
          /**/                      "Runs_Allowed    = ? "
          /**/
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_update_old( db, query, team_stats_t_update_bindings, team_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_stats_s *team_stats = (team_stats_s *)data;

     BIND_INT( statement,  1, team_stats->team_id      );
     BIND_INT( statement,  2, team_stats->season       );
     BIND_INT( statement,  3, team_stats->season_phase );

     return SQLITE_OK;
}


int team_stats_t_delete( sqlite3 *db, const team_stats_s *team_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Stats_T WHERE Team_Id = ? AND Season = ? AND Season_Phase = ?", team_stats_t_delete_bindings, team_stats, NULL, NULL );
}
