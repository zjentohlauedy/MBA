#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_pitcher_data( void *output, int cols, char *data[], char *names[] )
{
     pitcher_s *p = (pitcher_s *)output;

     if ( cols < 5 ) return SQLITE_ERROR;

     /**/    p->player_id = atoi( data[0] );
     /**/    p->speed     = atoi( data[1] );
     /**/    p->control   = atoi( data[2] );
     /**/    p->bunt      = atoi( data[3] );
     /**/    p->fatigue   = atoi( data[4] );

     return SQLITE_OK;
}

static pitcher_s *get_a_pitcher( int player_id )
{
     static pitcher_s pitcher;

     char query[999+1];

     memset( &pitcher, '\0', sizeof(pitcher_s) );

     snprintf( query, sizeof(query), "select * from pitchers_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_pitcher_data, &pitcher, NULL );

     return &pitcher;
}


static void insert_a_pitcher( pitcher_s *pitcher )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into pitchers_t (player_id, speed, control, bunt, fatigue)"
               "values (%d, %d, %d, %d, %d)",
               pitcher->player_id,
               pitcher->speed,
               pitcher->control,
               pitcher->bunt,
               pitcher->fatigue );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *pitchers_t_create__ShouldInsertRecordsInThePitchersTTable()
{
     pitcher_s expected = { 0 };

     expected.player_id = 123;
     expected.speed     = 5;
     expected.control   = 7;
     expected.bunt      = 3;
     expected.fatigue   = 8;

     assertEquals( "pitchers_t_create()", SQLITE_OK, pitchers_t_create( db, &expected ) );

     pitcher_s *actual = get_a_pitcher( expected.player_id );

     assertEquals( "player_id", expected.player_id, actual->player_id );
     assertEquals( "speed",     expected.speed,     actual->speed     );
     assertEquals( "control",   expected.control,   actual->control   );
     assertEquals( "bunt",      expected.bunt,      actual->bunt      );
     assertEquals( "fatigue",   expected.fatigue,   actual->fatigue   );

     sqlite3_exec( db, "delete from pitchers_t", NULL, NULL, NULL );

     return NULL;
}

static char *pitchers_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     pitcher_s expected = { 0 };

     expected.player_id = 123;
     expected.speed     = 2;
     expected.control   = 4;
     expected.bunt      = 6;
     expected.fatigue   = 8;

     insert_a_pitcher( &expected );

     pitcher_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( "pitchers_t_read()", SQLITE_OK, pitchers_t_read( db, &actual ) );

     assertEquals( "player_id", expected.player_id, actual.player_id );
     assertEquals( "speed",     expected.speed,     actual.speed     );
     assertEquals( "control",   expected.control,   actual.control   );
     assertEquals( "bunt",      expected.bunt,      actual.bunt      );
     assertEquals( "fatigue",   expected.fatigue,   actual.fatigue   );

     sqlite3_exec( db, "delete from pitchers_t", NULL, NULL, NULL );

     return NULL;
}

static char *pitchers_t_update__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     pitcher_s expected = { 0 };

     expected.player_id = 123;
     expected.speed     = 1;
     expected.control   = 1;
     expected.bunt      = 1;
     expected.fatigue   = 1;

     insert_a_pitcher( &expected );

     expected.speed     = 6;
     expected.control   = 9;
     expected.bunt      = 6;
     expected.fatigue   = 9;

     assertEquals( "pitchers_t_update()", SQLITE_OK, pitchers_t_update( db, &expected ) );

     pitcher_s *actual = get_a_pitcher( expected.player_id );

     assertEquals( "player_id", expected.player_id, actual->player_id );
     assertEquals( "speed",     expected.speed,     actual->speed     );
     assertEquals( "control",   expected.control,   actual->control   );
     assertEquals( "bunt",      expected.bunt,      actual->bunt      );
     assertEquals( "fatigue",   expected.fatigue,   actual->fatigue   );

     sqlite3_exec( db, "delete from pitchers_t", NULL, NULL, NULL );

     return NULL;
}

static char *pitchers_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     pitcher_s expected = { 0 };

     expected.player_id = 123;
     expected.speed     = 1;
     expected.control   = 1;
     expected.bunt      = 1;
     expected.fatigue   = 1;

     insert_a_pitcher( &expected );

     assertEquals( "pitchers_t_delete()", SQLITE_OK, pitchers_t_delete( db, &expected ) );

     pitcher_s *actual = get_a_pitcher( expected.player_id );

     assertEquals( "player_id", 0, actual->player_id );
     assertEquals( "speed",     0, actual->speed     );
     assertEquals( "control",   0, actual->control   );
     assertEquals( "bunt",      0, actual->bunt      );
     assertEquals( "fatigue",   0, actual->fatigue   );

     sqlite3_exec( db, "delete from pitchers_t", NULL, NULL, NULL );

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
     run_test( pitchers_t_create__ShouldInsertRecordsInThePitchersTTable,      check_sqlite_error );
     run_test( pitchers_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( pitchers_t_update__ShouldModifyMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( pitchers_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId, check_sqlite_error );
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
