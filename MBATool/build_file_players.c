#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"

static fileposition_e pickPosition( void )
{
     struct
     {
          fileposition_e pos;
          int            weight;

     } possibilities[] = { { fpos_Pitcher,       288 },
                           { fpos_Catcher,        54 },
                           { fpos_FirstBaseman,   54 },
                           { fpos_SecondBaseman,  54 },
                           { fpos_ThirdBaseman,   54 },
                           { fpos_ShortStop,      54 },
                           { fpos_LeftField,      54 },
                           { fpos_CenterField,    54 },
                           { fpos_RightField,     54 }
     };

     const static int total = 720;

     int x = (rand() % total) + 1;

     for ( int i = 0; i < 8; ++i )
     {
          if ( (x -= possibilities[i].weight) <= 0 ) return possibilities[i].pos;
     }

     return fpos_Pitcher;
}

static void buildBatterStats( acc_bat_stats_s *stats, acc_bat_stats_s *overflow )
{
     int rbi = rand() % 500;
     int so  = rand() % 500;

     int2word( stats->acc_ab,        rand() % 50000 );
     int2word( stats->acc_hits,      rand() % 50000 );
     /**/      stats->acc_2b[0]    = rand() % 250;
     /**/      stats->acc_3b[0]    = rand() % 250;
     /**/      stats->acc_hr[0]    = rand() % 250;
     /**/      stats->acc_bb[0]    = rand() % 250;
     /**/      stats->acc_games[0] = rand() % 250;
     /**/      stats->acc_runs[0]  = rand() % 250;
     /**/      stats->acc_sb[0]    = rand() % 250;
     /**/      stats->acc_err[0]   = rand() % 250;

     if   ( rbi <= 250 ) { stats->acc_rbi[0] = rbi;                                   }
     else                { stats->acc_rbi[0] = 250; overflow->acc_rbi[0] = rbi - 250; }

     if   ( so  <= 250 ) { stats->acc_so[0]  = so;                                    }
     else                { stats->acc_so[0]  = 250; overflow->acc_so[0]  = so  - 250; }
}

static void buildPitcherStats( acc_pch_stats_s *stats )
{
     /**/      stats->acc_wins[0]   =  rand() % 250;
     /**/      stats->acc_losses[0] =  rand() % 250;
     /**/      stats->acc_starts[0] =  rand() % 250;
     /**/      stats->acc_saves[0]  =  rand() % 250;
     int2word( stats->acc_innings,    (rand() % 5000) + (rand() % 3) );
     int2word( stats->acc_hits,        rand() % 50000                );
     int2word( stats->acc_er,          rand() % 50000                );
     /**/      stats->acc_hr[0]     =  rand() % 250;
     /**/      stats->acc_bb[0]     =  rand() % 250;
     int2word( stats->acc_so,          rand() % 50000                );
}

fileplayer_s *buildFilePlayers( void )
{
     fileplayer_s *players_data = buildFilePlayersWithoutIds();

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          unsigned char pos       = nibble( players_data[i].position[0], n_High );
          int           player_id = (rand() % 50000) + 1;

          if ( pos == fpos_Pitcher )
          {
               acc_player_id_s *player_id_data = (acc_player_id_s *)&(players_data[i].filestats.filepitching.action);

               int2word( player_id_data->player_id,                  player_id );
               /**/      player_id_data->checksum[0] = calcChecksum( player_id );
          }
          else
          {
               acc_player_id_s *player_id_data = (acc_player_id_s *)&(players_data[i].filestats.filebatting.action);

               int2word( player_id_data->player_id,                  player_id );
               /**/      player_id_data->checksum[0] = calcChecksum( player_id );
          }
     }

     return players_data;
}

fileplayer_s *buildFilePlayersWithoutIds( void )
{
     static fileplayer_s players_data[TOTAL_PLAYERS] = { 0 };

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          memset( &players_data[i], '\0', sizeof(fileplayer_s) );

          unsigned char pos       = pickPosition();
          unsigned char hand      = ((rand() % 100)) > 49 ? fh_Right : fh_Left;
          unsigned char color     = ((rand() % 100)) > 49 ? fc_Light : fc_Dark;
          unsigned char longevity = (rand() % 10) + 1;
          unsigned char position;
          int           year      = YEAR_SEASON_OFFSET + (i / 100);

          if ( pos == fpos_Pitcher )
          {
               struct pitching_s *pitching = &(players_data[i].filestats.filepitching);

               unsigned char speed     = (rand() % 10) + 1;
               unsigned char control   = (rand() % 10) + 1;
               unsigned char fatigue   = (rand() % 10) + 1;

               position = (pos << 4) + hand;

               pitching->color[0]   = color;
               pitching->ratings[0] = (speed   << 4) + control;
               pitching->ratings[1] = (fatigue << 4) + longevity;

               buildPitcherStats( &(pitching->simulated) );
          }
          else
          {
               struct batting_s *batting = &(players_data[i].filestats.filebatting);

               unsigned char pos2      = pickPosition();
               unsigned char power     = (rand() % 10) + 1;
               unsigned char hit_n_run = (rand() % 10) + 1;
               unsigned char bunt      = (rand() % 10) + 1;
               unsigned char running   = (rand() % 10) + 1;
               unsigned char range     = (rand() % 10) + 1;
               unsigned char arm       = (rand() % 10) + 1;

               position = (pos << 4) + pos2;

               batting->color[0]   = color;
               batting->ratings[0] = (hand    << 4) + arm;
               batting->ratings[1] = (running << 4) + range;
               batting->ratings[2] = (power   << 4) + longevity;
               batting->ratings[3] = (bunt    << 4) + hit_n_run;

               buildBatterStats( &(batting->simulated), &(batting->action) );
          }

          sprintf( players_data[i].first_name,   "Fst%d",    i + 1 );
          sprintf( players_data[i].last_name,    "Last%d",   i + 1 );
          sprintf( players_data[i].first_phoen,  "FIR3ST%d", i + 1 );
          sprintf( players_data[i].last_phoen,   "LAH4ST%d", i + 1 );
          /**/     players_data[i].year[0]      = year;
          /**/     players_data[i].position[0]  = position;
     }

     return players_data;
}
