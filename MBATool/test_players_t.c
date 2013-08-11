#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_player_data( void *output, int cols, char *data[], char *names[] )
{
     player_s *p = (player_s *)output;

     if ( cols < 10 ) return SQLITE_ERROR;

     /**/    p->player_id       = atoi( data[0] );
     strcpy( p->first_name,             data[1] );
     strcpy( p->last_name,              data[2] );
     strcpy( p->first_phonetic,         data[3] );
     strcpy( p->last_phonetic,          data[4] );
     /**/    p->skin_tone       = atoi( data[5] );
     /**/    p->handedness      = atoi( data[6] );
     /**/    p->player_type     = atoi( data[7] );
     /**/    p->rookie_season   = atoi( data[8] );
     /**/    p->longevity       = atoi( data[9] );

     return SQLITE_OK;
}

static player_s *get_a_player( int player_id )
{
     static player_s player;

     char query[999+1];

     memset( &player, '\0', sizeof(player_s) );

     snprintf( query, sizeof(query), "select * from players_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_data, &player, NULL );

     return &player;
}


static void insert_a_player( player_s *player )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into players_t (player_id, first_name, last_name, "
               "first_phonetic, last_phonetic, skin_tone, handedness, player_type, rookie_season, longevity)"
               "values (%d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d)",
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
     player_s expected = { 0 };

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

     player_s *actual = get_a_player( expected.player_id );

     assertEquals(    "player_id",      expected.player_id,      actual->player_id      );
     assertEqualsStr( "first_name",     expected.first_name,     actual->first_name     );
     assertEqualsStr( "last_name",      expected.last_name,      actual->last_name      );
     assertEqualsStr( "first_phonetic", expected.first_phonetic, actual->first_phonetic );
     assertEqualsStr( "last_phonetic",  expected.last_phonetic,  actual->last_phonetic  );
     assertEquals(    "skin_tone",      expected.skin_tone,      actual->skin_tone      );
     assertEquals(    "handedness",     expected.handedness,     actual->handedness     );
     assertEquals(    "player_type",    expected.player_type,    actual->player_type    );
     assertEquals(    "rookie_season",  expected.rookie_season,  actual->rookie_season  );
     assertEquals(    "longevity",      expected.longevity,      actual->longevity      );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *get_player__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };

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

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

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
     player_s expected = { 0 };


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

     player_s *actual = get_a_player( expected.player_id );

     assertEquals(    "player_id",      expected.player_id,      actual->player_id      );
     assertEqualsStr( "first_name",     expected.first_name,     actual->first_name     );
     assertEqualsStr( "last_name",      expected.last_name,      actual->last_name      );
     assertEqualsStr( "first_phonetic", expected.first_phonetic, actual->first_phonetic );
     assertEqualsStr( "last_phonetic",  expected.last_phonetic,  actual->last_phonetic  );
     assertEquals(    "skin_tone",      expected.skin_tone,      actual->skin_tone      );
     assertEquals(    "handedness",     expected.handedness,     actual->handedness     );
     assertEquals(    "player_type",    expected.player_type,    actual->player_type    );
     assertEquals(    "rookie_season",  expected.rookie_season,  actual->rookie_season  );
     assertEquals(    "longevity",      expected.longevity,      actual->longevity      );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *delete_player__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };


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

     player_s *actual = get_a_player( expected.player_id );

     assertEqualsStr( "first_name", "", actual->first_name );

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

static void run_all_tests()
{
     run_test( insert_player__ShouldInsertRecordsInThePlayersTTable,       check_sqlite_error );
     run_test( get_player__ShouldRetrieveMatchingRecord_GivenThePlayerId,  check_sqlite_error );
     run_test( update_player__ShouldModifyMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( delete_player__ShouldDeleteMatchingRecord_GivenThePlayerId, check_sqlite_error );
}


int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a db file name.\n" );

          return EXIT_FAILURE;
     }

     sqlite3_open( argv[1], &db );

     run_all_tests( db );

     show_test_results();

     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
