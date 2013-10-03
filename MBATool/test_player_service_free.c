#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static      player_accolade_s player_accolade_sentinel = PLAYER_ACCOLADE_SENTINEL;
static      pitcher_stats_s pitcher_stats_sentinel = PITCHER_STATS_SENTINEL;
static      pitcher_accolade_s pitcher_accolade_sentinel = PITCHER_ACCOLADE_SENTINEL;
static      batter_stats_s batter_stats_sentinel = BATTER_STATS_SENTINEL;
static      batter_accolade_s batter_accolade_sentinel = BATTER_ACCOLADE_SENTINEL;


static sqlite3 *db;
static char    *result;


static char *free_player__ShouldFreeTheAllocatedMemory_GivenAPlayerObject()
{
     return NULL;
}

static void run_all_tests()
{
     run_test( free_player__ShouldFreeTheAllocatedMemory_GivenAPlayerObject, null );
}

int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a db file name.\n" );

          return EXIT_FAILURE;
     }

     sqlite3_open( argv[1], &db );
     sqlite3_exec( db, "begin", NULL, NULL, NULL );


     run_all_tests();

     show_test_results();


     sqlite3_exec( db, "rollback", NULL, NULL, NULL );
     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
