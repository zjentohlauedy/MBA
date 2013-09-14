#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_accolade_s *player_accolade = (player_accolade_s *)data;

     BIND_INT(  statement,  1, player_accolade->player_id );
     BIND_INT(  statement,  2, player_accolade->season    );
     BIND_INT(  statement,  3, player_accolade->accolade  );

     return SQLITE_OK;
}


int player_accolades_t_create( sqlite3 *db, const player_accolade_s *player_accolade )
{
     static char query[]   = "INSERT INTO Player_Accolades_T ( Player_Id, Season, Accolade ) "
          /**/                                       "VALUES ( ?,"       "?,"    "?"      ")";

     return execute_update_old( db, query, player_accolades_t_create_bindings, player_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_accolades_t_read_by_player_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int player_accolades_t_read_by_player_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     player_accolade_s player_accolade = { 0 };

     player_accolade.player_id = sqlite3_column_int(  statement, 0 );
     player_accolade.season    = sqlite3_column_int(  statement, 1 );
     player_accolade.accolade  = sqlite3_column_int(  statement, 2 );

     if ( add_to_data_list( data_list, &player_accolade, sizeof(player_accolade_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int player_accolades_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *player_accolades )
{
     static char query[] = "SELECT Player_Id, Season, Accolade FROM Player_Accolades_T WHERE Player_Id = ?";

     return execute_query( db, query, player_accolades_t_read_by_player_bindings, &player_id, player_accolades_t_read_by_player_retrieve, player_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_accolade_s *player_accolade = (player_accolade_s *)data;

     BIND_INT(  statement,  1, player_accolade->player_id );
     BIND_INT(  statement,  2, player_accolade->season    );
     BIND_INT(  statement,  3, player_accolade->accolade  );

     return SQLITE_OK;
}


int player_accolades_t_delete( sqlite3 *db, const player_accolade_s *player_accolade )
{
     static char query[] = "DELETE FROM Player_Accolades_T WHERE Player_Id = ? AND Season = ? AND Accolade = ?";

     return execute_update_old( db, query, player_accolades_t_delete_bindings, player_accolade, NULL, NULL );
}
