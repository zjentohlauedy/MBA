#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitcher_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const pitcher_stats_s *pitcher_stats = (pitcher_stats_s *)data;

     BIND_INT( statement,  1, pitcher_stats->player_id    );
     BIND_INT( statement,  2, pitcher_stats->season       );
     BIND_INT( statement,  3, pitcher_stats->season_phase );
     BIND_INT( statement,  4, pitcher_stats->wins         );
     BIND_INT( statement,  5, pitcher_stats->losses       );
     BIND_INT( statement,  6, pitcher_stats->games        );
     BIND_INT( statement,  7, pitcher_stats->saves        );
     BIND_DBL( statement,  8, pitcher_stats->innings      );
     BIND_INT( statement,  9, pitcher_stats->hits         );
     BIND_INT( statement, 10, pitcher_stats->earned_runs  );
     BIND_INT( statement, 11, pitcher_stats->home_runs    );
     BIND_INT( statement, 12, pitcher_stats->walks        );
     BIND_INT( statement, 13, pitcher_stats->strike_outs  );

     return SQLITE_OK;
}


int pitcher_stats_t_create( sqlite3 *db, const pitcher_stats_s *pitcher_stats )
{
     static char query[]   = "INSERT INTO Pitcher_Stats_T ( Player_Id, Season, Season_Phase, Wins, Losses, Games, Saves, Innings, Hits, Earned_Runs, Home_Runs, Walks, Strike_Outs )"
          /**/                                    "VALUES ( ?,"       "?,"    "?,"          "?,"  "?,"    "?,"   "?,"   "?,"     "?,"  "?,"         "?,"       "?,"   "?"         ")";

     return execute_query( db, query, pitcher_stats_t_create_bindings, pitcher_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitcher_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const pitcher_stats_s *pitcher_stats = (pitcher_stats_s *)data;

     BIND_INT( statement,  1, pitcher_stats->player_id    );
     BIND_INT( statement,  2, pitcher_stats->season       );
     BIND_INT( statement,  3, pitcher_stats->season_phase );
}

static int pitcher_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     pitcher_stats_s *pitcher_stats = (pitcher_stats_s *)data;

     /**/    pitcher_stats->wins        = sqlite3_column_int(    statement, 0 );
     /**/    pitcher_stats->losses      = sqlite3_column_int(    statement, 1 );
     /**/    pitcher_stats->games       = sqlite3_column_int(    statement, 2 );
     /**/    pitcher_stats->saves       = sqlite3_column_int(    statement, 3 );
     /**/    pitcher_stats->innings     = sqlite3_column_double( statement, 4 );
     /**/    pitcher_stats->hits        = sqlite3_column_int(    statement, 5 );
     /**/    pitcher_stats->earned_runs = sqlite3_column_int(    statement, 6 );
     /**/    pitcher_stats->home_runs   = sqlite3_column_int(    statement, 7 );
     /**/    pitcher_stats->walks       = sqlite3_column_int(    statement, 8 );
     /**/    pitcher_stats->strike_outs = sqlite3_column_int(    statement, 9 );

     return SQLITE_OK;
}

int pitcher_stats_t_read( sqlite3 *db, pitcher_stats_s *pitcher_stats )
{
     static char query[]   = "SELECT Wins, Losses, Games, Saves, Innings, Hits, Earned_Runs, Home_Runs, Walks, Strike_Outs "
          /**/               "FROM   Pitcher_Stats_T "
          /**/               "WHERE  Player_Id    = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, pitcher_stats_t_read_bindings, pitcher_stats, pitcher_stats_t_read_retrieve, pitcher_stats );
}
