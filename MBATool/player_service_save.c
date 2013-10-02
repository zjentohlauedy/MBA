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

static int save_batter( sqlite3 *db, const batter_s *batter )
{
     int rc;

     if ( batter->stats != NULL )
     {
          for ( int i = 0; batter->stats[i].player_id >= 0; ++i )
          {
               TRY( upsert_batter_stats( db, &batter->stats[i] ) );
          }
     }

     if ( batter->accolades != NULL )
     {
          for ( int i = 0; batter->accolades[i].player_id >= 0; ++i )
          {
               if ( (rc = batter_accolades_t_create( db, &batter->accolades[i] )) != SQLITE_OK )
               {
                    if ( rc != SQLITE_CONSTRAINT ) return rc;
               }
          }
     }

     return upsert_batter( db, batter );
}

static int save_pitcher( sqlite3 *db, const pitcher_s *pitcher )
{
     int rc;

     if ( pitcher->stats != NULL )
     {
          for ( int i = 0; pitcher->stats[i].player_id >= 0; ++i )
          {
               TRY( upsert_pitcher_stats( db, &pitcher->stats[i] ) );
          }
     }

     if ( pitcher->accolades != NULL )
     {
          for ( int i = 0; pitcher->accolades[i].player_id >= 0; ++i )
          {
               if ( (rc = pitcher_accolades_t_create( db, &pitcher->accolades[i] )) != SQLITE_OK )
               {
                    if ( rc != SQLITE_CONSTRAINT ) return rc;
               }
          }
     }

     return upsert_pitcher( db, pitcher );
}

int save_player( sqlite3 *db, const player_s *player )
{
     int rc;

     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          TRY( save_batter( db, player->details.batting ) );
     }

     if ( player->player_type == pt_Pitcher  &&  player->details.pitching != NULL )
     {
          TRY( save_pitcher( db, player->details.pitching ) );
     }

     if ( player->accolades != NULL )
     {
          for ( int i = 0; player->accolades[i].player_id >= 0; ++i )
          {
               if ( (rc = player_accolades_t_create( db, &player->accolades[i] )) != SQLITE_OK )
               {
                    if ( rc != SQLITE_CONSTRAINT ) return rc;
               }
          }
     }

     return upsert_player( db, player );
}
