#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batter_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_accolade_s *batter_accolade = (batter_accolade_s *)data;

     BIND_INT(  statement,  1, batter_accolade->player_id );
     BIND_INT(  statement,  2, batter_accolade->season    );
     BIND_INT(  statement,  3, batter_accolade->accolade  );

     return SQLITE_OK;
}


int batter_accolades_t_create( sqlite3 *db, const batter_accolade_s *batter_accolade )
{
     static char query[]   = "INSERT INTO Batter_Accolades_T ( Player_Id, Season, Accolade ) "
          /**/                                        "VALUES ( ?,"       "?,"    "?"      ")";

     return execute_query( db, query, batter_accolades_t_create_bindings, batter_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int idx = 0;
static int max = 0;

static int batter_accolades_t_read_by_player_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int batter_accolades_t_read_by_player_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     batter_accolade_s batter_accolade = { 0 };

     batter_accolade.player_id = sqlite3_column_int(  statement, 0 );
     batter_accolade.season    = sqlite3_column_int(  statement, 1 );
     batter_accolade.accolade  = sqlite3_column_int(  statement, 2 );

     if ( add_to_data_list( data_list, &batter_accolade, sizeof(batter_accolade), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int batter_accolades_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *batter_accolades )
{
     static char query[] = "SELECT Player_Id, Season, Accolade FROM Batter_Accolades_T WHERE Player_Id = ?";

     idx = 0;
     max = 0;

     return execute_query( db, query, batter_accolades_t_read_by_player_bindings, &player_id, batter_accolades_t_read_by_player_retrieve, batter_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int batter_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const batter_accolade_s *batter_accolade = (batter_accolade_s *)data;

     BIND_INT(  statement,  1, batter_accolade->player_id );
     BIND_INT(  statement,  2, batter_accolade->season    );
     BIND_INT(  statement,  3, batter_accolade->accolade  );

     return SQLITE_OK;
}


int batter_accolades_t_delete( sqlite3 *db, const batter_accolade_s *batter_accolade )
{
     static char query[] = "DELETE FROM Batter_Accolades_T WHERE Player_Id = ? AND Season = ? AND Accolade = ?";

     return execute_query( db, query, batter_accolades_t_delete_bindings, batter_accolade, NULL, NULL );
}
