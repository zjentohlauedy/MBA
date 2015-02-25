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
     static player_s player;

     player_s **p = (player_s **)output;

     if ( cols < 10 ) return SQLITE_ERROR;

     memset( &player, '\0', sizeof(player_s) );

     /**/    player.player_id        = atoi( data[0] );
     strcpy( player.first_name,              data[1] );
     strcpy( player.last_name,               data[2] );
     strcpy( player.first_phoenetic,         data[3] );
     strcpy( player.last_phoenetic,          data[4] );
     /**/    player.skin_tone        = atoi( data[5] );
     /**/    player.handedness       = atoi( data[6] );
     /**/    player.player_type      = atoi( data[7] );
     /**/    player.rookie_season    = atoi( data[8] );
     /**/    player.longevity        = atoi( data[9] );

     *p = &player;

     return SQLITE_OK;
}

static player_s *get_a_player( int player_id )
{
     player_s *player = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from players_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_data, &player, NULL );

     return player;
}


static void insert_a_player( player_s *player )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into players_t (player_id, first_name, last_name, "
               "first_phoenetic, last_phoenetic, skin_tone, handedness, player_type, rookie_season, longevity)"
               "values (%d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d)",
               player->player_id,
               player->first_name,
               player->last_name,
               player->first_phoenetic,
               player->last_phoenetic,
               player->skin_tone,
               player->handedness,
               player->player_type,
               player->rookie_season,
               player->longevity );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *players_t_create__ShouldInsertRecordsInThePlayersTTable()
{
     player_s expected = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     player_s *actual = get_a_player( expected.player_id );

     assertEqualsInt( expected.player_id,       actual->player_id       );
     assertEqualsStr( expected.first_name,      actual->first_name      );
     assertEqualsStr( expected.last_name,       actual->last_name       );
     assertEqualsStr( expected.first_phoenetic, actual->first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual->last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual->skin_tone       );
     assertEqualsInt( expected.handedness,      actual->handedness      );
     assertEqualsInt( expected.player_type,     actual->player_type     );
     assertEqualsInt( expected.rookie_season,   actual->rookie_season   );
     assertEqualsInt( expected.longevity,       actual->longevity       );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists()
{
     player_s expected = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName2" );
     strcpy( expected.last_name,        "LastName2"  );
     strcpy( expected.first_phoenetic,  "FirstPho2"  );
     strcpy( expected.last_phoenetic,   "LastPho2"   );
     /**/    expected.skin_tone        = st_Dark;
     /**/    expected.handedness       = hnd_Left;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 3;
     /**/    expected.longevity        = 3;


     assertEquals( SQLITE_CONSTRAINT, players_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     insert_a_player( &expected );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( expected.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_update__ShouldModifyMatchingRecord_GivenThePlayer()
{
     player_s expected = { 0 };


     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     insert_a_player( &expected );

     strcpy( expected.first_name,       "FirstName2" );
     strcpy( expected.last_name,        "LastName2"  );
     strcpy( expected.first_phoenetic,  "FirstPho2"  );
     strcpy( expected.last_phoenetic,   "LastPho2"   );
     /**/    expected.skin_tone        = st_Dark;
     /**/    expected.handedness       = hnd_Left;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 4;
     /**/    expected.longevity        = 2;

     assertEquals( SQLITE_OK, players_t_update( db, &expected ) );

     player_s *actual = get_a_player( expected.player_id );

     assertEqualsInt( expected.player_id,       actual->player_id      );
     assertEqualsStr( expected.first_name,      actual->first_name     );
     assertEqualsStr( expected.last_name,       actual->last_name      );
     assertEqualsStr( expected.first_phoenetic, actual->first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual->last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual->skin_tone      );
     assertEqualsInt( expected.handedness,      actual->handedness     );
     assertEqualsInt( expected.player_type,     actual->player_type    );
     assertEqualsInt( expected.rookie_season,   actual->rookie_season  );
     assertEqualsInt( expected.longevity,       actual->longevity      );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_update_phoenetics__ShouldModifyOnlyPhoeneticsOfMatchingRecord_GivenThePlayer()
{
     player_s expected = { 0 };
     player_s modified = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     insert_a_player( &expected );

     modified = expected;

     strcpy( modified.first_name,       "FirstName2" );
     strcpy( modified.last_name,        "LastName2"  );
     strcpy( modified.first_phoenetic,  "FirstPho2"  );
     strcpy( modified.last_phoenetic,   "LastPho2"   );
     /**/    modified.skin_tone        = st_Dark;
     /**/    modified.handedness       = hnd_Left;
     /**/    modified.player_type      = pt_Batter;
     /**/    modified.rookie_season    = 4;
     /**/    modified.longevity        = 2;

     assertEquals( SQLITE_OK, players_t_update_phoenetics( db, &modified ) );

     player_s *actual = get_a_player( expected.player_id );

     assertEqualsInt( expected.player_id,       actual->player_id      );
     assertEqualsStr( expected.first_name,      actual->first_name     );
     assertEqualsStr( expected.last_name,       actual->last_name      );
     assertEqualsStr( modified.first_phoenetic, actual->first_phoenetic );
     assertEqualsStr( modified.last_phoenetic,  actual->last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual->skin_tone      );
     assertEqualsInt( expected.handedness,      actual->handedness     );
     assertEqualsInt( expected.player_type,     actual->player_type    );
     assertEqualsInt( expected.rookie_season,   actual->rookie_season  );
     assertEqualsInt( expected.longevity,       actual->longevity      );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };


     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     insert_a_player( &expected );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );

     assertNull( get_a_player( expected.player_id ) );

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
     run_test( players_t_create__ShouldInsertRecordsInThePlayersTTable,                                check_sqlite_error );
     run_test( players_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists,                             check_sqlite_error );
     run_test( players_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId,                          check_sqlite_error );
     run_test( players_t_update__ShouldModifyMatchingRecord_GivenThePlayer,                            check_sqlite_error );
     run_test( players_t_update_phoenetics__ShouldModifyOnlyPhoeneticsOfMatchingRecord_GivenThePlayer, check_sqlite_error );
     run_test( players_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId,                          check_sqlite_error );
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


     run_all_tests( db );

     show_test_results();


     sqlite3_exec( db, "rollback", NULL, NULL, NULL );
     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
