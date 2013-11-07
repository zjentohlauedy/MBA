#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "compares.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static char *formatAmigaToDos_ShouldMovePitcherSimulatedStartsToActionGames_GivenAPlayersFile()
{
     fileplayer_s *players_file   = buildFilePlayers();
     int           expected_games = 0;
     int           player_idx     = 0;

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos == fpos_Pitcher )
          {
               acc_pch_stats_s *sim = &(players_file[i].filestats.filepitching.simulated);

               expected_games = byte2int( sim->acc_starts );
               player_idx     = i;

               break;
          }
     }

     formatAmigaToDos( players_file );

     acc_bat_stats_s *act = (acc_bat_stats_s *)&(players_file[player_idx].filestats.filepitching.action);

     assertEquals( expected_games, byte2int( act->acc_games ) );

     return NULL;
}

static char *formatAmigaToDos_ShouldSwapBatterSimulatedWithAction_GivenAPlayersFile()
{
     fileplayer_s    *players_file   = buildFilePlayers();
     acc_bat_stats_s  expected_sim = { 0 };
     acc_bat_stats_s  expected_act = { 0 };
     int              player_idx   =   0;

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               if ( byte2int( players_file[i].filestats.filebatting.simulated.acc_rbi ) < STAT_CAP_AMOUNT &&
                    byte2int( players_file[i].filestats.filebatting.simulated.acc_so  ) < STAT_CAP_AMOUNT    )
               {
                    expected_act = players_file[i].filestats.filebatting.simulated;
                    expected_sim = players_file[i].filestats.filebatting.action;
                    player_idx   = i;

                    break;
               }
          }
     }

     formatAmigaToDos( players_file );

     compareBattingStats( &expected_act, &(players_file[player_idx].filestats.filebatting.action)    );
     compareBattingStats( &expected_sim, &(players_file[player_idx].filestats.filebatting.simulated) );

     return NULL;
}

static char *formatAmigaToDos_ShouldCapRBIAndMoveExcessFromSimulatedToAction_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayers();
     int           player_idx   = 0;

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               int2byte( players_file[i].filestats.filebatting.simulated.acc_rbi,  247 );
               int2byte( players_file[i].filestats.filebatting.action.acc_rbi,       0 );

               player_idx = i;

               break;
          }
     }

     formatAmigaToDos( players_file );

     assertEquals( 225, byte2int( players_file[player_idx].filestats.filebatting.action.acc_rbi    ) );
     assertEquals(  22, byte2int( players_file[player_idx].filestats.filebatting.simulated.acc_rbi ) );

     return NULL;
}

static char *formatAmigaToDos_ShouldCapStrikeOutsAndMoveExcessFromSimulatedToAction_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayers();
     int           player_idx   = 0;

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_file[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               int2byte( players_file[i].filestats.filebatting.simulated.acc_so,  229 );
               int2byte( players_file[i].filestats.filebatting.action.acc_so,       0 );

               player_idx = i;

               break;
          }
     }

     formatAmigaToDos( players_file );

     assertEquals( 225, byte2int( players_file[player_idx].filestats.filebatting.action.acc_so    ) );
     assertEquals(   4, byte2int( players_file[player_idx].filestats.filebatting.simulated.acc_so ) );

     return NULL;
}

static void run_all_tests()
{
     run_test( formatAmigaToDos_ShouldMovePitcherSimulatedStartsToActionGames_GivenAPlayersFile,         null );
     run_test( formatAmigaToDos_ShouldSwapBatterSimulatedWithAction_GivenAPlayersFile,                   null );
     run_test( formatAmigaToDos_ShouldCapRBIAndMoveExcessFromSimulatedToAction_GivenAPlayersFile,        null );
     run_test( formatAmigaToDos_ShouldCapStrikeOutsAndMoveExcessFromSimulatedToAction_GivenAPlayersFile, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
