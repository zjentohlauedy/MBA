#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitcher_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const pitcher_accolade_s *pitcher_accolade = (pitcher_accolade_s *)data;

     BIND_INT(  statement,  1, pitcher_accolade->player_id );
     BIND_INT(  statement,  2, pitcher_accolade->season    );
     BIND_INT(  statement,  3, pitcher_accolade->accolade  );

     return SQLITE_OK;
}


int pitcher_accolades_t_create( sqlite3 *db, const pitcher_accolade_s *pitcher_accolade )
{
     static char query[]   = "INSERT INTO Pitcher_Accolades_T ( Player_Id, Season, Accolade ) "
          /**/                                        "VALUES ( ?,"       "?,"    "?"      ")";

     return execute_update_old( db, query, pitcher_accolades_t_create_bindings, pitcher_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitcher_accolades_t_read_by_player_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int pitcher_accolades_t_read_by_player_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     pitcher_accolade_s pitcher_accolade = { 0 };

     pitcher_accolade.player_id = sqlite3_column_int(  statement, 0 );
     pitcher_accolade.season    = sqlite3_column_int(  statement, 1 );
     pitcher_accolade.accolade  = sqlite3_column_int(  statement, 2 );

     if ( add_to_data_list( data_list, &pitcher_accolade, sizeof(pitcher_accolade_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int pitcher_accolades_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *pitcher_accolades )
{
     static char query[] = "SELECT Player_Id, Season, Accolade FROM Pitcher_Accolades_T WHERE Player_Id = ?";

     return execute_query( db, query, pitcher_accolades_t_read_by_player_bindings, &player_id, pitcher_accolades_t_read_by_player_retrieve, pitcher_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int pitcher_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const pitcher_accolade_s *pitcher_accolade = (pitcher_accolade_s *)data;

     BIND_INT(  statement,  1, pitcher_accolade->player_id );
     BIND_INT(  statement,  2, pitcher_accolade->season    );
     BIND_INT(  statement,  3, pitcher_accolade->accolade  );

     return SQLITE_OK;
}


int pitcher_accolades_t_delete( sqlite3 *db, const pitcher_accolade_s *pitcher_accolade )
{
     static char query[] = "DELETE FROM Pitcher_Accolades_T WHERE Player_Id = ? AND Season = ? AND Accolade = ?";

     return execute_update_old( db, query, pitcher_accolades_t_delete_bindings, pitcher_accolade, NULL, NULL );
}
