#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_league_accolade_data( void *output, int cols, char *data[], char *names[] )
{
     static league_accolade_s league_accolade;

     league_accolade_s **la = (league_accolade_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &league_accolade, '\0', sizeof(league_accolade_s) );

     league_accolade.league_id = atoi( data[0] );
     league_accolade.season    = atoi( data[1] );
     league_accolade.accolade  = atoi( data[2] );

     *la = &league_accolade;

     return SQLITE_OK;
}

static league_accolade_s *get_a_league_accolade( int league_id )
{
     league_accolade_s *league_accolade = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from league_accolades_t where league_id = %d", league_id );

     sqlite3_exec( db, query, load_league_accolade_data, &league_accolade, NULL );

     return league_accolade;
}

static void insert_a_league_accolade( league_accolade_s *league_accolade )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into league_accolades_t (league_id, season, accolade) values (%d, %d, %d)",
               league_accolade->league_id,
               league_accolade->season,
               league_accolade->accolade );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}

static char *league_accolades_t_create__ShouldInsertRecordsInTheLeagueAccoladesTTable()
{
     league_accolade_s expected = { 0 };

     expected.league_id = 123;
     expected.season    = 5;
     expected.accolade  = lacc_World_Title;

     assertEquals( SQLITE_OK, league_accolades_t_create( db, &expected ) );

     league_accolade_s *actual = get_a_league_accolade( expected.league_id );

     assertEquals( expected.league_id, actual->league_id );
     assertEquals( expected.season,    actual->season      );
     assertEquals( expected.accolade,  actual->accolade    );

     sqlite3_exec( db, "delete from league_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     league_accolade_s expected = { 0 };

     expected.league_id = 123;
     expected.season    = 5;
     expected.accolade  = lacc_World_Title;

     assertEquals( SQLITE_OK,         league_accolades_t_create( db, &expected ) );
     assertEquals( SQLITE_CONSTRAINT, league_accolades_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from league_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_accolades_t_read_by_league__ShouldRetrieveMatchingRecord_GivenTheLeagueId()
{
     league_accolade_s expected = { 0 };

     expected.league_id = 123;
     expected.season    = 5;
     expected.accolade  = lacc_World_Title;

     insert_a_league_accolade( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_accolades_t_read_by_league( db, expected.league_id, &list ) );

     assertEquals( 1, list.count );

     league_accolade_s *actual = list.data;

     assertEquals( expected.league_id, actual->league_id );
     assertEquals( expected.season,    actual->season      );
     assertEquals( expected.accolade,  actual->accolade    );

     free( actual );

     sqlite3_exec( db, "delete from league_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_accolades_t_read_by_league__ShouldRetrieveAllMatchingRecords_GivenTheLeagueId()
{
     league_accolade_s expected1 = { 0 };
     league_accolade_s expected2 = { 0 };

     expected1.league_id = 123;
     expected1.season    = 5;
     expected1.accolade  = lacc_World_Title;

     expected2.league_id = 123;
     expected2.season    = 6;
     expected2.accolade  = lacc_Best_Record;

     insert_a_league_accolade( &expected1 );
     insert_a_league_accolade( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_accolades_t_read_by_league( db, expected1.league_id, &list ) );

     assertEquals( 2, list.count );

     league_accolade_s *actual = list.data;

     assertEquals( expected1.league_id, actual[0].league_id );
     assertEquals( expected1.season,    actual[0].season    );
     assertEquals( expected1.accolade,  actual[0].accolade  );

     assertEquals( expected2.league_id, actual[1].league_id );
     assertEquals( expected2.season,    actual[1].season    );
     assertEquals( expected2.accolade,  actual[1].accolade  );

     free( actual );

     sqlite3_exec( db, "delete from league_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_accolades_t_delete__ShouldDeleteMatchingRecord_GivenALeagueAccolade()
{
     league_accolade_s expected = { 0 };

     expected.league_id = 123;
     expected.season    = 5;
     expected.accolade  = lacc_World_Title;

     insert_a_league_accolade( &expected );

     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected ) );

     assertNull( get_a_league_accolade( expected.league_id ) );

     sqlite3_exec( db, "delete from league_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenALeagueAccolade()
{
     league_accolade_s expected1 = { 0 };
     league_accolade_s expected2 = { 0 };

     expected1.league_id = 123;
     expected1.season    = 5;
     expected1.accolade  = lacc_World_Title;

     insert_a_league_accolade( &expected1 );

     expected2.league_id = 123;
     expected2.season    = 5;
     expected2.accolade  = lacc_Best_Record;

     insert_a_league_accolade( &expected2 );

     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected2 ) );

     league_accolade_s *actual = get_a_league_accolade( expected1.league_id );

     assertEquals( expected1.league_id, actual->league_id );
     assertEquals( expected1.season,    actual->season      );
     assertEquals( expected1.accolade,  actual->accolade    );

     sqlite3_exec( db, "delete from league_accolades_t", NULL, NULL, NULL );

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
     run_test( league_accolades_t_create__ShouldInsertRecordsInTheLeagueAccoladesTTable,             check_sqlite_error );
     run_test( league_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,              check_sqlite_error );
     run_test( league_accolades_t_read_by_league__ShouldRetrieveMatchingRecord_GivenTheLeagueId,     check_sqlite_error );
     run_test( league_accolades_t_read_by_league__ShouldRetrieveAllMatchingRecords_GivenTheLeagueId, check_sqlite_error );
     run_test( league_accolades_t_delete__ShouldDeleteMatchingRecord_GivenALeagueAccolade,           check_sqlite_error );
     run_test( league_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenALeagueAccolade,       check_sqlite_error );
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
