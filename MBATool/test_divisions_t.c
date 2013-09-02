#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_division_data( void *output, int cols, char *data[], char *names[] )
{
     static division_s division;

     division_s **d = (division_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &division, '\0', sizeof(division_s) );

     /**/    division.division_id = atoi( data[0] );
     strcpy( division.name,               data[1] );

     *d = &division;

     return SQLITE_OK;
}

static division_s *get_a_division( int division_id )
{
     division_s *division = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from divisions_t where division_id = %d", division_id );

     sqlite3_exec( db, query, load_division_data, &division, NULL );

     return division;
}

static void insert_a_division( division_s *division )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into divisions_t (division_id, name)"
               "values (%d, '%s')",
               division->division_id,
               division->name );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *divisions_t_create__ShouldInsertRecordsInTheDivisionsTTable()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 5;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( "divisions_t_create()", SQLITE_OK, divisions_t_create( db, &expected ) );

     division_s *actual = get_a_division( expected.division_id );

     assertEquals(    "division_id", expected.division_id, actual->division_id );
     assertEqualsStr( "name",        expected.name,        actual->name        );

     sqlite3_exec( db, "delete from divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *divisions_t_create__ShouldGiveAnErrorIfDivisionIdAlreadyExists()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 5;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( "divisions_t_create()", SQLITE_OK, divisions_t_create( db, &expected ) );

     strcpy( expected.name, "DivisionName2" );

     assertEquals( "divisions_t_create()", SQLITE_CONSTRAINT, divisions_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *divisions_t_read__ShouldRetrieveMatchingRecord_GivenTheDivisionId()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 5;
     strcpy( expected.name,        "DivisionName1" );

     insert_a_division( &expected );

     division_s actual = { 0 };

     actual.division_id = expected.division_id;

     assertEquals( "divisions_t_read()", SQLITE_OK, divisions_t_read( db, &actual ) );

     assertEquals(    "division_id", expected.division_id, actual.division_id );
     assertEqualsStr( "name",        expected.name,        actual.name        );

     sqlite3_exec( db, "delete from divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *divisions_t_update__ShouldModifyMatchingRecord_GivenTheDivisionId()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 5;
     strcpy( expected.name,        "DivisionName1" );

     insert_a_division( &expected );

     strcpy( expected.name, "DivisionName2" );

     assertEquals( "divisions_t_update()", SQLITE_OK, divisions_t_update( db, &expected ) );

     division_s *actual = get_a_division( expected.division_id );

     assertEquals(    "division_id", expected.division_id, actual->division_id );
     assertEqualsStr( "name",        expected.name,        actual->name        );

     sqlite3_exec( db, "delete from divisions_t", NULL, NULL, NULL );

     return NULL;
}

static char *divisions_t_delete__ShouldDeleteMatchingRecord_GivenTheDivisionId()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 5;
     strcpy( expected.name,        "DivisionName1" );

     insert_a_division( &expected );

     assertEquals( "divisions_t_delete()", SQLITE_OK, divisions_t_delete( db, &expected ) );

     assertEquals( "get_a_division()", NULL, get_a_division( expected.division_id ) );

     sqlite3_exec( db, "delete from divisions_t", NULL, NULL, NULL );

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
     run_test( divisions_t_create__ShouldInsertRecordsInTheDivisionsTTable,       check_sqlite_error );
     run_test( divisions_t_create__ShouldGiveAnErrorIfDivisionIdAlreadyExists,    check_sqlite_error );
     run_test( divisions_t_read__ShouldRetrieveMatchingRecord_GivenTheDivisionId, check_sqlite_error );
     run_test( divisions_t_update__ShouldModifyMatchingRecord_GivenTheDivisionId, check_sqlite_error );
     run_test( divisions_t_delete__ShouldDeleteMatchingRecord_GivenTheDivisionId, check_sqlite_error );
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
