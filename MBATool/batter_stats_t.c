#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batter_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_stats_s *batter_stats = (batter_stats_s *)data;

     BIND_INT( statement,  1, batter_stats->player_id      );
     BIND_INT( statement,  2, batter_stats->season         );
     BIND_INT( statement,  3, batter_stats->season_phase   );
     BIND_INT( statement,  4, batter_stats->games          );
     BIND_INT( statement,  5, batter_stats->at_bats        );
     BIND_INT( statement,  6, batter_stats->runs           );
     BIND_INT( statement,  7, batter_stats->hits           );
     BIND_INT( statement,  8, batter_stats->doubles        );
     BIND_INT( statement,  9, batter_stats->triples        );
     BIND_INT( statement, 10, batter_stats->home_runs      );
     BIND_INT( statement, 11, batter_stats->runs_batted_in );
     BIND_INT( statement, 12, batter_stats->walks          );
     BIND_INT( statement, 13, batter_stats->strike_outs    );
     BIND_INT( statement, 14, batter_stats->steals         );
     BIND_INT( statement, 15, batter_stats->errors         );

     return SQLITE_OK;
}


int batter_stats_t_create( sqlite3 *db, const batter_stats_s *batter_stats )
{
     static char query[]   = "INSERT INTO Batter_Stats_T ( Player_Id, Season, Season_Phase, Games, At_Bats, Runs, Hits, Doubles, Triples, Home_Runs, Runs_Batted_In, Walks, Strike_Outs, Steals, Errors )"
          /**/                                   "VALUES ( ?,"       "?,"    "?,"          "?,"   "?,"     "?,"  "?,"  "?,"     "?,"     "?,"       "?,"            "?,"   "?,"         "?,"    "?"    ")";

     return execute_query( db, query, batter_stats_t_create_bindings, batter_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int batter_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_stats_s *batter_stats = (batter_stats_s *)data;

     BIND_INT( statement,  1, batter_stats->player_id    );
     BIND_INT( statement,  2, batter_stats->season       );
     BIND_INT( statement,  3, batter_stats->season_phase );

     return SQLITE_OK;
}

static int batter_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     batter_stats_s *batter_stats = (batter_stats_s *)data;

     batter_stats->games          = sqlite3_column_int( statement,  0 );
     batter_stats->at_bats        = sqlite3_column_int( statement,  1 );
     batter_stats->runs           = sqlite3_column_int( statement,  2 );
     batter_stats->hits           = sqlite3_column_int( statement,  3 );
     batter_stats->doubles        = sqlite3_column_int( statement,  4 );
     batter_stats->triples        = sqlite3_column_int( statement,  5 );
     batter_stats->home_runs      = sqlite3_column_int( statement,  6 );
     batter_stats->runs_batted_in = sqlite3_column_int( statement,  7 );
     batter_stats->walks          = sqlite3_column_int( statement,  8 );
     batter_stats->strike_outs    = sqlite3_column_int( statement,  9 );
     batter_stats->steals         = sqlite3_column_int( statement, 10 );
     batter_stats->errors         = sqlite3_column_int( statement, 11 );

     return SQLITE_OK;
}

int batter_stats_t_read( sqlite3 *db, batter_stats_s *batter_stats )
{
     static char query[]   = "SELECT Games, At_Bats, Runs, Hits, Doubles, Triples, Home_Runs, Runs_Batted_In, Walks, Strike_Outs, Steals, Errors "
          /**/               "FROM   Batter_Stats_T "
          /**/               "WHERE  Player_Id    = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, batter_stats_t_read_bindings, batter_stats, batter_stats_t_read_retrieve, batter_stats );
}

// By Player:
static int batter_stats_t_read_by_player_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int batter_stats_t_read_by_player_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     batter_stats_s batter_stats = { 0 };

     batter_stats.player_id      = sqlite3_column_int( statement,  0 );
     batter_stats.season         = sqlite3_column_int( statement,  1 );
     batter_stats.season_phase   = sqlite3_column_int( statement,  2 );
     batter_stats.games          = sqlite3_column_int( statement,  3 );
     batter_stats.at_bats        = sqlite3_column_int( statement,  4 );
     batter_stats.runs           = sqlite3_column_int( statement,  5 );
     batter_stats.hits           = sqlite3_column_int( statement,  6 );
     batter_stats.doubles        = sqlite3_column_int( statement,  7 );
     batter_stats.triples        = sqlite3_column_int( statement,  8 );
     batter_stats.home_runs      = sqlite3_column_int( statement,  9 );
     batter_stats.runs_batted_in = sqlite3_column_int( statement, 10 );
     batter_stats.walks          = sqlite3_column_int( statement, 11 );
     batter_stats.strike_outs    = sqlite3_column_int( statement, 12 );
     batter_stats.steals         = sqlite3_column_int( statement, 13 );
     batter_stats.errors         = sqlite3_column_int( statement, 14 );

     if ( add_to_data_list( data_list, &batter_stats, sizeof(batter_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int batter_stats_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *batter_stats )
{
     static char query[] = "SELECT Player_Id, Season, Season_Phase, Games, At_Bats, Runs, Hits, Doubles, Triples, Home_Runs, Runs_Batted_In, Walks, Strike_Outs, Steals, Errors FROM Batter_Stats_T WHERE Player_Id = ?";

     return execute_query( db, query, batter_stats_t_read_by_player_bindings, &player_id, batter_stats_t_read_by_player_retrieve, batter_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batter_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_stats_s *batter_stats = (batter_stats_s *)data;

     BIND_INT( statement,  1, batter_stats->games          );
     BIND_INT( statement,  2, batter_stats->at_bats        );
     BIND_INT( statement,  3, batter_stats->runs           );
     BIND_INT( statement,  4, batter_stats->hits           );
     BIND_INT( statement,  5, batter_stats->doubles        );
     BIND_INT( statement,  6, batter_stats->triples        );
     BIND_INT( statement,  7, batter_stats->home_runs      );
     BIND_INT( statement,  8, batter_stats->runs_batted_in );
     BIND_INT( statement,  9, batter_stats->walks          );
     BIND_INT( statement, 10, batter_stats->strike_outs    );
     BIND_INT( statement, 11, batter_stats->steals         );
     BIND_INT( statement, 12, batter_stats->errors         );

     BIND_INT( statement, 13, batter_stats->player_id    );
     BIND_INT( statement, 14, batter_stats->season       );
     BIND_INT( statement, 15, batter_stats->season_phase );

     return SQLITE_OK;
}


int batter_stats_t_update( sqlite3 *db, const batter_stats_s *batter_stats )
{
     static char query[]   = "UPDATE Batter_Stats_T "
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
          /**/               "WHERE  Player_Id    = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Season_Phase = ? ";

     return execute_query( db, query, batter_stats_t_update_bindings, batter_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batter_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_stats_s *batter_stats = (batter_stats_s *)data;

     BIND_INT( statement, 1, batter_stats->player_id    );
     BIND_INT( statement, 2, batter_stats->season       );
     BIND_INT( statement, 3, batter_stats->season_phase );

     return SQLITE_OK;
}


int batter_stats_t_delete( sqlite3 *db, const batter_stats_s *batter_stats )
{
     return execute_query( db, "DELETE FROM Batter_Stats_T WHERE Player_Id = ? AND Season = ? AND Season_Phase = ?", batter_stats_t_delete_bindings, batter_stats, NULL, NULL );
}
