#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static void clearLongevity( fileplayer_s *players_data )
{
     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          int pos = nibble( players_data[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               players_data[i].filestats.filebatting.ratings[2] &= 0xF0;
          }
     }
}

static int getLongevity( fileplayer_s *players_data )
{
     int pos = nibble( players_data->position[0], n_High );

     if ( pos != fpos_Pitcher )
     {
          return nibble( players_data->filestats.filebatting.ratings[2], n_Low );
     }

     return 0;
}


static char *copyLongevity_ShouldReturnTrueOnSuccess_GivenTwoPlayersFiles()
{
     fileplayer_s players_data1[TOTAL_PLAYERS] = { 0 };
     fileplayer_s players_data2[TOTAL_PLAYERS] = { 0 };

     assertEquals( bl_True, copyLongevity( players_data1, players_data2 ) );

     return NULL;
}

static char *copyLongevity_ShouldReturnFalseIfPlayerNotFound_GivenTwoPlayersFiles()
{
     fileplayer_s *players_data1                = buildFilePlayers();
     fileplayer_s  players_data2[TOTAL_PLAYERS] = { 0 };

     assertEquals( bl_False, copyLongevity( players_data1, players_data2 ) );

     return NULL;
}

static char *copyLongevity_ShouldCopyLongevityBetweenBatters_GivenTwoPlayersFiles()
{
     fileplayer_s  players_data1[TOTAL_PLAYERS] = { 0 };
     fileplayer_s *players_data2                = buildFilePlayers();

     memcpy( players_data1, players_data2, sizeof(players_data1) );

     clearLongevity( players_data1 );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i ) assertEquals( 0, getLongevity( &players_data1[i] ) );

     assertEquals( bl_True, copyLongevity( players_data1, players_data2 ) );

     for ( int i = 0; i < TOTAL_PLAYERS; ++i ) assertEquals( getLongevity( &players_data2[i] ), getLongevity( &players_data1[i] ) );

     return NULL;
}

static void get_error_message()
{
     printf( "copyLongevity error message: %s\n", getCopyLongevityError() );
}

static void run_all_tests()
{
     run_test( copyLongevity_ShouldReturnTrueOnSuccess_GivenTwoPlayersFiles,         get_error_message );
     run_test( copyLongevity_ShouldReturnFalseIfPlayerNotFound_GivenTwoPlayersFiles, get_error_message );
     run_test( copyLongevity_ShouldCopyLongevityBetweenBatters_GivenTwoPlayersFiles, get_error_message );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
