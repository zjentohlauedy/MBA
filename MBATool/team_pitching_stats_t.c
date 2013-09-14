#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_pitching_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_pitching_stats_s *team_pitching_stats = (team_pitching_stats_s *)data;

     BIND_INT( statement,  1, team_pitching_stats->team_id      );
     BIND_INT( statement,  2, team_pitching_stats->season       );
     BIND_INT( statement,  3, team_pitching_stats->season_phase );
     BIND_INT( statement,  4, team_pitching_stats->wins         );
     BIND_INT( statement,  5, team_pitching_stats->losses       );
     BIND_INT( statement,  6, team_pitching_stats->games        );
     BIND_INT( statement,  7, team_pitching_stats->saves        );
     BIND_DBL( statement,  8, team_pitching_stats->innings      );
     BIND_INT( statement,  9, team_pitching_stats->hits         );
     BIND_INT( statement, 10, team_pitching_stats->earned_runs  );
     BIND_INT( statement, 11, team_pitching_stats->home_runs    );
     BIND_INT( statement, 12, team_pitching_stats->walks        );
     BIND_INT( statement, 13, team_pitching_stats->strike_outs  );

     return SQLITE_OK;
}


int team_pitching_stats_t_create( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     static char query[]   = "INSERT INTO Team_Pitching_Stats_T ( Team_Id, Season, Season_Phase, Wins, Losses, Games, Saves, Innings, Hits, Earned_Runs, Home_Runs, Walks, Strike_Outs )"
          /**/                                          "VALUES ( ?,"     "?,"    "?,"          "?,"  "?,"    "?,"   "?,"   "?,"     "?,"  "?,"         "?,"       "?,"   "?"         ")";

     return execute_update_old( db, query, team_pitching_stats_t_create_bindings, team_pitching_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_pitching_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_pitching_stats_s *team_pitching_stats = (team_pitching_stats_s *)data;

     BIND_INT( statement,  1, team_pitching_stats->team_id      );
     BIND_INT( statement,  2, team_pitching_stats->season       );
     BIND_INT( statement,  3, team_pitching_stats->season_phase );

     return SQLITE_OK;
}

static int team_pitching_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_pitching_stats_s *team_pitching_stats = (team_pitching_stats_s *)data;

     team_pitching_stats->wins        = sqlite3_column_int(    statement, 0 );
     team_pitching_stats->losses      = sqlite3_column_int(    statement, 1 );
     team_pitching_stats->games       = sqlite3_column_int(    statement, 2 );
     team_pitching_stats->saves       = sqlite3_column_int(    statement, 3 );
     team_pitching_stats->innings     = sqlite3_column_double( statement, 4 );
     team_pitching_stats->hits        = sqlite3_column_int(    statement, 5 );
     team_pitching_stats->earned_runs = sqlite3_column_int(    statement, 6 );
     team_pitching_stats->home_runs   = sqlite3_column_int(    statement, 7 );
     team_pitching_stats->walks       = sqlite3_column_int(    statement, 8 );
     team_pitching_stats->strike_outs = sqlite3_column_int(    statement, 9 );

     return SQLITE_OK;
}

int team_pitching_stats_t_read( sqlite3 *db, team_pitching_stats_s *team_pitching_stats )
{
     static char query[]   = "SELECT Wins, Losses, Games, Saves, Innings, Hits, Earned_Runs, Home_Runs, Walks, Strike_Outs "
          /**/               "FROM   Team_Pitching_Stats_T "
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, team_pitching_stats_t_read_bindings, team_pitching_stats, team_pitching_stats_t_read_retrieve, team_pitching_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_pitching_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_pitching_stats_s *team_pitching_stats = (team_pitching_stats_s *)data;

     BIND_INT(  statement,  1, team_pitching_stats->wins        );
     BIND_INT(  statement,  2, team_pitching_stats->losses      );
     BIND_INT(  statement,  3, team_pitching_stats->games       );
     BIND_INT(  statement,  4, team_pitching_stats->saves       );
     BIND_DBL(  statement,  5, team_pitching_stats->innings     );
     BIND_INT(  statement,  6, team_pitching_stats->hits        );
     BIND_INT(  statement,  7, team_pitching_stats->earned_runs );
     BIND_INT(  statement,  8, team_pitching_stats->home_runs   );
     BIND_INT(  statement,  9, team_pitching_stats->walks       );
     BIND_INT(  statement, 10, team_pitching_stats->strike_outs );

     BIND_INT(  statement, 11, team_pitching_stats->team_id      );
     BIND_INT(  statement, 12, team_pitching_stats->season       );
     BIND_INT(  statement, 13, team_pitching_stats->season_phase );

     return SQLITE_OK;
}


int team_pitching_stats_t_update( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     static char query[]   = "UPDATE Team_Pitching_Stats_T "
          /**/
          /**/               "SET    Wins        = ?,"
          /**/                      "Losses      = ?,"
          /**/                      "Games       = ?,"
          /**/                      "Saves       = ?,"
          /**/                      "Innings     = ?,"
          /**/                      "Hits        = ?,"
          /**/                      "Earned_Runs = ?,"
          /**/                      "Home_Runs   = ?,"
          /**/                      "Walks       = ?,"
          /**/                      "Strike_Outs = ? "
          /**/
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_update_old( db, query, team_pitching_stats_t_update_bindings, team_pitching_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_pitching_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_pitching_stats_s *team_pitching_stats = (team_pitching_stats_s *)data;

     BIND_INT( statement,  1, team_pitching_stats->team_id      );
     BIND_INT( statement,  2, team_pitching_stats->season       );
     BIND_INT( statement,  3, team_pitching_stats->season_phase );

     return SQLITE_OK;
}


int team_pitching_stats_t_delete( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Pitching_Stats_T WHERE Team_Id = ? AND Season = ? AND Season_Phase = ?", team_pitching_stats_t_delete_bindings, team_pitching_stats, NULL, NULL );
}
