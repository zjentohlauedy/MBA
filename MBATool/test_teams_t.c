#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_team_data( void *output, int cols, char *data[], char *names[] )
{
     static team_s team;

     team_s **t = (team_s **)output;

     if ( cols < 5 ) return SQLITE_ERROR;

     memset( &team, '\0', sizeof(team_s) );

     /**/    team.team_id         = atoi( data[0] );
     strcpy( team.name,                   data[1] );
     strcpy( team.location,               data[2] );
     /**/    team.primary_color   = atoi( data[3] );
     /**/    team.secondary_color = atoi( data[4] );

     *t = &team;

     return SQLITE_OK;
}

static team_s *get_a_team( int team_id )
{
     team_s *team = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from teams_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_data, &team, NULL );

     return team;
}

static void insert_a_team( team_s *team )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into teams_t (team_id, name, location, primary_color, secondary_color)"
               "values (%d, '%s', '%s', %d, %d)",
               team->team_id,
               team->name,
               team->location,
               team->primary_color,
               team->secondary_color );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *teams_t_create__ShouldInsertRecordsInTheTeamsTTable()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 123;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( "teams_t_create()", SQLITE_OK, teams_t_create( db, &expected ) );

     team_s *actual = get_a_team( expected.team_id );

     assertEquals(    "team_id",         expected.team_id,         actual->team_id         );
     assertEqualsStr( "name",            expected.name,            actual->name            );
     assertEqualsStr( "location",        expected.location,        actual->location        );
     assertEquals(    "primary_color",   expected.primary_color,   actual->primary_color   );
     assertEquals(    "secondary_color", expected.secondary_color, actual->secondary_color );

     sqlite3_exec( db, "delete from teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *teams_t_create__ShouldGiveAnErrorIfTeamIdAlreadyExists()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 123;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( "teams_t_create()", SQLITE_OK, teams_t_create( db, &expected ) );

     strcpy( expected.name,            "TeamName2" );
     strcpy( expected.location,        "Location2" );
     /**/    expected.primary_color   = cl_Red;
     /**/    expected.secondary_color = cl_Black;

     assertEquals( "teams_t_create()", SQLITE_CONSTRAINT, teams_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *teams_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamId()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 123;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     insert_a_team( &expected );

     team_s actual = { 0 };

     actual.team_id = expected.team_id;

     assertEquals( "teams_t_read()", SQLITE_OK, teams_t_read( db, &actual ) );

     assertEquals(    "team_id",         expected.team_id,         actual.team_id         );
     assertEqualsStr( "name",            expected.name,            actual.name            );
     assertEqualsStr( "location",        expected.location,        actual.location        );
     assertEquals(    "primary_color",   expected.primary_color,   actual.primary_color   );
     assertEquals(    "secondary_color", expected.secondary_color, actual.secondary_color );

     sqlite3_exec( db, "delete from teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *teams_t_update__ShouldModifyMatchingRecord_GivenTheTeamId()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 123;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     insert_a_team( &expected );

     strcpy( expected.name,            "TeamName2" );
     strcpy( expected.location,        "Location2" );
     /**/    expected.primary_color   = cl_Red;
     /**/    expected.secondary_color = cl_Black;

     assertEquals( "teams_t_update()", SQLITE_OK, teams_t_update( db, &expected ) );

     team_s *actual = get_a_team( expected.team_id );

     assertEquals(    "team_id",         expected.team_id,         actual->team_id         );
     assertEqualsStr( "name",            expected.name,            actual->name            );
     assertEqualsStr( "location",        expected.location,        actual->location        );
     assertEquals(    "primary_color",   expected.primary_color,   actual->primary_color   );
     assertEquals(    "secondary_color", expected.secondary_color, actual->secondary_color );

     sqlite3_exec( db, "delete from teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *teams_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamId()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 123;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     insert_a_team( &expected );

     assertEquals( "teams_t_delete()", SQLITE_OK, teams_t_delete( db, &expected ) );

     assertEquals( "get_a_team()", NULL, get_a_team( expected.team_id ) );

     sqlite3_exec( db, "delete from teams_t", NULL, NULL, NULL );

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
     run_test( teams_t_create__ShouldInsertRecordsInTheTeamsTTable,       check_sqlite_error );
     run_test( teams_t_create__ShouldGiveAnErrorIfTeamIdAlreadyExists,    check_sqlite_error );
     run_test( teams_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamId, check_sqlite_error );
     run_test( teams_t_update__ShouldModifyMatchingRecord_GivenTheTeamId, check_sqlite_error );
     run_test( teams_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamId, check_sqlite_error );
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
