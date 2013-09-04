#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_league_data( void *output, int cols, char *data[], char *names[] )
{
     static league_s league;

     league_s **l = (league_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &league, '\0', sizeof(league_s) );

     /**/    league.league_id = atoi( data[0] );
     strcpy( league.name,             data[1] );

     *l = &league;

     return SQLITE_OK;
}

static league_s *get_a_league( int league_id )
{
     league_s *league = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from leagues_t where league_id = %d", league_id );

     sqlite3_exec( db, query, load_league_data, &league, NULL );

     return league;
}

static void insert_a_league( league_s *league )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into leagues_t (league_id, name)"
               "values (%d, '%s')",
               league->league_id,
               league->name );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *leagues_t_create__ShouldInsertRecordsInTheLeaguesTTable()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 5;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( "leagues_t_create()", SQLITE_OK, leagues_t_create( db, &expected ) );

     league_s *actual = get_a_league( expected.league_id );

     assertEqualsInt( "league_id", expected.league_id, actual->league_id );
     assertEqualsStr( "name",      expected.name,      actual->name      );

     sqlite3_exec( db, "delete from leagues_t", NULL, NULL, NULL );

     return NULL;
}

static char *leagues_t_create__ShouldGiveAnErrorIfLeagueIdAlreadyExists()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 5;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( "leagues_t_create()", SQLITE_OK, leagues_t_create( db, &expected ) );

     strcpy( expected.name, "LeagueName2" );

     assertEquals( "leagues_t_create()", SQLITE_CONSTRAINT, leagues_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from leagues_t", NULL, NULL, NULL );

     return NULL;
}

static char *leagues_t_read__ShouldRetrieveMatchingRecord_GivenTheLeagueId()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 5;
     strcpy( expected.name,      "LeagueName1" );

     insert_a_league( &expected );

     league_s actual = { 0 };

     actual.league_id = expected.league_id;

     assertEquals( "leagues_t_read()", SQLITE_OK, leagues_t_read( db, &actual ) );

     assertEqualsInt( "league_id", expected.league_id, actual.league_id );
     assertEqualsStr( "name",      expected.name,      actual.name      );

     sqlite3_exec( db, "delete from leagues_t", NULL, NULL, NULL );

     return NULL;
}

static char *leagues_t_update__ShouldModifyMatchingRecord_GivenTheLeagueId()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 5;
     strcpy( expected.name,      "LeagueName1" );

     insert_a_league( &expected );

     strcpy( expected.name, "LeagueName2" );

     assertEquals( "leagues_t_update()", SQLITE_OK, leagues_t_update( db, &expected ) );

     league_s *actual = get_a_league( expected.league_id );

     assertEqualsInt( "league_id", expected.league_id, actual->league_id );
     assertEqualsStr( "name",      expected.name,      actual->name      );

     sqlite3_exec( db, "delete from leagues_t", NULL, NULL, NULL );

     return NULL;
}

static char *leagues_t_delete__ShouldDeleteMatchingRecord_GivenTheLeagueId()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 5;
     strcpy( expected.name,      "LeagueName1" );

     insert_a_league( &expected );

     assertEquals( "leagues_t_delete()", SQLITE_OK, leagues_t_delete( db, &expected ) );

     assertNull( "get_a_league()", get_a_league( expected.league_id ) );

     sqlite3_exec( db, "delete from leagues_t", NULL, NULL, NULL );

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
     run_test( leagues_t_create__ShouldInsertRecordsInTheLeaguesTTable,       check_sqlite_error );
     run_test( leagues_t_create__ShouldGiveAnErrorIfLeagueIdAlreadyExists,    check_sqlite_error );
     run_test( leagues_t_read__ShouldRetrieveMatchingRecord_GivenTheLeagueId, check_sqlite_error );
     run_test( leagues_t_update__ShouldModifyMatchingRecord_GivenTheLeagueId, check_sqlite_error );
     run_test( leagues_t_delete__ShouldDeleteMatchingRecord_GivenTheLeagueId, check_sqlite_error );
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
