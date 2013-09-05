#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_league_division_data( void *output, int cols, char *data[], char *names[] )
{
     static league_division_s league_division;

     league_division_s **ld = (league_division_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &league_division, '\0', sizeof(league_division_s) );

     league_division.league_id   = atoi( data[0] );
     league_division.division_id = atoi( data[1] );

     *ld = &league_division;

     return SQLITE_OK;
}

static league_division_s *get_a_league_division( int league_id )
{
     league_division_s *league_division = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from league_divisions_t where league_id = %d", league_id );

     sqlite3_exec( db, query, load_league_division_data, &league_division, NULL );

     return league_division;
}

static void insert_a_league_division( league_division_s *league_division )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into league_divisions_t (league_id, division_id) values (%d, %d)",
               league_division->league_id,
               league_division->division_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *league_divisions_t_create__ShouldInsertRecordsInTheLeagueDivisionsTTable()
{
     league_division_s expected = { 0 };

     expected.league_id   = 5;
     expected.division_id = 8;

     assertEquals( "league_divisions_t_create()", SQLITE_OK, league_divisions_t_create( db, &expected ) );

     league_division_s *actual = get_a_league_division( expected.league_id );

     assertEquals( "league_id",   expected.league_id,   actual->league_id   );
     assertEquals( "division_id", expected.division_id, actual->division_id );

     sqlite3_exec( db, "delete from league_divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_divisions_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     league_division_s expected = { 0 };

     expected.league_id   = 5;
     expected.division_id = 8;

     assertEquals( "league_divisions_t_create()", SQLITE_OK,         league_divisions_t_create( db, &expected ) );
     assertEquals( "league_divisions_t_create()", SQLITE_CONSTRAINT, league_divisions_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from league_divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_divisions_t_read_by_division__ShouldRetrieveMatchingRecord_GivenTheLeagueId()
{
     league_division_s expected = { 0 };

     expected.league_id   = 5;
     expected.division_id = 8;

     insert_a_league_division( &expected );

     data_list_s list = { 0 };

     assertEquals( "league_divisions_t_read_by_league()", SQLITE_OK, league_divisions_t_read_by_league( db, expected.league_id, &list ) );

     assertEquals( "count", 1, list.count );

     league_division_s *actual = list.data;

     assertEquals( "league_id",   expected.league_id,   actual->league_id   );
     assertEquals( "division_id", expected.division_id, actual->division_id );

     sqlite3_exec( db, "delete from league_divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_divisions_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheLeagueId()
{
     league_division_s expected1 = { 0 };
     league_division_s expected2 = { 0 };

     expected1.league_id   = 5;
     expected1.division_id = 3;

     expected2.league_id   = 5;
     expected2.division_id = 8;

     insert_a_league_division( &expected1 );
     insert_a_league_division( &expected2 );

     data_list_s list = { 0 };

     assertEquals( "league_divisions_t_read_by_league()", SQLITE_OK, league_divisions_t_read_by_league( db, expected1.league_id, &list ) );

     assertEquals( "count", 2, list.count );

     league_division_s *actual = list.data;

     assertEquals( "league_id",   expected1.league_id,   actual[0].league_id   );
     assertEquals( "division_id", expected1.division_id, actual[0].division_id );

     assertEquals( "league_id",   expected2.league_id,   actual[1].league_id   );
     assertEquals( "division_id", expected2.division_id, actual[1].division_id );

     sqlite3_exec( db, "delete from league_divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_divisions_t_delete__ShouldDeleteMatchingRecord_GivenALeagueDivision()
{
     league_division_s expected = { 0 };

     expected.league_id   = 5;
     expected.division_id = 8;

     insert_a_league_division( &expected );

     assertEquals( "league_divisions_t_delete()", SQLITE_OK, league_divisions_t_delete( db, &expected ) );

     league_division_s *actual = get_a_league_division( expected.league_id );

     assertNull( "get_a_league_division()", get_a_league_division( expected.league_id ) );

     sqlite3_exec( db, "delete from league_divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_divisions_t_delete__ShouldOnlyDeleteMatchingRecord_GivenALeagueDivision()
{
     league_division_s expected1 = { 0 };
     league_division_s expected2 = { 0 };

     expected1.league_id   = 5;
     expected1.division_id = 3;

     expected2.league_id   = 5;
     expected2.division_id = 8;

     insert_a_league_division( &expected1 );
     insert_a_league_division( &expected2 );

     assertEquals( "league_divisions_t_delete()", SQLITE_OK, league_divisions_t_delete( db, &expected2 ) );

     league_division_s *actual = get_a_league_division( expected1.league_id );

     assertEquals( "league_id",   expected1.league_id,   actual->league_id   );
     assertEquals( "division_id", expected1.division_id, actual->division_id );

     sqlite3_exec( db, "delete from league_divisions_t", NULL, NULL, NULL );

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
     run_test( league_divisions_t_create__ShouldInsertRecordsInTheLeagueDivisionsTTable,               check_sqlite_error );
     run_test( league_divisions_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,                check_sqlite_error );
     run_test( league_divisions_t_read_by_division__ShouldRetrieveMatchingRecord_GivenTheLeagueId,     check_sqlite_error );
     run_test( league_divisions_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheLeagueId, check_sqlite_error );
     run_test( league_divisions_t_delete__ShouldDeleteMatchingRecord_GivenALeagueDivision,             check_sqlite_error );
     run_test( league_divisions_t_delete__ShouldOnlyDeleteMatchingRecord_GivenALeagueDivision,         check_sqlite_error );
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
