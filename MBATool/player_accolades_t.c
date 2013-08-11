#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int insert_player_accolade_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_accolade_s *player_accolade = (player_accolade_s *)data;

     BIND_INT(  statement,  1, player_accolade->player_id );
     BIND_INT(  statement,  2, player_accolade->season    );
     BIND_INT(  statement,  3, player_accolade->accolade  );

     return SQLITE_OK;
}


int insert_player_accolade( sqlite3 *db, const player_accolade_s *player_accolade )
{
     static char query[]   = "INSERT INTO Player_Accolades_T ( Player_Id, Season, Accolade ) "
          /**/                                       "VALUES ( ?,"       "?,"    "?"      ")";

     return execute_query( db, query, insert_player_accolade_bindings, player_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int get_player_accolade_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_accolade_s *player_accolade = (player_accolade_s *)data;

     return sqlite3_bind_int( statement, 1, player_accolade->player_id );
}

static int get_player_accolade_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_accolade_s *player_accolade = (player_accolade_s *)data;

     player_accolade->season   = sqlite3_column_int(  statement, 0 );
     player_accolade->accolade = sqlite3_column_int(  statement, 1 );

     return SQLITE_OK;
}

int get_player_accolade( sqlite3 *db, player_accolade_s *player_accolade )
{
     static char query[]   = "SELECT Season, Accolade FROM Player_Accolades_T WHERE Player_Id = ?";

     return execute_query( db, query, get_player_accolade_bindings, player_accolade, get_player_accolade_retrieve, player_accolade );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int update_player_accolade_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_accolade_s *player_accolade = (player_accolade_s *)data;

     BIND_INT( statement, 1, player_accolade->season    );
     BIND_INT( statement, 2, player_accolade->accolade  );
     BIND_INT( statement, 3, player_accolade->player_id );

     return SQLITE_OK;
}


int update_player_accolade( sqlite3 *db, const player_accolade_s *player_accolade )
{
     static char query[]   = "UPDATE Player_Accolades_T "
          /**/
          /**/               "SET    Season   = ?,"
          /**/                      "Accolade = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_query( db, query, update_player_accolade_bindings, player_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int delete_player_accolade_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_accolade_s *player_accolade = (player_accolade_s *)data;

     return sqlite3_bind_int( statement, 1, player_accolade->player_id );
}


int delete_player_accolade( sqlite3 *db, const player_accolade_s *player_accolade )
{
     return execute_query( db, "DELETE FROM Player_Accolades_T WHERE Player_Id = ?", delete_player_accolade_bindings, player_accolade, NULL, NULL );
}
