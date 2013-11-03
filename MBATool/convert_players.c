#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

static char *convertTerminatedBuffer( const char *buffer, const size_t length )
{
     static char result[99 + 1] = { 0 };

     memset( result, '\0', sizeof(result) );

     memcpy( result, buffer, length );

     untermName( result, length );

     return result;
}

static handedness_e mapHandedness( const filehand_e filehandedness )
{
     switch ( filehandedness )
     {
     case fh_Right: return hnd_Right;
     case fh_Left:  return hnd_Left;
     }

     return hnd_None;
}

static skin_tone_e mapSkinTone( const filecolor_e color )
{
     switch ( color )
     {
     case fc_Light: return st_Light;
     case fc_Dark:  return st_Dark;
     }

     return st_None;
}

static position_e mapPosition( const fileposition_e pos )
{
     switch ( pos )
     {
     case fpos_Catcher:       return pos_Catcher;
     case fpos_FirstBaseman:  return pos_FirstBase;
     case fpos_SecondBaseman: return pos_SecondBase;
     case fpos_ThirdBaseman:  return pos_ThirdBase;
     case fpos_ShortStop:     return pos_ShortStop;
     case fpos_LeftField:     return pos_LeftField;
     case fpos_CenterField:   return pos_CenterField;
     case fpos_RightField:    return pos_RightField;
     }

     return pos_None;
}

static batter_stats_s *convertBatterStats( const int player_id, const int season, const season_phase_e season_phase, const acc_bat_stats_s *stats, const acc_bat_stats_s *addtl_stats )
{
     data_list_s      list         = { 0 };
     batter_stats_s   sentinel     = BATTER_STATS_SENTINEL;
     batter_stats_s   batter_stats = { 0 };

     int rbi = stats->acc_rbi[0] + addtl_stats->acc_rbi[0];
     int so  = stats->acc_so[0]  + addtl_stats->acc_so[0];

     batter_stats.player_id      =                  player_id;
     batter_stats.season         =                  season;
     batter_stats.season_phase   =                  season_phase;
     batter_stats.games          =           stats->acc_games[0];
     batter_stats.at_bats        = word2int( stats->acc_ab        );
     batter_stats.runs           =           stats->acc_runs[0];
     batter_stats.hits           = word2int( stats->acc_hits      );
     batter_stats.doubles        =           stats->acc_2b[0];
     batter_stats.triples        =           stats->acc_3b[0];
     batter_stats.home_runs      =           stats->acc_hr[0];
     batter_stats.runs_batted_in =                  rbi;
     batter_stats.walks          =           stats->acc_bb[0];
     batter_stats.strike_outs    =                  so;
     batter_stats.steals         =           stats->acc_sb[0];
     batter_stats.errors         =           stats->acc_err[0];

     if ( add_to_data_list( &list, &batter_stats, sizeof(batter_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,     sizeof(batter_stats_s), 5 );

     return list.data;
}

static batter_s *createBatter( const int player_id, const int season, const season_phase_e season_phase, const filebatting_s *batter_data, const unsigned char positions )
{
     batter_s *batter = NULL;

     if ( (batter = malloc( sizeof(batter_s) )) == NULL ) return NULL;

     memset( batter, '\0', sizeof(batter_s) );

     batter->player_id          = player_id;
     batter->primary_position   = mapPosition( nibble( positions,               n_High ) );
     batter->secondary_position = mapPosition( nibble( positions,               n_Low  ) );
     batter->power              =              nibble( batter_data->ratings[2], n_High );
     batter->hit_n_run          =              nibble( batter_data->ratings[3], n_Low  );
     batter->bunt               =              nibble( batter_data->ratings[3], n_High );
     batter->running            =              nibble( batter_data->ratings[1], n_High );
     batter->range              =              nibble( batter_data->ratings[1], n_Low  );
     batter->arm                =              nibble( batter_data->ratings[0], n_Low  );

     if ( (batter->stats = convertBatterStats( player_id, season, season_phase, &(batter_data->simulated), &(batter_data->action) )) == NULL )
     {
          free( batter );

          return NULL;
     }

     return batter;
}

static pitcher_stats_s *convertPitcherStats( const int player_id, const int season, const season_phase_e season_phase, const acc_pch_stats_s *stats )
{
     data_list_s      list          = { 0 };
     pitcher_stats_s  sentinel      = PITCHER_STATS_SENTINEL;
     pitcher_stats_s  pitcher_stats = { 0 };

     pitcher_stats.player_id       =                  player_id;
     pitcher_stats.season          =                  season;
     pitcher_stats.season_phase    =                  season_phase;
     pitcher_stats.wins            =           stats->acc_wins[0];
     pitcher_stats.losses          =           stats->acc_losses[0];
     pitcher_stats.games           =           stats->acc_starts[0];
     pitcher_stats.saves           =           stats->acc_saves[0];
     pitcher_stats.innings.innings = word2int( stats->acc_innings   ) / 10;
     pitcher_stats.innings.outs    = word2int( stats->acc_innings   ) % 10;
     pitcher_stats.hits            = word2int( stats->acc_hits      );
     pitcher_stats.earned_runs     = word2int( stats->acc_er        );
     pitcher_stats.home_runs       =           stats->acc_hr[0];
     pitcher_stats.walks           =           stats->acc_bb[0];
     pitcher_stats.strike_outs     = word2int( stats->acc_so        );

     if ( add_to_data_list( &list, &pitcher_stats, sizeof(pitcher_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,      sizeof(pitcher_stats_s), 5 );

     return list.data;
}

static pitcher_s *createPitcher( const int player_id, const int season, const season_phase_e season_phase, const filepitching_s *pitcher_data )
{
     pitcher_s *pitcher = NULL;

     if ( (pitcher = malloc( sizeof(pitcher_s) )) == NULL ) return NULL;

     memset( pitcher, '\0', sizeof(pitcher_s) );

     pitcher->player_id = player_id;
     pitcher->speed     = nibble( pitcher_data->ratings[0], n_High );
     pitcher->control   = nibble( pitcher_data->ratings[0], n_Low  );
     pitcher->bunt      = nibble( pitcher_data->ratings[1], n_Low  );
     pitcher->fatigue   = nibble( pitcher_data->ratings[1], n_High );

     if ( (pitcher->stats = convertPitcherStats( player_id, season, season_phase, &(pitcher_data->simulated) )) == NULL )
     {
          free( pitcher );

          return NULL;
     }

     return pitcher;
}

static player_s *createPlayer( const int season, const season_phase_e season_phase, const fileplayer_s *players_data )
{
     player_s *player    = NULL;
     int       player_id = 0;

     if ( (player = malloc( sizeof(player_s) )) == NULL ) return NULL;

     memset( player, '\0', sizeof(player_s) );

     int pos = nibble( players_data->position[0], n_High );

     if ( pos == fpos_Pitcher )
     {
          const filepitching_s  *pitcher_data = &(players_data->filestats.filepitching);
          const acc_player_id_s *id_info      = (acc_player_id_s *)&(pitcher_data->action);

          player_id = word2int( id_info->player_id );

          if ( calcChecksum( player_id ) != id_info->checksum[0] )
          {
               free( player );

               return NULL;
          }

          if ( (player->details.pitching = createPitcher( player_id, season, season_phase, pitcher_data )) == NULL )
          {
               free( player );

               return NULL;
          }

          player->player_type = pt_Pitcher;
          player->handedness  = mapHandedness( nibble( players_data->position[0], n_Low ) );
          player->skin_tone   = mapSkinTone(           pitcher_data->color[0]             );
          player->longevity   =                nibble( pitcher_data->ratings[1],  n_Low   );
     }
     else
     {
          const filebatting_s   *batter_data = &(players_data->filestats.filebatting);
          const acc_player_id_s *id_info     = (acc_player_id_s *)&(batter_data->action);

          player_id = word2int( id_info->player_id );

          if ( calcChecksum( player_id ) != id_info->checksum[0] )
          {
               free( player );

               return NULL;
          }

          if ( (player->details.batting = createBatter( player_id, season, season_phase, batter_data, players_data->position[0] )) == NULL )
          {
               free( player );

               return NULL;
          }

          player->player_type = pt_Batter;
          player->handedness  = mapHandedness( nibble( batter_data->ratings[0], n_High ) );
          player->skin_tone   = mapSkinTone(           batter_data->color[0]             );
          player->longevity   =                nibble( batter_data->ratings[2],  n_Low   );
     }

     strcpy( player->first_name,       convertTerminatedBuffer( players_data->first_name,  sizeof(players_data->first_name)  ) );
     strcpy( player->last_name,        convertTerminatedBuffer( players_data->last_name,   sizeof(players_data->last_name)   ) );
     strcpy( player->first_phonetic,   convertTerminatedBuffer( players_data->first_phoen, sizeof(players_data->first_phoen) ) );
     strcpy( player->last_phonetic,    convertTerminatedBuffer( players_data->last_phoen,  sizeof(players_data->last_phoen)  ) );
     /**/    player->player_id         =                        player_id;
     /**/    player->rookie_season     =                        players_data->year[0] - YEAR_SEASON_OFFSET;

     return player;
}

static void freePlayers( player_s *players[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( players[i] != NULL ) free_player( players[i] );
     }
}

static boolean_e addPlayerToList( data_list_s *list, const int team_id, const int season, player_s *player )
{
     team_player_s team_player = { 0 };

     team_player.team_id   = team_id;
     team_player.season    = season;
     team_player.player_id = player->player_id;
     team_player.player    = player;

     if ( add_to_data_list( list, &team_player, sizeof(team_player_s), 30 ) < 0 ) return bl_False;

     return bl_True;
}

team_player_s *convertPlayers( const org_data_s *org_data, const int team_id )
{
     data_list_s    list                      = { 0 };
     team_player_s  sentinel                  = TEAM_PLAYER_SENTINEL;
     player_s      *players[PLAYERS_PER_TEAM] = { 0 };


     int idx = (team_id - 1) * PLAYERS_PER_TEAM;

     for ( int i = 0; i < PLAYERS_PER_TEAM; ++i )
     {
          if ( org_data->players_data[idx + i].last_name[0] == '\0' ) continue;

          if ( (players[i] = createPlayer( org_data->season, org_data->season_phase, &(org_data->players_data[idx + i]) )) == NULL )
          {
               freePlayers( players, PLAYERS_PER_TEAM );

               return NULL;
          }

          if ( addPlayerToList( &list, team_id, org_data->season, players[i] ) != bl_True )
          {
               freePlayers( players, PLAYERS_PER_TEAM );

               return NULL;
          }
     }

     add_to_data_list( &list, &sentinel, sizeof(team_player_s), 30 );

     return list.data;
}
