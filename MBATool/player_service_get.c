#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "services.h"


static batter_accolade_s *get_batter_accolades( sqlite3 *db, const int player_id )
{
     static batter_accolade_s sentinel = BATTER_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( batter_accolades_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(batter_accolade_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static batter_stats_s *get_batter_stats( sqlite3 *db, const int player_id )
{
     static batter_stats_s sentinel = BATTER_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( batter_stats_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(batter_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static batter_s *get_batting_details( sqlite3 *db, const int player_id )
{
     batter_s *batter = NULL;

     if ( (batter = malloc( sizeof(batter_s) )) == NULL ) return NULL;

     memset( batter, '\0', sizeof(batter_s) );

     batter->player_id = player_id;

     if ( batters_t_read( db, batter ) != SQLITE_OK )
     {
          free( batter );

          return NULL;
     }

     return batter;
}

static batter_s *get_batter( sqlite3 *db, const int player_id )
{
     batter_s *batter = NULL;

     if ( (batter = get_batting_details(  db, player_id )) != NULL )
     {
          batter->stats     = get_batter_stats(     db, player_id );
          batter->accolades = get_batter_accolades( db, player_id );
     }

     return batter;
}

static pitcher_accolade_s *get_pitcher_accolades( sqlite3 *db, const int player_id )
{
     static pitcher_accolade_s sentinel = PITCHER_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( pitcher_accolades_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(pitcher_accolade_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static pitcher_stats_s *get_pitcher_stats( sqlite3 *db, const int player_id )
{
     static pitcher_stats_s sentinel = PITCHER_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( pitcher_stats_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(pitcher_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static pitcher_s *get_pitching_details( sqlite3 *db, const int player_id )
{
     pitcher_s *pitcher = NULL;

     if ( (pitcher = malloc( sizeof(pitcher_s) )) == NULL ) return NULL;

     memset( pitcher, '\0', sizeof(pitcher_s) );

     pitcher->player_id = player_id;

     if ( pitchers_t_read( db, pitcher ) != SQLITE_OK )
     {
          free( pitcher );

          return NULL;
     }

     return pitcher;
}

static pitcher_s *get_pitcher( sqlite3 *db, const int player_id )
{
     pitcher_s *pitcher = NULL;

     if ( (pitcher = get_pitching_details(  db, player_id )) != NULL )
     {
          pitcher->stats     = get_pitcher_stats(     db, player_id );
          pitcher->accolades = get_pitcher_accolades( db, player_id );
     }

     return pitcher;
}

static player_accolade_s *get_player_accolades( sqlite3 *db, const int player_id )
{
     static player_accolade_s sentinel = PLAYER_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( player_accolades_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(player_accolade_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static player_s *get_player_details( sqlite3 *db, const int player_id )
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

     return player;
}

player_s *get_player( sqlite3 *db, const int player_id )
{
     player_s *player = NULL;

     if ( (player = get_player_details( db, player_id )) == NULL ) return NULL;

     player->accolades = get_player_accolades( db, player_id );

     switch ( player->player_type )
     {
     case pt_Batter:  player->details.batting  = get_batter(  db, player_id ); break;
     case pt_Pitcher: player->details.pitching = get_pitcher( db, player_id ); break;
     }

     return player;
}
