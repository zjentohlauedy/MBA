#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_stats_s *league_stats = (league_stats_s *)data;

     BIND_INT( statement,  1, league_stats->league_id    );
     BIND_INT( statement,  2, league_stats->season       );
     BIND_INT( statement,  3, league_stats->season_phase );
     BIND_INT( statement,  4, league_stats->wins         );
     BIND_INT( statement,  5, league_stats->losses       );
     BIND_INT( statement,  6, league_stats->home_wins    );
     BIND_INT( statement,  7, league_stats->home_losses  );
     BIND_INT( statement,  8, league_stats->road_wins    );
     BIND_INT( statement,  9, league_stats->road_losses  );
     BIND_INT( statement, 10, league_stats->runs_scored  );
     BIND_INT( statement, 11, league_stats->runs_allowed );

     return SQLITE_OK;
}


int league_stats_t_create( sqlite3 *db, const league_stats_s *league_stats )
{
     static char query[]   = "INSERT INTO League_Stats_T ( League_Id, Season, Season_Phase, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Runs_Scored, Runs_Allowed )"
          /**/                                   "VALUES ( ?,"       "?,"    "?,"          "?,"  "?,"    "?,"       "?,"         "?,"       "?,"         "?,"         "?"          ")";

     return execute_update_old( db, query, league_stats_t_create_bindings, league_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int league_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_stats_s *league_stats = (league_stats_s *)data;

     BIND_INT( statement,  1, league_stats->league_id    );
     BIND_INT( statement,  2, league_stats->season       );
     BIND_INT( statement,  3, league_stats->season_phase );

     return SQLITE_OK;
}

static int league_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     league_stats_s *league_stats = (league_stats_s *)data;

     league_stats->wins          = sqlite3_column_int( statement, 0 );
     league_stats->losses        = sqlite3_column_int( statement, 1 );
     league_stats->home_wins     = sqlite3_column_int( statement, 2 );
     league_stats->home_losses   = sqlite3_column_int( statement, 3 );
     league_stats->road_wins     = sqlite3_column_int( statement, 4 );
     league_stats->road_losses   = sqlite3_column_int( statement, 5 );
     league_stats->runs_scored   = sqlite3_column_int( statement, 6 );
     league_stats->runs_allowed  = sqlite3_column_int( statement, 7 );

     return SQLITE_OK;
}

int league_stats_t_read( sqlite3 *db, league_stats_s *league_stats )
{
     static char query[]   = "SELECT Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Runs_Scored, Runs_Allowed "
          /**/               "FROM   League_Stats_T "
          /**/               "WHERE  League_Id    = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, league_stats_t_read_bindings, league_stats, league_stats_t_read_retrieve, league_stats );
}

// By League:
static int league_stats_t_read_by_league_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *league_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *league_id );
}

static int league_stats_t_read_by_league_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     league_stats_s league_stats = { 0 };

     league_stats.league_id    = sqlite3_column_int( statement,  0 );
     league_stats.season       = sqlite3_column_int( statement,  1 );
     league_stats.season_phase = sqlite3_column_int( statement,  2 );
     league_stats.wins         = sqlite3_column_int( statement,  3 );
     league_stats.losses       = sqlite3_column_int( statement,  4 );
     league_stats.home_wins    = sqlite3_column_int( statement,  5 );
     league_stats.home_losses  = sqlite3_column_int( statement,  6 );
     league_stats.road_wins    = sqlite3_column_int( statement,  7 );
     league_stats.road_losses  = sqlite3_column_int( statement,  8 );
     league_stats.runs_scored  = sqlite3_column_int( statement,  9 );
     league_stats.runs_allowed = sqlite3_column_int( statement, 10 );

     if ( add_to_data_list( data_list, &league_stats, sizeof(league_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int league_stats_t_read_by_league( sqlite3 *db, const int league_id, data_list_s *league_stats )
{
     static char query[]   = "SELECT League_Id, Season, Season_Phase, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Runs_Scored, Runs_Allowed FROM League_Stats_T WHERE League_Id = ? ";

     return execute_query( db, query, league_stats_t_read_by_league_bindings, &league_id, league_stats_t_read_by_league_retrieve, league_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_stats_s *league_stats = (league_stats_s *)data;

     BIND_INT(  statement,  1, league_stats->wins         );
     BIND_INT(  statement,  2, league_stats->losses       );
     BIND_INT(  statement,  3, league_stats->home_wins    );
     BIND_INT(  statement,  4, league_stats->home_losses  );
     BIND_INT(  statement,  5, league_stats->road_wins    );
     BIND_INT(  statement,  6, league_stats->road_losses  );
     BIND_INT(  statement,  7, league_stats->runs_scored  );
     BIND_INT(  statement,  8, league_stats->runs_allowed );

     BIND_INT(  statement,  9, league_stats->league_id    );
     BIND_INT(  statement, 10, league_stats->season       );
     BIND_INT(  statement, 11, league_stats->season_phase );

     return SQLITE_OK;
}


int league_stats_t_update( sqlite3 *db, const league_stats_s *league_stats )
{
     static char query[]   = "UPDATE League_Stats_T "
          /**/
          /**/               "SET    Wins            = ?,"
          /**/                      "Losses          = ?,"
          /**/                      "Home_Wins       = ?,"
          /**/                      "Home_Losses     = ?,"
          /**/                      "Road_Wins       = ?,"
          /**/                      "Road_Losses     = ?,"
          /**/                      "Runs_Scored     = ?,"
          /**/                      "Runs_Allowed    = ? "
          /**/
          /**/               "WHERE  League_Id    = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_update_old( db, query, league_stats_t_update_bindings, league_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_stats_s *league_stats = (league_stats_s *)data;

     BIND_INT( statement,  1, league_stats->league_id    );
     BIND_INT( statement,  2, league_stats->season       );
     BIND_INT( statement,  3, league_stats->season_phase );

     return SQLITE_OK;
}


int league_stats_t_delete( sqlite3 *db, const league_stats_s *league_stats )
{
     return execute_update_old( db, "DELETE FROM League_Stats_T WHERE League_Id = ? AND Season = ? AND Season_Phase = ?", league_stats_t_delete_bindings, league_stats, NULL, NULL );
}
