#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_s *player = (player_s *)data;

     BIND_INT( statement,  1, player->player_id      );
     BIND_TXT( statement,  2, player->first_name     );
     BIND_TXT( statement,  3, player->last_name      );
     BIND_TXT( statement,  4, player->first_phonetic );
     BIND_TXT( statement,  5, player->last_phonetic  );
     BIND_INT( statement,  6, player->skin_tone      );
     BIND_INT( statement,  7, player->handedness     );
     BIND_INT( statement,  8, player->player_type    );
     BIND_INT( statement,  9, player->rookie_season  );
     BIND_INT( statement, 10, player->longevity      );

     return SQLITE_OK;
}


int players_t_create( sqlite3 *db, const player_s *player )
{
     static char query[]   = "INSERT INTO Players_T ( Player_Id, First_Name, Last_Name, First_Phonetic, Last_Phonetic, Skin_Tone, Handedness, Player_Type, Rookie_Season, Longevity )"
          /**/                              "VALUES ( ?,"       "?,"        "?,"       "?,"            "?,"           "?,"       "?,"        "?,"         "?,"           "?"       ")";

     return execute_update( db, query, players_t_create_bindings, player );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_s *player = (player_s *)data;

     return sqlite3_bind_int( statement, 1, player->player_id );
}

static int players_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_s *player = (player_s *)data;

     cpytxt( player->first_name,      sqlite3_column_text( statement, 0 ) );
     cpytxt( player->last_name,       sqlite3_column_text( statement, 1 ) );
     cpytxt( player->first_phonetic,  sqlite3_column_text( statement, 2 ) );
     cpytxt( player->last_phonetic,   sqlite3_column_text( statement, 3 ) );
     /**/    player->skin_tone      = sqlite3_column_int(  statement, 4 );
     /**/    player->handedness     = sqlite3_column_int(  statement, 5 );
     /**/    player->player_type    = sqlite3_column_int(  statement, 6 );
     /**/    player->rookie_season  = sqlite3_column_int(  statement, 7 );
     /**/    player->longevity      = sqlite3_column_int(  statement, 8 );

     return SQLITE_OK;
}

int players_t_read( sqlite3 *db, player_s *player )
{
     static char query[]   = "SELECT First_Name, Last_Name, First_Phonetic, Last_Phonetic, Skin_Tone, Handedness, Player_Type, Rookie_Season, Longevity FROM Players_T WHERE Player_Id = ?";

     return execute_query( db, query, players_t_read_bindings, player, players_t_read_retrieve, player );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_s *player = (player_s *)data;

     BIND_TXT( statement,  1, player->first_name     );
     BIND_TXT( statement,  2, player->last_name      );
     BIND_TXT( statement,  3, player->first_phonetic );
     BIND_TXT( statement,  4, player->last_phonetic  );
     BIND_INT( statement,  5, player->skin_tone      );
     BIND_INT( statement,  6, player->handedness     );
     BIND_INT( statement,  7, player->player_type    );
     BIND_INT( statement,  8, player->rookie_season  );
     BIND_INT( statement,  9, player->longevity      );

     BIND_INT( statement, 10, player->player_id );

     return SQLITE_OK;
}


int players_t_update( sqlite3 *db, const player_s *player )
{
     static char query[]   = "UPDATE Players_T "
          /**/
          /**/               "SET    First_Name     = ?,"
          /**/                      "Last_Name      = ?,"
          /**/                      "First_Phonetic = ?,"
          /**/                      "Last_Phonetic  = ?,"
          /**/                      "Skin_Tone      = ?,"
          /**/                      "Handedness     = ?,"
          /**/                      "Player_Type    = ?,"
          /**/                      "Rookie_Season  = ?,"
          /**/                      "Longevity      = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update( db, query, players_t_update_bindings, player );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_s *player = (player_s *)data;

     return sqlite3_bind_int( statement, 1, player->player_id );
}


int players_t_delete( sqlite3 *db, const player_s *player )
{
     return execute_update( db, "DELETE FROM Players_T WHERE Player_Id = ?", players_t_delete_bindings, player );
}
