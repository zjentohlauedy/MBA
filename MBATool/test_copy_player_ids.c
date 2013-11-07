#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static char *copyPlayerIds_ShouldReturnTrueOnSuccess_GivenTwoPlayerFiles()
{
     fileplayer_s players_file1[TOTAL_PLAYERS] = { 0 };
     fileplayer_s players_file2[TOTAL_PLAYERS] = { 0 };

     assertEquals( bl_True, copyPlayerIds( players_file1, players_file2 ) );

     return NULL;
}

static char *copyPlayerIds_ShouldReturnFalseOnError_GivenTwoPlayerFiles()
{
     fileplayer_s *players_file1 = buildFilePlayers();
     fileplayer_s *players_file2 = buildFilePlayers();

     assertEquals( bl_False, copyPlayerIds( players_file1, players_file2 ) );

     return NULL;
}

static char *copyPlayerIds_ShouldCopyThePlayerIdFromOneFileToAnotherBasedOnName_GivenTwoPlayerFiles()
{
     fileplayer_s  players_file1[TOTAL_PLAYERS] = { 0 };
     fileplayer_s *players_file2                = buildFilePlayersWithoutIds();

     int players[10] = { 0 };

     for ( int i = 0; i < 10; ++i ) players[i] = rand() % 800;

     for ( int i = 0; i < 10; ++i ) memcpy( &players_file1[i], &players_file2[players[i]], sizeof(fileplayer_s) );

     fileplayer_s *players_file3 = buildFilePlayers();

     assertEquals( bl_True, copyPlayerIds( players_file1, players_file3 ) );

     for ( int i = 0; i < 10; ++i ) assertEquals( getPlayerId( &players_file3[players[i]] ), getPlayerId( &players_file1[i] ) );

     return NULL;
}

static void get_error_message()
{
     printf( "copyPlayerIds error message: %s\n", getCopyPlayerIdsError() );
}

static void run_all_tests()
{
     run_test( copyPlayerIds_ShouldReturnTrueOnSuccess_GivenTwoPlayerFiles,                            get_error_message );
     run_test( copyPlayerIds_ShouldReturnFalseOnError_GivenTwoPlayerFiles,                             get_error_message );
     run_test( copyPlayerIds_ShouldCopyThePlayerIdFromOneFileToAnotherBasedOnName_GivenTwoPlayerFiles, get_error_message );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
