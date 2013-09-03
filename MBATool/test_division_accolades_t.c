#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_division_accolade_data( void *output, int cols, char *data[], char *names[] )
{
     static division_accolade_s division_accolade;

     division_accolade_s **da = (division_accolade_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &division_accolade, '\0', sizeof(division_accolade_s) );

     division_accolade.division_id = atoi( data[0] );
     division_accolade.season      = atoi( data[1] );
     division_accolade.accolade    = atoi( data[2] );

     *da = &division_accolade;

     return SQLITE_OK;
}

static division_accolade_s *get_a_division_accolade( int division_id )
{
     division_accolade_s *division_accolade = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from division_accolades_t where division_id = %d", division_id );

     sqlite3_exec( db, query, load_division_accolade_data, &division_accolade, NULL );

     return division_accolade;
}

static void insert_a_division_accolade( division_accolade_s *division_accolade )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into division_accolades_t (division_id, season, accolade) values (%d, %d, %d)",
               division_accolade->division_id,
               division_accolade->season,
               division_accolade->accolade );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}

static char *division_accolades_t_create__ShouldInsertRecordsInTheDivisionAccoladesTTable()
{
     division_accolade_s expected = { 0 };

     expected.division_id = 123;
     expected.season      = 5;
     expected.accolade    = dacc_League_Title;

     assertEquals( "division_accolades_t_create()", SQLITE_OK, division_accolades_t_create( db, &expected ) );

     division_accolade_s *actual = get_a_division_accolade( expected.division_id );

     assertEquals( "division_id", expected.division_id, actual->division_id );
     assertEquals( "season",      expected.season,      actual->season      );
     assertEquals( "accolade",    expected.accolade,    actual->accolade    );

     sqlite3_exec( db, "delete from division_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     division_accolade_s expected = { 0 };

     expected.division_id = 123;
     expected.season      = 5;
     expected.accolade    = dacc_League_Title;

     assertEquals( "division_accolades_t_create()", SQLITE_OK,         division_accolades_t_create( db, &expected ) );
     assertEquals( "division_accolades_t_create()", SQLITE_CONSTRAINT, division_accolades_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from division_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_accolades_t_read_by_division__ShouldRetrieveMatchingRecord_GivenTheDivisionId()
{
     division_accolade_s expected = { 0 };

     expected.division_id = 123;
     expected.season      = 5;
     expected.accolade    = dacc_League_Title;

     insert_a_division_accolade( &expected );

     data_list_s list = { 0 };

     assertEquals( "division_accolades_t_read_by_division()", SQLITE_OK, division_accolades_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( "count", 1, list.count );

     division_accolade_s *actual = list.data;

     assertEquals( "division_id", expected.division_id, actual->division_id );
     assertEquals( "season",      expected.season,      actual->season      );
     assertEquals( "accolade",    expected.accolade,    actual->accolade    );

     free( actual );

     sqlite3_exec( db, "delete from division_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_accolades_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheDivisionId()
{
     division_accolade_s expected1 = { 0 };
     division_accolade_s expected2 = { 0 };

     expected1.division_id = 123;
     expected1.season      = 5;
     expected1.accolade    = dacc_League_Title;

     expected2.division_id = 123;
     expected2.season      = 6;
     expected2.accolade    = dacc_Best_Record;

     insert_a_division_accolade( &expected1 );
     insert_a_division_accolade( &expected2 );

     data_list_s list = { 0 };

     assertEquals( "division_accolades_t_read_by_division()", SQLITE_OK, division_accolades_t_read_by_division( db, expected1.division_id, &list ) );

     assertEquals( "count", 2, list.count );

     division_accolade_s *actual = list.data;

     assertEquals( "division_id", expected1.division_id, actual[0].division_id );
     assertEquals( "season",      expected1.season,      actual[0].season      );
     assertEquals( "accolade",    expected1.accolade,    actual[0].accolade    );

     assertEquals( "division_id", expected2.division_id, actual[1].division_id );
     assertEquals( "season",      expected2.season,      actual[1].season      );
     assertEquals( "accolade",    expected2.accolade,    actual[1].accolade    );

     free( actual );

     sqlite3_exec( db, "delete from division_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_accolades_t_delete__ShouldDeleteMatchingRecord_GivenADivisionAccolade()
{
     division_accolade_s expected = { 0 };

     expected.division_id = 123;
     expected.season      = 5;
     expected.accolade    = dacc_League_Title;

     insert_a_division_accolade( &expected );

     assertEquals( "division_accolades_t_delete()", SQLITE_OK, division_accolades_t_delete( db, &expected ) );

     assertNull( "get_a_division_accolade()", get_a_division_accolade( expected.division_id ) );

     sqlite3_exec( db, "delete from division_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenADivisionAccolade()
{
     division_accolade_s expected1 = { 0 };
     division_accolade_s expected2 = { 0 };

     expected1.division_id = 123;
     expected1.season      = 5;
     expected1.accolade    = dacc_League_Title;

     insert_a_division_accolade( &expected1 );

     expected2.division_id = 123;
     expected2.season      = 5;
     expected2.accolade    = dacc_Best_Record;

     insert_a_division_accolade( &expected2 );

     assertEquals( "division_accolades_t_delete()", SQLITE_OK, division_accolades_t_delete( db, &expected2 ) );

     division_accolade_s *actual = get_a_division_accolade( expected1.division_id );

     assertEquals( "division_id", expected1.division_id, actual->division_id );
     assertEquals( "season",      expected1.season,      actual->season      );
     assertEquals( "accolade",    expected1.accolade,    actual->accolade    );

     sqlite3_exec( db, "delete from division_accolades_t", NULL, NULL, NULL );

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
     run_test( division_accolades_t_create__ShouldInsertRecordsInTheDivisionAccoladesTTable,           check_sqlite_error );
     run_test( division_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,          check_sqlite_error );
     run_test( division_accolades_t_read_by_division__ShouldRetrieveMatchingRecord_GivenTheDivisionId,     check_sqlite_error );
     run_test( division_accolades_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheDivisionId, check_sqlite_error );
     run_test( division_accolades_t_delete__ShouldDeleteMatchingRecord_GivenADivisionAccolade,         check_sqlite_error );
     run_test( division_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenADivisionAccolade,     check_sqlite_error );
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
