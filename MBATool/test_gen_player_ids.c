#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


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

     players_file[0].acc_stats.amiga.action.pitching.acc_wins[0] = 1;

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

static void get_error_message()
{
     printf( "genPlayerIds error message: %s\n", getGenPlayerIdsError() );
}

static void run_all_tests()
{
     run_test( genPlayerIds_ShouldReturnTrueIfSuccessful_GivenAPlayersFile,                        get_error_message );
     run_test( genPlayerIds_ShouldReturnFalseIfDataAlreadyExistsInPlayerIdField_GivenAPlayersFile, get_error_message );
     run_test( genPlayerIds_ShouldSetThePlayerIdsOnPlayers_GivenAPlayersFile,                      get_error_message );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
