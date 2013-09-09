#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"


static batter_s *get_batting_details( sqlite3 *db, int player_id )
{
     batter_s *batter = NULL;

     if ( (batter = malloc( sizeof(batter_s) )) == NULL ) return NULL;

     memset( batter, '\0', sizeof(batter_s) );

     batter->player_id = player_id;

     if ( batters_t_read( db, batter ) != SQLITE_OK ) return NULL;

     return batter;
}

static pitcher_stats_s *get_pitcher_stats( sqlite3 *db, int player_id )
{
     static pitcher_stats_s sentinel = PITCHER_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( pitcher_stats_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     add_to_data_list( &list, &sentinel, sizeof(pitcher_stats_s), 10 );

     return list.data;
}

static pitcher_s *get_pitching_details( sqlite3 *db, int player_id )
{
     pitcher_s *pitcher = NULL;

     if ( (pitcher = malloc( sizeof(pitcher_s) )) == NULL ) return NULL;

     memset( pitcher, '\0', sizeof(pitcher_s) );

     pitcher->player_id = player_id;

     if ( pitchers_t_read( db, pitcher ) != SQLITE_OK ) return NULL;

     return pitcher;
}

static player_accolade_s *get_player_accolades( sqlite3 *db, int player_id )
{
     static player_accolade_s sentinel = PLAYER_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( player_accolades_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     add_to_data_list( &list, &sentinel, sizeof(player_accolade_s), 10 );

     return list.data;
}

player_s *get_player( sqlite3 *db, int player_id )
{
     player_s *player = NULL;

     if ( (player = malloc( sizeof(player_s) )) == NULL ) return NULL;

     memset( player, '\0', sizeof(player_s) );

     player->player_id = player_id;

     if ( players_t_read( db, player ) != SQLITE_OK )
     {
          free( player );

          return NULL;
     }

     player->accolades = get_player_accolades( db, player_id );

     if ( player->player_type == pt_Pitcher )
     {
          pitcher_s *pitching = NULL;

          pitching        = get_pitching_details( db, player_id );
          pitching->stats = get_pitcher_stats( db, player_id );

          player->details.pitching = pitching;
     }

     if ( player->player_type == pt_Batter  ) player->details.batting  = get_batting_details(  db, player_id );

     return player;
}

void free_player( player_s *player )
{
     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          free( player->details.batting );

          player->details.batting = NULL;
     }

     if ( player->player_type == pt_Pitcher  &&   player->details.pitching != NULL )
     {
          if ( player->details.pitching->stats != NULL ) free( player->details.pitching->stats );

          player->details.pitching->stats = NULL;

          free( player->details.pitching );

          player->details.pitching = NULL;
     }

     if ( player->accolades != NULL ) free( player->accolades );

     player->accolades = NULL;

     free( player );
}
