#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "compares.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static void swapBatterStats( fileplayer_s *players_file )
{
     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               acc_stats_u *sim = &(players_file[i].acc_stats.simulated);
               acc_stats_u *act = &(players_file[i].acc_stats.action);

               acc_stats_u temp = *sim;

               *sim = *act;
               *act = temp;
          }
     }
}


static char *formatDosToAmiga_ShouldReturnTrueIfSuccessful_GivenAPlayersFile()
{
     fileplayer_s players_file[TOTAL_PLAYERS] = { 0 };

     assertEquals( bl_True, formatDosToAmiga( players_file ) );

     return NULL;
}

static char *formatDosToAmiga_ShouldReturnFalseIfBatterNeedingOverflowHasPitchingStats_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayers();

     swapBatterStats( players_file );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               acc_bat_stats_s *batting = &(players_file[i].acc_stats.action.batting);

               int2byte( batting->acc_rbi, STAT_CAP_AMOUNT + 1 );
               int2byte( batting->acc_so,  STAT_CAP_AMOUNT + 1 );

               acc_pch_stats_s *pitching = &(players_file[i].acc_stats.simulated.pitching);

               int2word( pitching->acc_innings, 1 );
               int2word( pitching->acc_hits,    1 );

               break;
          }
     }

     assertEquals( bl_False, formatDosToAmiga( players_file ) );

     return NULL;
}

static char *formatDosToAmiga_ShouldReturnFalseOnPlayerIdChecksumMismatch_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayers();

     acc_player_id_s *id_info = &(players_file[0].acc_stats.action.id_info);

     id_info->checksum[0]++;

     swapBatterStats( players_file );

     assertEquals( bl_False, formatDosToAmiga( players_file ) );

     return NULL;
}

static char *formatDosToAmiga_ShouldMovePitcherActionGamesToSimulatedStarts_GivenAPlayersFile()
{
     fileplayer_s *players_file    = buildFilePlayers();
     int           expected_starts = 0;
     int           player_idx      = 0;

     swapBatterStats( players_file );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos == fpos_Pitcher )
          {
               acc_pch_stats_s *sim = &(players_file[i].acc_stats.simulated.pitching);
               acc_bat_stats_s *act = &(players_file[i].acc_stats.action.batting);

               expected_starts = byte2int( sim->acc_starts );
               player_idx      = i;

               int2byte( sim->acc_starts, byte2int( act->acc_games ) );
               int2byte( act->acc_games,  expected_starts            );

               break;
          }
     }

     assertEquals( bl_True, formatDosToAmiga( players_file ) );

     acc_pch_stats_s *sim = &(players_file[player_idx].acc_stats.simulated.pitching);

     assertEquals( expected_starts, byte2int( sim->acc_starts ) );

     return NULL;
}

static char *formatDosToAmiga_ShouldSwapBatterSimulatedWithAction_GivenAPlayersFile()
{
     fileplayer_s    *players_file = buildFilePlayers();
     acc_bat_stats_s  expected_sim = { 0 };
     acc_bat_stats_s  expected_act = { 0 };
     int              player_idx   =   0;

     swapBatterStats( players_file );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               if ( byte2int( players_file[i].acc_stats.action.batting.acc_rbi ) < STAT_CAP_AMOUNT &&
                    byte2int( players_file[i].acc_stats.action.batting.acc_so  ) < STAT_CAP_AMOUNT    )
               {
                    expected_act = players_file[i].acc_stats.simulated.batting;
                    expected_sim = players_file[i].acc_stats.action.batting;
                    player_idx   = i;

                    break;
               }
          }
     }

     assertEquals( bl_True, formatDosToAmiga( players_file ) );

     compareBattingStats( &expected_act, &(players_file[player_idx].acc_stats.action.batting)    );
     compareBattingStats( &expected_sim, &(players_file[player_idx].acc_stats.simulated.batting) );

     return NULL;
}

static char *formatDosToAmiga_ShouldCapRBIAndMoveExcessFromActionToSimulated_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayers();
     int           player_idx   = 0;

     swapBatterStats( players_file );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               int2byte( players_file[i].acc_stats.action.batting.acc_rbi,    247 );
               int2byte( players_file[i].acc_stats.simulated.batting.acc_rbi,   0 );

               player_idx = i;

               break;
          }
     }

     formatDosToAmiga( players_file );

     assertEquals( 225, byte2int( players_file[player_idx].acc_stats.simulated.batting.acc_rbi ) );
     assertEquals(  22, byte2int( players_file[player_idx].acc_stats.action.batting.acc_rbi    ) );

     return NULL;
}

static char *formatDosToAmiga_ShouldCapStrikeOutsAndMoveExcessFromActionToSimulated_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayers();
     int           player_idx   = 0;

     swapBatterStats( players_file );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               int2byte( players_file[i].acc_stats.action.batting.acc_so,    229 );
               int2byte( players_file[i].acc_stats.simulated.batting.acc_so,   0 );

               player_idx = i;

               break;
          }
     }

     formatDosToAmiga( players_file );

     assertEquals( 225, byte2int( players_file[player_idx].acc_stats.simulated.batting.acc_so ) );
     assertEquals(   4, byte2int( players_file[player_idx].acc_stats.action.batting.acc_so    ) );

     return NULL;
}

static void get_error_message()
{
     printf( "formatDosToAmiga error message: %s\n", getFormatDosToAmigaError() );
}

static void run_all_tests()
{
     run_test( formatDosToAmiga_ShouldReturnTrueIfSuccessful_GivenAPlayersFile,                             get_error_message );
     run_test( formatDosToAmiga_ShouldReturnFalseIfBatterNeedingOverflowHasPitchingStats_GivenAPlayersFile, get_error_message );
     run_test( formatDosToAmiga_ShouldReturnFalseOnPlayerIdChecksumMismatch_GivenAPlayersFile,              get_error_message );
     run_test( formatDosToAmiga_ShouldMovePitcherActionGamesToSimulatedStarts_GivenAPlayersFile,            get_error_message );
     run_test( formatDosToAmiga_ShouldSwapBatterSimulatedWithAction_GivenAPlayersFile,                      get_error_message );
     run_test( formatDosToAmiga_ShouldCapRBIAndMoveExcessFromActionToSimulated_GivenAPlayersFile,           get_error_message );
     run_test( formatDosToAmiga_ShouldCapStrikeOutsAndMoveExcessFromActionToSimulated_GivenAPlayersFile,    get_error_message );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
