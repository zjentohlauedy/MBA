#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_batter_data( void *output, int cols, char *data[], char *names[] )
{
     static batter_s batter;

     batter_s **b = (batter_s **)output;

     if ( cols < 9 ) return SQLITE_ERROR;

     memset( &batter, '\0', sizeof(batter_s) );

     batter.player_id          = atoi( data[0] );
     batter.power              = atoi( data[1] );
     batter.hit_n_run          = atoi( data[2] );
     batter.bunt               = atoi( data[3] );
     batter.running            = atoi( data[4] );
     batter.range              = atoi( data[5] );
     batter.arm                = atoi( data[6] );
     batter.primary_position   = atoi( data[7] );
     batter.secondary_position = atoi( data[8] );

     *b = &batter;

     return SQLITE_OK;
}

static batter_s *get_a_batter( int player_id )
{
     batter_s *batter = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from batters_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_batter_data, &batter, NULL );

     return batter;
}


static void insert_a_batter( batter_s *batter )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into batters_t (player_id, power, hit_n_run, bunt, running, range, arm, "
               "primary_position, secondary_position)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d)",
               batter->player_id,
               batter->power,
               batter->hit_n_run,
               batter->bunt,
               batter->running,
               batter->range,
               batter->arm,
               batter->primary_position,
               batter->secondary_position );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *batters_t_create__ShouldInsertRecordsInTheBattersTTable()
{
     batter_s expected = { 0 };

     expected.player_id          = 123;
     expected.power              = 6;
     expected.hit_n_run          = 2;
     expected.bunt               = 5;
     expected.running            = 3;
     expected.range              = 1;
     expected.arm                = 8;
     expected.primary_position   = pos_SecondBase;
     expected.secondary_position = pos_ThirdBase;

     assertEquals( "batters_t_create()", SQLITE_OK, batters_t_create( db, &expected ) );

     batter_s *actual = get_a_batter( expected.player_id );

     assertEquals( "player_id",          expected.player_id,          actual->player_id          );
     assertEquals( "power",              expected.power,              actual->power              );
     assertEquals( "hit_n_run",          expected.hit_n_run,          actual->hit_n_run          );
     assertEquals( "bunt",               expected.bunt,               actual->bunt               );
     assertEquals( "running",            expected.running,            actual->running            );
     assertEquals( "range",              expected.range,              actual->range              );
     assertEquals( "arm",                expected.arm,                actual->arm                );
     assertEquals( "primary_position",   expected.primary_position,   actual->primary_position   );
     assertEquals( "secondary_position", expected.secondary_position, actual->secondary_position );

     sqlite3_exec( db, "delete from batters_t", NULL, NULL, NULL );

     return NULL;
}

static char *batters_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists()
{
     batter_s expected = { 0 };

     expected.player_id          = 123;
     expected.power              = 6;
     expected.hit_n_run          = 2;
     expected.bunt               = 5;
     expected.running            = 3;
     expected.range              = 1;
     expected.arm                = 8;
     expected.primary_position   = pos_SecondBase;
     expected.secondary_position = pos_ThirdBase;

     assertEquals( "batters_t_create()", SQLITE_OK, batters_t_create( db, &expected ) );

     expected.power              = 4;
     expected.hit_n_run          = 6;
     expected.bunt               = 3;
     expected.running            = 7;
     expected.range              = 5;
     expected.arm                = 2;
     expected.primary_position   = pos_Catcher;
     expected.secondary_position = pos_RightField;

     assertEquals( "batters_t_create()", SQLITE_CONSTRAINT, batters_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from batters_t", NULL, NULL, NULL );

     return NULL;
}

static char *batters_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     batter_s expected = { 0 };

     expected.player_id          = 123;
     expected.power              = 6;
     expected.hit_n_run          = 2;
     expected.bunt               = 5;
     expected.running            = 3;
     expected.range              = 1;
     expected.arm                = 8;
     expected.primary_position   = pos_SecondBase;
     expected.secondary_position = pos_ThirdBase;

     insert_a_batter( &expected );

     batter_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( "batters_t_read()", SQLITE_OK, batters_t_read( db, &actual ) );

     assertEquals( "player_id",          expected.player_id,          actual.player_id          );
     assertEquals( "power",              expected.power,              actual.power              );
     assertEquals( "hit_n_run",          expected.hit_n_run,          actual.hit_n_run          );
     assertEquals( "bunt",               expected.bunt,               actual.bunt               );
     assertEquals( "running",            expected.running,            actual.running            );
     assertEquals( "range",              expected.range,              actual.range              );
     assertEquals( "arm",                expected.arm,                actual.arm                );
     assertEquals( "primary_position",   expected.primary_position,   actual.primary_position   );
     assertEquals( "secondary_position", expected.secondary_position, actual.secondary_position );

     sqlite3_exec( db, "delete from batters_t", NULL, NULL, NULL );

     return NULL;
}

static char *batters_t_update__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     batter_s expected = { 0 };

     expected.player_id          = 123;
     expected.power              = 6;
     expected.hit_n_run          = 2;
     expected.bunt               = 5;
     expected.running            = 3;
     expected.range              = 1;
     expected.arm                = 8;
     expected.primary_position   = pos_SecondBase;
     expected.secondary_position = pos_ThirdBase;

     insert_a_batter( &expected );

     expected.power              = 4;
     expected.hit_n_run          = 6;
     expected.bunt               = 3;
     expected.running            = 7;
     expected.range              = 5;
     expected.arm                = 2;
     expected.primary_position   = pos_Catcher;
     expected.secondary_position = pos_RightField;

     assertEquals( "batters_t_update()", SQLITE_OK, batters_t_update( db, &expected ) );

     batter_s *actual = get_a_batter( expected.player_id );

     assertEquals( "player_id",          expected.player_id,          actual->player_id          );
     assertEquals( "power",              expected.power,              actual->power              );
     assertEquals( "hit_n_run",          expected.hit_n_run,          actual->hit_n_run          );
     assertEquals( "bunt",               expected.bunt,               actual->bunt               );
     assertEquals( "running",            expected.running,            actual->running            );
     assertEquals( "range",              expected.range,              actual->range              );
     assertEquals( "arm",                expected.arm,                actual->arm                );
     assertEquals( "primary_position",   expected.primary_position,   actual->primary_position   );
     assertEquals( "secondary_position", expected.secondary_position, actual->secondary_position );

     sqlite3_exec( db, "delete from batters_t", NULL, NULL, NULL );

     return NULL;
}

static char *batters_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     batter_s expected = { 0 };

     expected.player_id          = 123;
     expected.power              = 6;
     expected.hit_n_run          = 2;
     expected.bunt               = 5;
     expected.running            = 3;
     expected.range              = 1;
     expected.arm                = 8;
     expected.primary_position   = pos_SecondBase;
     expected.secondary_position = pos_ThirdBase;

     insert_a_batter( &expected );

     assertEquals( "batters_t_delete()", SQLITE_OK, batters_t_delete( db, &expected ) );

     assertNull( "get_a_batter()", get_a_batter( expected.player_id ) );

     sqlite3_exec( db, "delete from batters_t", NULL, NULL, NULL );

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
     run_test( batters_t_create__ShouldInsertRecordsInTheBattersTTable,       check_sqlite_error );
     run_test( batters_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists,    check_sqlite_error );
     run_test( batters_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( batters_t_update__ShouldModifyMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( batters_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId, check_sqlite_error );
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
