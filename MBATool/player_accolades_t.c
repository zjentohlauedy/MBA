#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "macros.h"
#include "player.h"

int   insert_player_accolade(    sqlite3 *db, const player_accolade_s *player_accolade )
{
     static char          query[]   = "INSERT INTO Player_Accolades_T ( Player_Id, Season, Accolade ) VALUES ( ?, ?, ? )";
     /**/   sqlite3_stmt *statement;
     /**/   int           rc;

     if ( (rc = sqlite3_prepare_v2( db, query, sizeof(query), &statement, NULL )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_bind_int(  statement,  1, player->player_id )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement,  9, player->season    )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement, 10, player->accolade  )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_step( statement )) != SQLITE_DONE ) return rc;

     return sqlite3_finalize( statement );
}

int   get_player_accolade(       sqlite3 *db,       player_accolade_s *player_accolade )
{

}

int   update_player_accolade(    sqlite3 *db, const player_accolade_s *player_accolade )
{

}

int   delete_player_accolade(    sqlite3 *db, const player_accolade_s *player_accolade )
{

}


