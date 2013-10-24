#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static int getPlayerId( fileplayer_s *players_file )
{
     unsigned char pos = nibble( players_file->position[0], n_High );

     if ( pos == fpos_Pitcher )
     {
          acc_player_id_s *player_id_data = (acc_player_id_s *)&(players_file->filestats.filepitching.action);

          return word2int( player_id_data->player_id );
     }
     else
     {
          acc_player_id_s *player_id_data = (acc_player_id_s *)&(players_file->filestats.filebatting.action);

          return word2int( player_id_data->player_id );
     }
}


static char *genPlayerIds_ShouldReturnTrueIfSuccessful_GivenAPlayersFile()
{
     fileplayer_s players_file[TOTAL_PLAYERS] = { 0 };

     assertEquals( bl_True, genPlayerIds( players_file ) );

     return NULL;
}

static char *genPlayerIds_ShouldReturnFalseIfDataAlreadyExistsInPlayerIdField_GivenAPlayersFile()
{
     fileplayer_s players_file[TOTAL_PLAYERS] = { 0 };

     strcpy( players_file[0].last_name, "Test" );

     players_file[0].filestats.filepitching.action.acc_wins[0] = 1;

     assertEquals( bl_False, genPlayerIds( players_file ) );

     return NULL;
}

static char *genPlayerIds_ShouldSetThePlayerIdsOnPlayers_GivenAPlayersFile()
{
     fileplayer_s *players_file = buildFilePlayersWithoutIds();

     assertEquals( bl_True, genPlayerIds( players_file ) );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          assertEquals( i + 1, getPlayerId( &players_file[i] ) );
     }

     return NULL;
}

static void run_all_tests()
{
     run_test( genPlayerIds_ShouldReturnTrueIfSuccessful_GivenAPlayersFile,                        null );
     run_test( genPlayerIds_ShouldReturnFalseIfDataAlreadyExistsInPlayerIdField_GivenAPlayersFile, null );
     run_test( genPlayerIds_ShouldSetThePlayerIdsOnPlayers_GivenAPlayersFile,                      null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
