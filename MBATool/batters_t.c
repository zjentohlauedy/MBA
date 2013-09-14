#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batters_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_s *batter = (batter_s *)data;

     BIND_INT( statement, 1, batter->player_id          );
     BIND_INT( statement, 2, batter->power              );
     BIND_INT( statement, 3, batter->hit_n_run          );
     BIND_INT( statement, 4, batter->bunt               );
     BIND_INT( statement, 5, batter->running            );
     BIND_INT( statement, 6, batter->range              );
     BIND_INT( statement, 7, batter->arm                );
     BIND_INT( statement, 8, batter->primary_position   );
     BIND_INT( statement, 9, batter->secondary_position );

     return SQLITE_OK;
}


int batters_t_create( sqlite3 *db, const batter_s *batter )
{
     static char query[]   = "INSERT INTO Batters_T ( Player_Id, Power, Hit_N_Run, Bunt, Running, Range, Arm, Primary_Position, Secondary_Position )"
          /**/                              "VALUES ( ?,"       "?,"   "?,"       "?,"  "?,"     "?,"   "?," "?,"              "?"                ")";

     return execute_update_old( db, query, batters_t_create_bindings, batter, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batters_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const batter_s *batter = (batter_s *)data;

     return sqlite3_bind_int( statement, 1, batter->player_id );
}

static int batters_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     batter_s *batter = (batter_s *)data;

     batter->power              = sqlite3_column_int( statement, 0 );
     batter->hit_n_run          = sqlite3_column_int( statement, 1 );
     batter->bunt               = sqlite3_column_int( statement, 2 );
     batter->running            = sqlite3_column_int( statement, 3 );
     batter->range              = sqlite3_column_int( statement, 4 );
     batter->arm                = sqlite3_column_int( statement, 5 );
     batter->primary_position   = sqlite3_column_int( statement, 6 );
     batter->secondary_position = sqlite3_column_int( statement, 7 );

     return SQLITE_OK;
}

int batters_t_read( sqlite3 *db, batter_s *batter )
{
     static char query[]   = "SELECT Power, Hit_N_Run, Bunt, Running, Range, Arm, Primary_Position, Secondary_Position FROM Batters_T WHERE Player_Id = ?";

     return execute_query( db, query, batters_t_read_bindings, batter, batters_t_read_retrieve, batter );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batters_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_s *batter = (batter_s *)data;

     BIND_INT( statement, 1, batter->power              );
     BIND_INT( statement, 2, batter->hit_n_run          );
     BIND_INT( statement, 3, batter->bunt               );
     BIND_INT( statement, 4, batter->running            );
     BIND_INT( statement, 5, batter->range              );
     BIND_INT( statement, 6, batter->arm                );
     BIND_INT( statement, 7, batter->primary_position   );
     BIND_INT( statement, 8, batter->secondary_position );
     BIND_INT( statement, 9, batter->player_id          );

     return SQLITE_OK;
}


int batters_t_update( sqlite3 *db, const batter_s *batter )
{
     static char query[]   = "UPDATE Batters_T "
          /**/
          /**/               "SET    Power              = ?,"
          /**/                      "Hit_N_Run          = ?,"
          /**/                      "Bunt               = ?,"
          /**/                      "Running            = ?,"
          /**/                      "Range              = ?,"
          /**/                      "Arm                = ?,"
          /**/                      "Primary_Position   = ?,"
          /**/                      "Secondary_Position = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, batters_t_update_bindings, batter, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batters_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const batter_s *batter = (batter_s *)data;

     return sqlite3_bind_int( statement, 1, batter->player_id );
}


int batters_t_delete( sqlite3 *db, const batter_s *batter )
{
     return execute_update_old( db, "DELETE FROM Batters_T WHERE Player_Id = ?", batters_t_delete_bindings, batter, NULL, NULL );
}
