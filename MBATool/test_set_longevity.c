#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "unit_test.h"


static char *result;


static char *setLongevity_ShouldSetBatterLongevity_GivenAPlayersFile()
{
     fileplayer_s players_data[TOTAL_PLAYERS] = { 0 };

     strcpy( players_data[0].last_name, "Name" );
     players_data[0].position[0] = 0x50;

     setLongevity( players_data );

     assertNonZero( nibble( players_data[0].filestats.filebatting.ratings[2], n_Low ) );

     return NULL;
}

static void run_all_tests()
{
     run_test( setLongevity_ShouldSetBatterLongevity_GivenAPlayersFile, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
