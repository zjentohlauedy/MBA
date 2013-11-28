#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_league_team_data( void *output, int cols, char *data[], char *names[] )
{
     static league_team_s league_team;

     league_team_s **dt = (league_team_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &league_team, '\0', sizeof(league_team_s) );

     league_team.league_id = atoi( data[0] );
     league_team.team_id   = atoi( data[1] );

     *dt = &league_team;

     return SQLITE_OK;
}

static league_team_s *get_a_league_team( int league_id )
{
     league_team_s *league_team = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from league_teams_t where league_id = %d", league_id );

     sqlite3_exec( db, query, load_league_team_data, &league_team, NULL );

     return league_team;
}

static void insert_a_league_team( league_team_s *league_team )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into league_teams_t (league_id, team_id) values (%d, %d)",
               league_team->league_id,
               league_team->team_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *league_teams_t_create__ShouldInsertRecordsInTheLeagueTeamsTTable()
{
     league_team_s expected = { 0 };

     expected.league_id = 8;
     expected.team_id   = 123;

     assertEquals( SQLITE_OK, league_teams_t_create( db, &expected) );

     league_team_s *actual = get_a_league_team( expected.league_id );

     assertEquals( expected.league_id, actual->league_id );
     assertEquals( expected.team_id,   actual->team_id     );

     sqlite3_exec( db, "delete from league_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_teams_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     league_team_s expected = { 0 };

     expected.league_id = 8;
     expected.team_id   = 123;

     assertEquals( SQLITE_OK,         league_teams_t_create( db, &expected) );
     assertEquals( SQLITE_CONSTRAINT, league_teams_t_create( db, &expected) );

     sqlite3_exec( db, "delete from league_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_teams_t_read_by_league__ShouldRetrieveMatchingRecord_GivenTheLeagueId()
{
     league_team_s expected = { 0 };

     expected.league_id = 8;
     expected.team_id   = 123;

     insert_a_league_team( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_teams_t_read_by_league( db, expected.league_id, &list) );

     assertEquals( 1, list.count );

     league_team_s *actual = list.data;

     assertEquals( expected.league_id, actual[0].league_id );
     assertEquals( expected.team_id,   actual[0].team_id     );

     free( actual );

     sqlite3_exec( db, "delete from league_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_teams_t_read_by_league__ShouldRetrieveAllMatchingRecords_GivenTheLeagueId()
{
     league_team_s expected1 = { 0 };
     league_team_s expected2 = { 0 };

     expected1.league_id = 8;
     expected1.team_id   = 123;

     expected2.league_id = 8;
     expected2.team_id   = 321;

     insert_a_league_team( &expected1 );
     insert_a_league_team( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_teams_t_read_by_league( db, expected1.league_id, &list) );

     assertEquals( 2, list.count );

     league_team_s *actual = list.data;

     assertEquals( expected1.league_id, actual[0].league_id );
     assertEquals( expected1.team_id,   actual[0].team_id     );

     assertEquals( expected2.league_id, actual[1].league_id );
     assertEquals( expected2.team_id,   actual[1].team_id     );

     free( actual );

     sqlite3_exec( db, "delete from league_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_teams_t_delete__ShouldDeleteMatchingRecord_GivenALeagueTeam()
{
     league_team_s expected = { 0 };

     expected.league_id = 8;
     expected.team_id   = 123;

     insert_a_league_team( &expected );

     assertEquals( SQLITE_OK, league_teams_t_delete( db, &expected) );

     assertNull( get_a_league_team( expected.league_id ) );

     sqlite3_exec( db, "delete from league_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_teams_t_delete__ShouldOnlyDeleteMatchingRecord_GivenALeagueTeam()
{
     league_team_s expected1 = { 0 };
     league_team_s expected2 = { 0 };

     expected1.league_id = 8;
     expected1.team_id   = 123;

     expected2.league_id = 8;
     expected2.team_id   = 321;

     insert_a_league_team( &expected1 );
     insert_a_league_team( &expected2 );

     assertEquals( SQLITE_OK, league_teams_t_delete( db, &expected2) );

     league_team_s *actual = get_a_league_team( expected1.league_id );

     assertEquals( expected1.league_id, actual->league_id );
     assertEquals( expected1.team_id,   actual->team_id     );

     sqlite3_exec( db, "delete from league_teams_t", NULL, NULL, NULL );

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
     run_test( league_teams_t_create__ShouldInsertRecordsInTheLeagueTeamsTTable,                 check_sqlite_error );
     run_test( league_teams_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,              check_sqlite_error );
     run_test( league_teams_t_read_by_league__ShouldRetrieveMatchingRecord_GivenTheLeagueId,     check_sqlite_error );
     run_test( league_teams_t_read_by_league__ShouldRetrieveAllMatchingRecords_GivenTheLeagueId, check_sqlite_error );
     run_test( league_teams_t_delete__ShouldDeleteMatchingRecord_GivenALeagueTeam,               check_sqlite_error );
     run_test( league_teams_t_delete__ShouldOnlyDeleteMatchingRecord_GivenALeagueTeam,           check_sqlite_error );
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
