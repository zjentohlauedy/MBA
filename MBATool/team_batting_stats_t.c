#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_batting_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_batting_stats_s *team_batting_stats = (team_batting_stats_s *)data;

     BIND_INT( statement,  1, team_batting_stats->team_id        );
     BIND_INT( statement,  2, team_batting_stats->season         );
     BIND_INT( statement,  3, team_batting_stats->season_phase   );
     BIND_INT( statement,  4, team_batting_stats->games          );
     BIND_INT( statement,  5, team_batting_stats->at_bats        );
     BIND_INT( statement,  6, team_batting_stats->runs           );
     BIND_INT( statement,  7, team_batting_stats->hits           );
     BIND_INT( statement,  8, team_batting_stats->doubles        );
     BIND_INT( statement,  9, team_batting_stats->triples        );
     BIND_INT( statement, 10, team_batting_stats->home_runs      );
     BIND_INT( statement, 11, team_batting_stats->runs_batted_in );
     BIND_INT( statement, 12, team_batting_stats->walks          );
     BIND_INT( statement, 13, team_batting_stats->strike_outs    );
     BIND_INT( statement, 14, team_batting_stats->steals         );
     BIND_INT( statement, 15, team_batting_stats->errors         );

     return SQLITE_OK;
}


int team_batting_stats_t_create( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     static char query[]   = "INSERT INTO Team_Batting_Stats_T ( Team_Id, Season, Season_Phase, Games, At_Bats, Runs, Hits, Doubles, Triples, Home_Runs, Runs_Batted_In, Walks, Strike_Outs, Steals, Errors )"
          /**/                                         "VALUES ( ?,"     "?,"    "?,"          "?,"   "?,"     "?,"  "?,"  "?,"     "?,"     "?,"       "?,"            "?,"   "?,"         "?,"    "?"    ")";

     return execute_query( db, query, team_batting_stats_t_create_bindings, team_batting_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_batting_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_batting_stats_s *team_batting_stats = (team_batting_stats_s *)data;

     BIND_INT( statement,  1, team_batting_stats->team_id      );
     BIND_INT( statement,  2, team_batting_stats->season       );
     BIND_INT( statement,  3, team_batting_stats->season_phase );

     return SQLITE_OK;
}

static int team_batting_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_batting_stats_s *team_batting_stats = (team_batting_stats_s *)data;

     team_batting_stats->games          = sqlite3_column_int( statement,  0 );
     team_batting_stats->at_bats        = sqlite3_column_int( statement,  1 );
     team_batting_stats->runs           = sqlite3_column_int( statement,  2 );
     team_batting_stats->hits           = sqlite3_column_int( statement,  3 );
     team_batting_stats->doubles        = sqlite3_column_int( statement,  4 );
     team_batting_stats->triples        = sqlite3_column_int( statement,  5 );
     team_batting_stats->home_runs      = sqlite3_column_int( statement,  6 );
     team_batting_stats->runs_batted_in = sqlite3_column_int( statement,  7 );
     team_batting_stats->walks          = sqlite3_column_int( statement,  8 );
     team_batting_stats->strike_outs    = sqlite3_column_int( statement,  9 );
     team_batting_stats->steals         = sqlite3_column_int( statement, 10 );
     team_batting_stats->errors         = sqlite3_column_int( statement, 11 );

     return SQLITE_OK;
}

int team_batting_stats_t_read( sqlite3 *db, team_batting_stats_s *team_batting_stats )
{
     static char query[]   = "SELECT Games, At_Bats, Runs, Hits, Doubles, Triples, Home_Runs, Runs_Batted_In, Walks, Strike_Outs, Steals, Errors "
          /**/               "FROM   Team_Batting_Stats_T "
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, team_batting_stats_t_read_bindings, team_batting_stats, team_batting_stats_t_read_retrieve, team_batting_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_batting_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_batting_stats_s *team_batting_stats = (team_batting_stats_s *)data;

     BIND_INT( statement,  1, team_batting_stats->games          );
     BIND_INT( statement,  2, team_batting_stats->at_bats        );
     BIND_INT( statement,  3, team_batting_stats->runs           );
     BIND_INT( statement,  4, team_batting_stats->hits           );
     BIND_INT( statement,  5, team_batting_stats->doubles        );
     BIND_INT( statement,  6, team_batting_stats->triples        );
     BIND_INT( statement,  7, team_batting_stats->home_runs      );
     BIND_INT( statement,  8, team_batting_stats->runs_batted_in );
     BIND_INT( statement,  9, team_batting_stats->walks          );
     BIND_INT( statement, 10, team_batting_stats->strike_outs    );
     BIND_INT( statement, 11, team_batting_stats->steals         );
     BIND_INT( statement, 12, team_batting_stats->errors         );

     BIND_INT( statement, 13, team_batting_stats->team_id      );
     BIND_INT( statement, 14, team_batting_stats->season       );
     BIND_INT( statement, 15, team_batting_stats->season_phase );

     return SQLITE_OK;
}


int team_batting_stats_t_update( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     static char query[]   = "UPDATE Team_Batting_Stats_T "
          /**/
          /**/               "SET    Games          = ?,"
          /**/                      "At_Bats        = ?,"
          /**/                      "Runs           = ?,"
          /**/                      "Hits           = ?,"
          /**/                      "Doubles        = ?,"
          /**/                      "Triples        = ?,"
          /**/                      "Home_Runs      = ?,"
          /**/                      "Runs_Batted_In = ?,"
          /**/                      "Walks          = ?,"
          /**/                      "Strike_Outs    = ?,"
          /**/                      "Steals         = ?,"
          /**/                      "Errors         = ? "
          /**/
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, team_batting_stats_t_update_bindings, team_batting_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_batting_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_batting_stats_s *team_batting_stats = (team_batting_stats_s *)data;

     BIND_INT( statement, 1, team_batting_stats->team_id      );
     BIND_INT( statement, 2, team_batting_stats->season       );
     BIND_INT( statement, 3, team_batting_stats->season_phase );

     return SQLITE_OK;
}


int team_batting_stats_t_delete( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     return execute_query( db, "DELETE FROM Team_Batting_Stats_T WHERE Team_Id = ? AND Season = ? AND Season_Phase = ?", team_batting_stats_t_delete_bindings, team_batting_stats, NULL, NULL );
}
