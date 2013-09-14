#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitchers_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const pitcher_s *pitcher = (pitcher_s *)data;

     BIND_INT(  statement,  1, pitcher->player_id );
     BIND_INT(  statement,  2, pitcher->speed     );
     BIND_INT(  statement,  3, pitcher->control   );
     BIND_INT(  statement,  4, pitcher->bunt      );
     BIND_INT(  statement,  5, pitcher->fatigue   );

     return SQLITE_OK;
}


int pitchers_t_create( sqlite3 *db, const pitcher_s *pitcher )
{
     static char query[]   = "INSERT INTO Pitchers_T ( Player_Id, Speed, Control, Bunt, Fatigue )"
          /**/                               "VALUES ( ?,"       "?,"   "?,"     "?,"  "?"     ")";

     return execute_update_old( db, query, pitchers_t_create_bindings, pitcher, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitchers_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const pitcher_s *pitcher = (pitcher_s *)data;

     return sqlite3_bind_int( statement, 1, pitcher->player_id );
}

static int pitchers_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     pitcher_s *pitcher = (pitcher_s *)data;

     /**/    pitcher->speed   = sqlite3_column_int(  statement, 0 );
     /**/    pitcher->control = sqlite3_column_int(  statement, 1 );
     /**/    pitcher->bunt    = sqlite3_column_int(  statement, 2 );
     /**/    pitcher->fatigue = sqlite3_column_int(  statement, 3 );

     return SQLITE_OK;
}

int pitchers_t_read( sqlite3 *db, pitcher_s *pitcher )
{
     static char query[]   = "SELECT Speed, Control, Bunt, Fatigue FROM Pitchers_T WHERE Player_Id = ?";

     return execute_query( db, query, pitchers_t_read_bindings, pitcher, pitchers_t_read_retrieve, pitcher );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitchers_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const pitcher_s *pitcher = (pitcher_s *)data;

     BIND_INT(  statement,  1, pitcher->speed     );
     BIND_INT(  statement,  2, pitcher->control   );
     BIND_INT(  statement,  3, pitcher->bunt      );
     BIND_INT(  statement,  4, pitcher->fatigue   );
     BIND_INT(  statement,  5, pitcher->player_id );

     return SQLITE_OK;
}


int pitchers_t_update( sqlite3 *db, const pitcher_s *pitcher )
{
     static char query[]   = "UPDATE Pitchers_T "
          /**/
          /**/               "SET    Speed   = ?,"
          /**/                      "Control = ?,"
          /**/                      "Bunt    = ?,"
          /**/                      "Fatigue = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, pitchers_t_update_bindings, pitcher, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitchers_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const pitcher_s *pitcher = (pitcher_s *)data;

     return sqlite3_bind_int( statement, 1, pitcher->player_id );
}


int pitchers_t_delete( sqlite3 *db, const pitcher_s *pitcher )
{
     return execute_update_old( db, "DELETE FROM Pitchers_T WHERE Player_Id = ?", pitchers_t_delete_bindings, pitcher, NULL, NULL );
}
