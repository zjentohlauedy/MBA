#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"


static batter_accolade_s *get_batter_accolades( sqlite3 *db, int player_id )
{
     static batter_accolade_s sentinel = BATTER_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( batter_accolades_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     add_to_data_list( &list, &sentinel, sizeof(batter_accolade_s), 10 );

     return list.data;
}

static batter_stats_s *get_batter_stats( sqlite3 *db, int player_id )
{
     static batter_stats_s sentinel = BATTER_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( batter_stats_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     add_to_data_list( &list, &sentinel, sizeof(batter_stats_s), 10 );

     return list.data;
}

static batter_s *get_batting_details( sqlite3 *db, int player_id )
{
     batter_s *batter = NULL;

     if ( (batter = malloc( sizeof(batter_s) )) == NULL ) return NULL;

     memset( batter, '\0', sizeof(batter_s) );

     batter->player_id = player_id;

     if ( batters_t_read( db, batter ) != SQLITE_OK ) return NULL;

     return batter;
}

static pitcher_accolade_s *get_pitcher_accolades( sqlite3 *db, int player_id )
{
     static pitcher_accolade_s sentinel = PITCHER_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( pitcher_accolades_t_read_by_player( db, player_id, &list ) != SQLITE_OK ) return NULL;

     add_to_data_list( &list, &sentinel, sizeof(pitcher_accolade_s), 10 );

     return list.data;
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

          if ( (pitching = get_pitching_details(  db, player_id )) != NULL )
          {
               pitching->stats     = get_pitcher_stats(     db, player_id );
               pitching->accolades = get_pitcher_accolades( db, player_id );
          }

          player->details.pitching = pitching;
     }

     if ( player->player_type == pt_Batter  )
     {
          batter_s *batting = NULL;

          if ( (batting = get_batting_details(  db, player_id )) != NULL )
          {
               batting->stats     = get_batter_stats(     db, player_id );
               batting->accolades = get_batter_accolades( db, player_id );
          }

          player->details.batting = batting;
     }

     return player;
}

static int upsert_batter_stats( sqlite3 *db, batter_stats_s *batter_stats )
{
     int rc;

     if ( (rc = batter_stats_t_create( db, batter_stats )) == SQLITE_CONSTRAINT )
     {
          return batter_stats_t_update( db, batter_stats );
     }

     return rc;
}

static int upsert_batter( sqlite3 *db, batter_s *batter )
{
     int rc;

     if ( (rc = batters_t_create( db, batter )) == SQLITE_CONSTRAINT )
     {
          return batters_t_update( db, batter );
     }

     return rc;
}

static int upsert_pitcher_stats( sqlite3 *db, pitcher_stats_s *pitcher_stats )
{
     int rc;

     if ( (rc = pitcher_stats_t_create( db, pitcher_stats )) == SQLITE_CONSTRAINT )
     {
          return pitcher_stats_t_update( db, pitcher_stats );
     }

     return rc;
}

static int upsert_pitcher( sqlite3 *db, pitcher_s *pitcher )
{
     int rc;

     if ( (rc = pitchers_t_create( db, pitcher )) == SQLITE_CONSTRAINT )
     {
          return pitchers_t_update( db, pitcher );
     }

     return rc;
}

static int upsert_player( sqlite3 *db, player_s *player )
{
     int rc;

     if ( (rc = players_t_create( db, player )) == SQLITE_CONSTRAINT )
     {
          return players_t_update( db, player );
     }

     return rc;
}

static int save_batter( sqlite3 *db, batter_s *batter )
{
     int rc;

     if ( batter->stats != NULL )
     {
          for ( int i = 0; batter->stats[i].player_id >= 0; ++i )
          {
               if ( (rc = upsert_batter_stats( db, &batter->stats[i] )) != SQLITE_OK ) return rc;
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

static int save_pitcher( sqlite3 *db, pitcher_s *pitcher )
{
     int rc;

     if ( pitcher->stats != NULL )
     {
          for ( int i = 0; pitcher->stats[i].player_id >= 0; ++i )
          {
               if ( (rc = upsert_pitcher_stats( db, &pitcher->stats[i] )) != SQLITE_OK ) return rc;
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

int save_player( sqlite3 *db, player_s *player )
{
     int rc;

     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          if ( (rc = save_batter( db, player->details.batting )) != SQLITE_OK ) return rc;
     }

     if ( player->player_type == pt_Pitcher  &&  player->details.pitching != NULL )
     {
          if ( (rc = save_pitcher( db, player->details.pitching )) != SQLITE_OK ) return rc;
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

static int remove_batter( sqlite3 *db, batter_s *batter )
{
     int rc;

     if ( batter->stats != NULL )
     {
          for ( int i = 0; batter->stats[i].player_id >= 0; ++i )
          {
               if ( (rc = batter_stats_t_delete( db, &batter->stats[i] )) != SQLITE_OK ) return rc;
          }
     }

     if ( batter->accolades != NULL )
     {
          for ( int i = 0; batter->accolades[i].player_id >= 0; ++i )
          {
               if ( (rc = batter_accolades_t_delete( db, &batter->accolades[i] )) != SQLITE_OK ) return rc;
          }
     }

     return batters_t_delete( db, batter );
}

static int remove_pitcher( sqlite3 *db, pitcher_s *pitcher )
{
     int rc;

     if ( pitcher->stats != NULL )
     {
          for ( int i = 0; pitcher->stats[i].player_id >= 0; ++i )
          {
               if ( (rc = pitcher_stats_t_delete( db, &pitcher->stats[i] )) != SQLITE_OK ) return rc;
          }
     }

     if ( pitcher->accolades != NULL )
     {
          for ( int i = 0; pitcher->accolades[i].player_id >= 0; ++i )
          {
               if ( (rc = pitcher_accolades_t_delete( db, &pitcher->accolades[i] )) != SQLITE_OK ) return rc;
          }
     }

     return pitchers_t_delete( db, pitcher );
}

int remove_player( sqlite3 *db, player_s *player )
{
     int rc;

     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          if ( (rc = remove_batter( db, player->details.batting )) != SQLITE_OK ) return rc;
     }

     if ( player->player_type == pt_Pitcher  &&  player->details.pitching != NULL )
     {
          if ( (rc = remove_pitcher( db, player->details.pitching )) != SQLITE_OK ) return rc;
     }

     if ( player->accolades != NULL )
     {
          for ( int i = 0; player->accolades[i].player_id >= 0; ++i )
          {
               if ( (rc = player_accolades_t_delete( db, &player->accolades[i] )) != SQLITE_OK ) return rc;
          }
     }

     return players_t_delete( db, player );
}

void free_player( player_s *player )
{
     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          if ( player->details.batting->stats != NULL ) free( player->details.batting->stats );

          player->details.batting->stats = NULL;

          if ( player->details.batting->accolades != NULL ) free( player->details.batting->accolades );

          player->details.batting->accolades = NULL;

          free( player->details.batting );

          player->details.batting = NULL;
     }

     if ( player->player_type == pt_Pitcher  &&   player->details.pitching != NULL )
     {
          if ( player->details.pitching->stats != NULL ) free( player->details.pitching->stats );

          player->details.pitching->stats = NULL;

          if ( player->details.pitching->accolades != NULL ) free( player->details.pitching->accolades );

          player->details.pitching->accolades = NULL;

          free( player->details.pitching );

          player->details.pitching = NULL;
     }

     if ( player->accolades != NULL ) free( player->accolades );

     player->accolades = NULL;

     free( player );
}
