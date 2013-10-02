#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "services.h"


static int remove_batter( sqlite3 *db, const batter_s *batter )
{
     int rc;

     if ( batter->stats != NULL )
     {
          for ( int i = 0; batter->stats[i].player_id >= 0; ++i )
          {
               TRY( batter_stats_t_delete( db, &batter->stats[i] ) );
          }
     }

     if ( batter->accolades != NULL )
     {
          for ( int i = 0; batter->accolades[i].player_id >= 0; ++i )
          {
               TRY( batter_accolades_t_delete( db, &batter->accolades[i] ) );
          }
     }

     return batters_t_delete( db, batter );
}

static int remove_pitcher( sqlite3 *db, const pitcher_s *pitcher )
{
     int rc;

     if ( pitcher->stats != NULL )
     {
          for ( int i = 0; pitcher->stats[i].player_id >= 0; ++i )
          {
               TRY( pitcher_stats_t_delete( db, &pitcher->stats[i] ) );
          }
     }

     if ( pitcher->accolades != NULL )
     {
          for ( int i = 0; pitcher->accolades[i].player_id >= 0; ++i )
          {
               TRY( pitcher_accolades_t_delete( db, &pitcher->accolades[i] ) );
          }
     }

     return pitchers_t_delete( db, pitcher );
}

int remove_player( sqlite3 *db, const player_s *player )
{
     int rc;

     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          TRY( remove_batter( db, player->details.batting ) );
     }

     if ( player->player_type == pt_Pitcher  &&  player->details.pitching != NULL )
     {
          TRY( remove_pitcher( db, player->details.pitching ) );
     }

     if ( player->accolades != NULL )
     {
          for ( int i = 0; player->accolades[i].player_id >= 0; ++i )
          {
               TRY( player_accolades_t_delete( db, &player->accolades[i] ) );
          }
     }

     return players_t_delete( db, player );
}
