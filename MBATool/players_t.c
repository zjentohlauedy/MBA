#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


int insert_player_cb( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_s *player = (player_s *)data;

     BIND_INT(  statement,  1, player->player_id      );
     BIND_TEXT( statement,  2, player->first_name     );
     BIND_TEXT( statement,  3, player->last_name      );
     BIND_TEXT( statement,  4, player->first_phonetic );
     BIND_TEXT( statement,  5, player->last_phonetic  );
     BIND_INT(  statement,  6, player->skin_tone      );
     BIND_INT(  statement,  7, player->handedness     );
     BIND_INT(  statement,  8, player->player_type    );
     BIND_INT(  statement,  9, player->rookie_season  );
     BIND_INT(  statement, 10, player->longevity      );

     return SQLITE_OK;
}


int insert_player( sqlite3 *db, const player_s *player )
{
     static char          query[]   = "INSERT INTO Players_T ( Player_Id, First_Name, Last_Name, First_Phonetic, Last_Phonetic, Skin_Tone, Handedness, Player_Type, Rookie_Season, Longevity )"
          /**/                                       "VALUES ( ?,"       "?,"        "?,"       "?,"            "?,"           "?,"       "?,"        "?,"         "?,"           "?"       ")";

     return execute_query( db, query, insert_player_cb, player );
}

int get_player( sqlite3 *db, player_s *player )
{
     static char          query[]   = "SELECT First_Name, Last_Name, First_Phonetic, Last_Phonetic, Skin_Tone, Handedness, Player_Type, Rookie_Season, Longevity FROM Players_T WHERE Player_Id = ?";
     /**/   sqlite3_stmt *statement;
     /**/   int           rc;


     if ( (rc = sqlite3_prepare_v2( db, query, sizeof(query), &statement, NULL )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_bind_int( statement, 1, player->player_id )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_step( statement )) != SQLITE_ROW ) return rc;

     cpytxt( player->first_name,      sqlite3_column_text( statement, 0 ) );
     cpytxt( player->last_name,       sqlite3_column_text( statement, 1 ) );
     cpytxt( player->first_phonetic,  sqlite3_column_text( statement, 2 ) );
     cpytxt( player->last_phonetic,   sqlite3_column_text( statement, 3 ) );
     /**/    player->skin_tone      = sqlite3_column_int(  statement, 4 );
     /**/    player->handedness     = sqlite3_column_int(  statement, 5 );
     /**/    player->player_type    = sqlite3_column_int(  statement, 6 );
     /**/    player->rookie_season  = sqlite3_column_int(  statement, 7 );
     /**/    player->longevity      = sqlite3_column_int(  statement, 8 );


     return sqlite3_finalize( statement );
}

int update_player( sqlite3 *db, const player_s *player )
{
     static char          query[]   = "UPDATE Players_T "
          /**/
          /**/                        "SET    First_Name     = ?,"
          /**/                               "Last_Name      = ?,"
          /**/                               "First_Phonetic = ?,"
          /**/                               "Last_Phonetic  = ?,"
          /**/                               "Skin_Tone      = ?,"
          /**/                               "Handedness     = ?,"
          /**/                               "Player_Type    = ?,"
          /**/                               "Rookie_Season  = ?,"
          /**/                               "Longevity      = ? "
          /**/
          /**/                        "WHERE  Player_Id = ?";
     /**/   sqlite3_stmt *statement;
     /**/   int           rc;

     if ( (rc = sqlite3_prepare_v2( db, query, sizeof(query), &statement, NULL )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_bind_text( statement,  1, player->first_name,     strlen(player->first_name),     SQLITE_STATIC )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_text( statement,  2, player->last_name,      strlen(player->last_name),      SQLITE_STATIC )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_text( statement,  3, player->first_phonetic, strlen(player->first_phonetic), SQLITE_STATIC )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_text( statement,  4, player->last_phonetic,  strlen(player->last_phonetic),  SQLITE_STATIC )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement,  5, player->skin_tone                                                     )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement,  6, player->handedness                                                    )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement,  7, player->player_type                                                   )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement,  8, player->rookie_season                                                 )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement,  9, player->longevity                                                     )) != SQLITE_OK ) return rc;
     if ( (rc = sqlite3_bind_int(  statement, 10, player->player_id                                                     )) != SQLITE_OK ) return rc;

     if ( (rc = sqlite3_step( statement )) != SQLITE_DONE ) return rc;

     return sqlite3_finalize( statement );
}

int delete_player_cb( sqlite3_stmt *statement, const void *data )
{
     const player_s *player = (player_s *)data;

     return sqlite3_bind_int( statement, 1, player->player_id );
}

int delete_player( sqlite3 *db, const player_s *player )
{
     return execute_query( db, "DELETE FROM Players_T WHERE Player_Id = ?", delete_player_cb, player );
}
