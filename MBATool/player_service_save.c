#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "services.h"


static int upsert_batter_stats( sqlite3 *db, const batter_stats_s *batter_stats )
{
     int rc;

     if ( (rc = batter_stats_t_create( db, batter_stats )) == SQLITE_CONSTRAINT )
     {
          return batter_stats_t_update( db, batter_stats );
     }

     return rc;
}

static int upsert_batter( sqlite3 *db, const batter_s *batter )
{
     int rc;

     if ( (rc = batters_t_create( db, batter )) == SQLITE_CONSTRAINT )
     {
          return batters_t_update( db, batter );
     }

     return rc;
}

static int upsert_pitcher_stats( sqlite3 *db, const pitcher_stats_s *pitcher_stats )
{
     int rc;

     if ( (rc = pitcher_stats_t_create( db, pitcher_stats )) == SQLITE_CONSTRAINT )
     {
          return pitcher_stats_t_update( db, pitcher_stats );
     }

     return rc;
}

static int upsert_pitcher( sqlite3 *db, const pitcher_s *pitcher )
{
     int rc;

     if ( (rc = pitchers_t_create( db, pitcher )) == SQLITE_CONSTRAINT )
     {
          return pitchers_t_update( db, pitcher );
     }

     return rc;
}

static int upsert_player( sqlite3 *db, const player_s *player )
{
     int rc;

     if ( (rc = players_t_create( db, player )) == SQLITE_CONSTRAINT )
     {
          return players_t_update( db, player );
     }

     return rc;
}

static int save_batter_stats( sqlite3 *db, const batter_stats_s *batter_stats )
{
     int rc;

     if ( batter_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; batter_stats[i].player_id >= 0; ++i )
     {
          TRY( upsert_batter_stats( db, &batter_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_batter_accolades( sqlite3 *db, const batter_accolade_s *batter_accolades )
{
     int rc;

     if ( batter_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; batter_accolades[i].player_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( batter_accolades_t_create( db, &batter_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int save_pitcher_stats( sqlite3 *db, const pitcher_stats_s *pitcher_stats )
{
     int rc;

     if ( pitcher_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; pitcher_stats[i].player_id >= 0; ++i )
     {
          TRY( upsert_pitcher_stats( db, &pitcher_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_pitcher_accolades( sqlite3 *db, const pitcher_accolade_s *pitcher_accolades )
{
     int rc;

     if ( pitcher_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; pitcher_accolades[i].player_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( pitcher_accolades_t_create( db, &pitcher_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int save_player_accolades( sqlite3 *db, const player_accolade_s *player_accolades )
{
     int rc;

     if ( player_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; player_accolades[i].player_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( player_accolades_t_create( db, &player_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int save_batter( sqlite3 *db, const batter_s *batter )
{
     int rc;

     if ( batter == NULL ) return SQLITE_OK;

     TRY( save_batter_stats(     db, batter->stats     ) );
     TRY( save_batter_accolades( db, batter->accolades ) );

     return upsert_batter( db, batter );
}

static int save_pitcher( sqlite3 *db, const pitcher_s *pitcher )
{
     int rc;

     if ( pitcher == NULL ) return SQLITE_OK;

     TRY( save_pitcher_stats(     db, pitcher->stats     ) );
     TRY( save_pitcher_accolades( db, pitcher->accolades ) );

     return upsert_pitcher( db, pitcher );
}

int save_player( sqlite3 *db, const player_s *player )
{
     int rc;

     switch ( player->player_type )
     {
     case pt_Batter:  TRY( save_batter(  db, player->details.batting  ) ); break;
     case pt_Pitcher: TRY( save_pitcher( db, player->details.pitching ) ); break;
     }

     TRY( save_player_accolades( db, player->accolades ) );

     return upsert_player( db, player );
}
