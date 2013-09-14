#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_stats_s *division_stats = (division_stats_s *)data;

     BIND_INT( statement,  1, division_stats->division_id         );
     BIND_INT( statement,  2, division_stats->season          );
     BIND_INT( statement,  3, division_stats->season_phase    );
     BIND_INT( statement,  4, division_stats->wins            );
     BIND_INT( statement,  5, division_stats->losses          );
     BIND_INT( statement,  6, division_stats->home_wins       );
     BIND_INT( statement,  7, division_stats->home_losses     );
     BIND_INT( statement,  8, division_stats->road_wins       );
     BIND_INT( statement,  9, division_stats->road_losses     );
     BIND_INT( statement, 10, division_stats->league_wins     );
     BIND_INT( statement, 11, division_stats->league_losses   );
     BIND_INT( statement, 12, division_stats->runs_scored     );
     BIND_INT( statement, 13, division_stats->runs_allowed    );

     return SQLITE_OK;
}


int division_stats_t_create( sqlite3 *db, const division_stats_s *division_stats )
{
     static char query[]   = "INSERT INTO Division_Stats_T ( Division_Id, Season, Season_Phase, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, League_Wins, League_Losses, Runs_Scored, Runs_Allowed )"
          /**/                                     "VALUES ( ?,"         "?,"    "?,"          "?,"  "?,"    "?,"       "?,"         "?,"       "?,"         "?,"         "?,"           "?,"         "?"          ")";

     return execute_update_old( db, query, division_stats_t_create_bindings, division_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_stats_s *division_stats = (division_stats_s *)data;

     BIND_INT( statement,  1, division_stats->division_id  );
     BIND_INT( statement,  2, division_stats->season       );
     BIND_INT( statement,  3, division_stats->season_phase );

     return SQLITE_OK;
}

static int division_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     division_stats_s *division_stats = (division_stats_s *)data;

     division_stats->wins          = sqlite3_column_int( statement, 0 );
     division_stats->losses        = sqlite3_column_int( statement, 1 );
     division_stats->home_wins     = sqlite3_column_int( statement, 2 );
     division_stats->home_losses   = sqlite3_column_int( statement, 3 );
     division_stats->road_wins     = sqlite3_column_int( statement, 4 );
     division_stats->road_losses   = sqlite3_column_int( statement, 5 );
     division_stats->league_wins   = sqlite3_column_int( statement, 6 );
     division_stats->league_losses = sqlite3_column_int( statement, 7 );
     division_stats->runs_scored   = sqlite3_column_int( statement, 8 );
     division_stats->runs_allowed  = sqlite3_column_int( statement, 9 );

     return SQLITE_OK;
}

int division_stats_t_read( sqlite3 *db, division_stats_s *division_stats )
{
     static char query[]   = "SELECT Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, League_Wins, League_Losses, Runs_Scored, Runs_Allowed "
          /**/               "FROM   Division_Stats_T "
          /**/               "WHERE  Division_Id  = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, division_stats_t_read_bindings, division_stats, division_stats_t_read_retrieve, division_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_stats_s *division_stats = (division_stats_s *)data;

     BIND_INT(  statement,  1, division_stats->wins            );
     BIND_INT(  statement,  2, division_stats->losses          );
     BIND_INT(  statement,  3, division_stats->home_wins       );
     BIND_INT(  statement,  4, division_stats->home_losses     );
     BIND_INT(  statement,  5, division_stats->road_wins       );
     BIND_INT(  statement,  6, division_stats->road_losses     );
     BIND_INT(  statement,  7, division_stats->league_wins     );
     BIND_INT(  statement,  8, division_stats->league_losses   );
     BIND_INT(  statement,  9, division_stats->runs_scored     );
     BIND_INT(  statement, 10, division_stats->runs_allowed    );

     BIND_INT(  statement, 11, division_stats->division_id  );
     BIND_INT(  statement, 12, division_stats->season       );
     BIND_INT(  statement, 13, division_stats->season_phase );

     return SQLITE_OK;
}


int division_stats_t_update( sqlite3 *db, const division_stats_s *division_stats )
{
     static char query[]   = "UPDATE Division_Stats_T "
          /**/
          /**/               "SET    Wins            = ?,"
          /**/                      "Losses          = ?,"
          /**/                      "Home_Wins       = ?,"
          /**/                      "Home_Losses     = ?,"
          /**/                      "Road_Wins       = ?,"
          /**/                      "Road_Losses     = ?,"
          /**/                      "League_Wins     = ?,"
          /**/                      "League_Losses   = ?,"
          /**/                      "Runs_Scored     = ?,"
          /**/                      "Runs_Allowed    = ? "
          /**/
          /**/               "WHERE  Division_Id  = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_update_old( db, query, division_stats_t_update_bindings, division_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_stats_s *division_stats = (division_stats_s *)data;

     BIND_INT( statement,  1, division_stats->division_id  );
     BIND_INT( statement,  2, division_stats->season       );
     BIND_INT( statement,  3, division_stats->season_phase );

     return SQLITE_OK;
}


int division_stats_t_delete( sqlite3 *db, const division_stats_s *division_stats )
{
     return execute_update_old( db, "DELETE FROM Division_Stats_T WHERE Division_Id = ? AND Season = ? AND Season_Phase = ?", division_stats_t_delete_bindings, division_stats, NULL, NULL );
}
