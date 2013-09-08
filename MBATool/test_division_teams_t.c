#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_division_team_data( void *output, int cols, char *data[], char *names[] )
{
     static division_team_s division_team;

     division_team_s **dt = (division_team_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &division_team, '\0', sizeof(division_team_s) );

     division_team.division_id = atoi( data[0] );
     division_team.team_id     = atoi( data[1] );

     *dt = &division_team;

     return SQLITE_OK;
}

static division_team_s *get_a_division_team( int division_id )
{
     division_team_s *division_team = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from division_teams_t where division_id = %d", division_id );

     sqlite3_exec( db, query, load_division_team_data, &division_team, NULL );

     return division_team;
}

static void insert_a_division_team( division_team_s *division_team )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into division_teams_t (division_id, team_id) values (%d, %d)",
               division_team->division_id,
               division_team->team_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *division_teams_t_create__ShouldInsertRecordsInTheDivisionTeamsTTable()
{
     division_team_s expected = { 0 };

     expected.division_id = 8;
     expected.team_id     = 123;

     assertEquals( SQLITE_OK, division_teams_t_create( db, &expected) );

     division_team_s *actual = get_a_division_team( expected.division_id );

     assertEquals( expected.division_id, actual->division_id );
     assertEquals( expected.team_id,     actual->team_id     );

     sqlite3_exec( db, "delete from division_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_teams_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     division_team_s expected = { 0 };

     expected.division_id = 8;
     expected.team_id     = 123;

     assertEquals( SQLITE_OK,         division_teams_t_create( db, &expected) );
     assertEquals( SQLITE_CONSTRAINT, division_teams_t_create( db, &expected) );

     sqlite3_exec( db, "delete from division_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_teams_t_read_by_division__ShouldRetrieveMatchingRecord_GivenTheDivisionId()
{
     division_team_s expected = { 0 };

     expected.division_id = 8;
     expected.team_id     = 123;

     insert_a_division_team( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_teams_t_read_by_division( db, expected.division_id, &list) );

     assertEquals( 1, list.count );

     division_team_s *actual = list.data;

     assertEquals( expected.division_id, actual[0].division_id );
     assertEquals( expected.team_id,     actual[0].team_id     );

     free( actual );

     sqlite3_exec( db, "delete from division_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_teams_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheDivisionId()
{
     division_team_s expected1 = { 0 };
     division_team_s expected2 = { 0 };

     expected1.division_id = 8;
     expected1.team_id     = 123;

     expected2.division_id = 8;
     expected2.team_id     = 321;

     insert_a_division_team( &expected1 );
     insert_a_division_team( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_teams_t_read_by_division( db, expected1.division_id, &list) );

     assertEquals( 2, list.count );

     division_team_s *actual = list.data;

     assertEquals( expected1.division_id, actual[0].division_id );
     assertEquals( expected1.team_id,     actual[0].team_id     );

     assertEquals( expected2.division_id, actual[1].division_id );
     assertEquals( expected2.team_id,     actual[1].team_id     );

     free( actual );

     sqlite3_exec( db, "delete from division_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_teams_t_delete__ShouldDeleteMatchingRecord_GivenADivisionTeam()
{
     division_team_s expected = { 0 };

     expected.division_id = 8;
     expected.team_id     = 123;

     insert_a_division_team( &expected );

     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected) );

     assertNull( get_a_division_team( expected.division_id ) );

     sqlite3_exec( db, "delete from division_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_teams_t_delete__ShouldOnlyDeleteMatchingRecord_GivenADivisionTeam()
{
     division_team_s expected1 = { 0 };
     division_team_s expected2 = { 0 };

     expected1.division_id = 8;
     expected1.team_id     = 123;

     expected2.division_id = 8;
     expected2.team_id     = 321;

     insert_a_division_team( &expected1 );
     insert_a_division_team( &expected2 );

     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected2) );

     division_team_s *actual = get_a_division_team( expected1.division_id );

     assertEquals( expected1.division_id, actual->division_id );
     assertEquals( expected1.team_id,     actual->team_id     );

     sqlite3_exec( db, "delete from division_teams_t", NULL, NULL, NULL );

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
     run_test( division_teams_t_create__ShouldInsertRecordsInTheDivisionTeamsTTable,                   check_sqlite_error );
     run_test( division_teams_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,                  check_sqlite_error );
     run_test( division_teams_t_read_by_division__ShouldRetrieveMatchingRecord_GivenTheDivisionId,     check_sqlite_error );
     run_test( division_teams_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheDivisionId, check_sqlite_error );
     run_test( division_teams_t_delete__ShouldDeleteMatchingRecord_GivenADivisionTeam,                 check_sqlite_error );
     run_test( division_teams_t_delete__ShouldOnlyDeleteMatchingRecord_GivenADivisionTeam,             check_sqlite_error );
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
