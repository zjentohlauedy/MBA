#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "services.h"


static int remove_batter_stats( sqlite3 *db, batter_stats_s *batter_stats )
{
     int rc;

     if ( batter_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; batter_stats[i].player_id >= 0; ++i )
     {
          TRY( batter_stats_t_delete( db, &batter_stats[i] ) );
     }

     return SQLITE_OK;
}

static int remove_batter_accolades( sqlite3 *db, batter_accolade_s *batter_accolades )
{
     int rc;

     if ( batter_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; batter_accolades[i].player_id >= 0; ++i )
     {
          TRY( batter_accolades_t_delete( db, &batter_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int remove_pitcher_stats( sqlite3 *db, pitcher_stats_s *pitcher_stats )
{
     int rc;

     if ( pitcher_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; pitcher_stats[i].player_id >= 0; ++i )
     {
          TRY( pitcher_stats_t_delete( db, &pitcher_stats[i] ) );
     }

     return SQLITE_OK;
}

static int remove_pitcher_accolades( sqlite3 *db, pitcher_accolade_s *pitcher_accolades )
{
     int rc;

     if ( pitcher_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; pitcher_accolades[i].player_id >= 0; ++i )
     {
          TRY( pitcher_accolades_t_delete( db, &pitcher_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int remove_player_accolades( sqlite3 *db, player_accolade_s *player_accolades )
{
     int rc;

     if ( player_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; player_accolades[i].player_id >= 0; ++i )
     {
          TRY( player_accolades_t_delete( db, &player_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int remove_batter( sqlite3 *db, const batter_s *batter )
{
     int rc;

     if ( batter == NULL ) return SQLITE_OK;

     TRY( remove_batter_stats(     db, batter->stats     ) );
     TRY( remove_batter_accolades( db, batter->accolades ) );

     return batters_t_delete( db, batter );
}

static int remove_pitcher( sqlite3 *db, const pitcher_s *pitcher )
{
     int rc;

     if ( pitcher == NULL ) return SQLITE_OK;

     TRY( remove_pitcher_stats(     db, pitcher->stats     ) );
     TRY( remove_pitcher_accolades( db, pitcher->accolades ) );

     return pitchers_t_delete( db, pitcher );
}

int remove_player( sqlite3 *db, const player_s *player )
{
     int rc;

     switch ( player->player_type )
     {
     case pt_Batter:  TRY( remove_batter(  db, player->details.batting  ) ); break;
     case pt_Pitcher: TRY( remove_pitcher( db, player->details.pitching ) ); break;
     }

     TRY( remove_player_accolades( db, player->accolades ) );

     return players_t_delete( db, player );
}
