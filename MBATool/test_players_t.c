#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static void insert_a_player( player_s *player )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into players_t (player_id, first_name, last_name, "
               "first_phonetic, last_phonetic, skin_tone, handedness, player_type, rookie_season, longevity)"
               "values (%d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d )",
               player->player_id,
               player->first_name,
               player->last_name,
               player->first_phonetic,
               player->last_phonetic,
               player->skin_tone,
               player->handedness,
               player->player_type,
               player->rookie_season,
               player->longevity );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


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

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *get_player__ShouldRetrieveMatchingRecord_GivenThePlayerId()
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

     insert_a_player( &expected );

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

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *update_player__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     player_s expected;

     char query[999+1];

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

     insert_a_player( &expected );

     strcpy( expected.first_name,      "FirstName2" );
     strcpy( expected.last_name,       "LastName2"  );
     strcpy( expected.first_phonetic,  "FirstPho2"  );
     strcpy( expected.last_phonetic,   "LastPho2"   );
     /**/    expected.skin_tone       = st_Dark;
     /**/    expected.handedness      = hnd_Left;
     /**/    expected.player_type     = pt_Hitter;
     /**/    expected.rookie_season   = 4;
     /**/    expected.longevity       = 2;

     assertEquals( "update_player()", SQLITE_OK, update_player( db, &expected ) );

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

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *delete_player__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     player_s expected;

     char query[999+1];

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

     insert_a_player( &expected );

     assertEquals( "delete_player()", SQLITE_OK, delete_player( db, &expected ) );

     assertEquals( "get_player()", SQLITE_DONE, get_player( db, &expected ) );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static void check_sqlite_error()
{
     if ( sqlite3_errcode( db ) != 0 )
     {
          printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
     }
}

static char *run_all_tests()
{
     run_test( insert_player__ShouldInsertRecordsInThePlayersTTable,       check_sqlite_error );
     run_test( get_player__ShouldRetrieveMatchingRecord_GivenThePlayerId,  check_sqlite_error );
     run_test( update_player__ShouldModifyMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( delete_player__ShouldDeleteMatchingRecord_GivenThePlayerId, check_sqlite_error );
     return NULL;
}


int main( int argc, char *argv[] )
{
     sqlite3_open( "mba.db", &db );

     run_all_tests( db );

     show_test_results();

     sqlite3_close( db );

     return (result == NULL) ? EXIT_SUCCESS : EXIT_FAILURE;
}
