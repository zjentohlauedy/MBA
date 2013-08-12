#include <stdio.h>
#include <stdlib.h>
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

static int idx = 0;
static int max = 0;

static int get_player_accolades_by_player_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int get_player_accolades_by_player_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_accolade_s **player_accolades = (player_accolade_s **)data;

     if ( idx == max )
     {
          player_accolade_s *tmp = NULL;

          max += 10;

          tmp = realloc( *player_accolades, sizeof(player_accolade_s) * (max + 1) );

          if ( tmp == NULL )
          {
               free( *player_accolades );

               return SQLITE_ERROR;
          }

          memset( tmp + idx, '\0', sizeof(player_accolade_s) * (max + 1 - idx) );

          *player_accolades = tmp;
     }

     (*player_accolades)[idx].player_id = sqlite3_column_int(  statement, 0 );
     (*player_accolades)[idx].season    = sqlite3_column_int(  statement, 1 );
     (*player_accolades)[idx].accolade  = sqlite3_column_int(  statement, 2 );

     idx++;

     return SQLITE_OK;
}

int get_player_accolades_by_player( sqlite3 *db, const int player_id, player_accolade_s **player_accolades )
{
     static char query[]   = "SELECT Player_Id, Season, Accolade FROM Player_Accolades_T WHERE Player_Id = ?";

     idx = 0;
     max = 0;

     return execute_query( db, query, get_player_accolades_by_player_bindings, &player_id, get_player_accolades_by_player_retrieve, player_accolades );
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
