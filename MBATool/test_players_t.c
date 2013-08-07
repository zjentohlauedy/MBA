#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static char *insert_player__ShouldInsertRecordsInThePlayersTTable()
{
     player_s expected;

     /**/    expected.player_id       = 1;
     strcpy( expected.first_name,      "FirstName1" );
     strcpy( expected.last_name,       "LastName1"  );
     strcpy( expected.first_phonetic,  "FirstPho1"  );
     strcpy( expected.last_phonetic,   "LastPho1"   );
     /**/    expected.skin_tone       = st_Light;
     /**/    expected.handedness      = hnd_Right;
     /**/    expected.player_type     = pt_Pitcher;
     /**/    expected.rookie_season   = 1;
     /**/    expected.longevity       = 5;

     assertEquals( "insert_player()", SQLITE_OK, insert_player( db, &expected ) );

     player_s actual;

     actual.player_id = 1;

     assertEquals( "get_player()", SQLITE_OK, get_player( db, &actual ) );

     assertEquals(    "player_id",      expected.player_id,      actual.player_id      );
     assertEqualsStr( "first_name",     expected.first_name,     actual.first_name     );
     assertEqualsStr( "last_name",      expected.last_name,      actual.last_name      );
     assertEqualsStr( "first_phonetic", expected.first_phonetic, actual.first_phonetic );
     assertEqualsStr( "last_phonetic",  expected.last_phonetic,  actual.last_phonetic  );
     assertEquals(    "skin_tone",      expected.skin_tone,      actual.skin_tone      );
     assertEquals(    "handedness",     expected.handedness,     actual.handedness     );
     assertEquals(    "player_type",    expected.player_type,    actual.player_type    );
     assertEquals(    "rookie_season",  expected.rookie_season,  actual.rookie_season  );
     assertEquals(    "longevity",      expected.longevity,      actual.longevity      );

     return NULL;
}

static char *get_player__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     return NULL;
}

static char *update_player__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     return NULL;
}

static char *delete_player__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     return NULL;
}

static char *all_tests()
{
     run_test( insert_player__ShouldInsertRecordsInThePlayersTTable       );
     run_test( get_player__ShouldRetrieveMatchingRecord_GivenThePlayerId  );
     run_test( update_player__ShouldModifyMatchingRecord_GivenThePlayerId );
     run_test( delete_player__ShouldDeleteMatchingRecord_GivenThePlayerId );
     return NULL;
}


int main( int argc, char *argv[] )
{
     sqlite3_open( "mba.db", &db );

     if ( (result = all_tests( db )) != NULL )
     {
          printf( "%s\n", result );

          if ( sqlite3_errcode( db ) != 0 )
          {
               printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
          }
     }
     else
     {
          tests_passed();
     }

/*
     strcpy( player1.first_name,  "Doug"  );
     strcpy( player1.last_name,   "Guod"  );
     /oo/    player1.handedness  = hnd_Left;

     if ( update_player( db, &player1 ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't update player1: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }


     player.player_id = 1;

     if ( get_player( db, &player ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't get player 1: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     print_player( &player );


     player.player_id = 2;

     if ( delete_player( db, &player ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't delete player 2: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }
*/

     sqlite3_close( db );

     return (result == NULL) ? EXIT_SUCCESS : EXIT_FAILURE;
}
