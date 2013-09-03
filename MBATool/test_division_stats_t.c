#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_division_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static division_stats_s division_stats;

     division_stats_s **ds = (division_stats_s **)output;

     if ( cols < 13 ) return SQLITE_ERROR;

     memset( &division_stats, '\0', sizeof(division_stats_s) );

     division_stats.division_id   = atoi( data[ 0] );
     division_stats.season        = atoi( data[ 1] );
     division_stats.season_phase  = atoi( data[ 2] );
     division_stats.wins          = atoi( data[ 3] );
     division_stats.losses        = atoi( data[ 4] );
     division_stats.home_wins     = atoi( data[ 5] );
     division_stats.home_losses   = atoi( data[ 6] );
     division_stats.road_wins     = atoi( data[ 7] );
     division_stats.road_losses   = atoi( data[ 8] );
     division_stats.league_wins   = atoi( data[ 9] );
     division_stats.league_losses = atoi( data[10] );
     division_stats.runs_scored   = atoi( data[11] );
     division_stats.runs_allowed  = atoi( data[12] );

     *ds = &division_stats;

     return SQLITE_OK;
}

static division_stats_s *get_a_division_stats( int division_id )
{
     division_stats_s *division_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from division_stats_t where division_id = %d", division_id );

     sqlite3_exec( db, query, load_division_stats_data, &division_stats, NULL );

     return division_stats;
}

static void insert_a_division_stats( division_stats_s *division_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into division_stats_t (division_id, season, season_phase, wins, losses, home_wins, home_losses, road_wins, road_losses, league_wins, league_losses, runs_scored, runs_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               division_stats->division_id,
               division_stats->season,
               division_stats->season_phase,
               division_stats->wins,
               division_stats->losses,
               division_stats->home_wins,
               division_stats->home_losses,
               division_stats->road_wins,
               division_stats->road_losses,
               division_stats->league_wins,
               division_stats->league_losses,
               division_stats->runs_scored,
               division_stats->runs_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *division_stats_t_create__ShouldInsertRecordsInTheDivisionStatsTTable()
{
     division_stats_s expected = { 0 };

     expected.division_id   = 3;
     expected.season        = 2;
     expected.season_phase  = sp_Regular;
     expected.wins          = 388;
     expected.losses        = 380;
     expected.home_wins     = 205;
     expected.home_losses   = 179;
     expected.road_wins     = 166;
     expected.road_losses   = 218;
     expected.league_wins   = 135;
     expected.league_losses = 121;
     expected.runs_scored   = 2642;
     expected.runs_allowed  = 2619;

     assertEquals( "division_stats_t_create()", SQLITE_OK, division_stats_t_create( db, &expected ) );

     division_stats_s *actual = get_a_division_stats( expected.division_id );

     assertEquals( "division_id",   expected.division_id,   actual->division_id     );
     assertEquals( "season",        expected.season,        actual->season          );
     assertEquals( "season_phase",  expected.season_phase,  actual->season_phase    );
     assertEquals( "wins",          expected.wins,          actual->wins            );
     assertEquals( "losses",        expected.losses,        actual->losses          );
     assertEquals( "home_wins",     expected.home_wins,     actual->home_wins       );
     assertEquals( "home_losses",   expected.home_losses,   actual->home_losses     );
     assertEquals( "road_wins",     expected.road_wins,     actual->road_wins       );
     assertEquals( "road_losses",   expected.road_losses,   actual->road_losses     );
     assertEquals( "league_wins",   expected.league_wins,   actual->league_wins     );
     assertEquals( "league_losses", expected.league_losses, actual->league_losses   );
     assertEquals( "runs_scored",   expected.runs_scored,   actual->runs_scored     );
     assertEquals( "runs_allowed",  expected.runs_allowed,  actual->runs_allowed    );

     sqlite3_exec( db, "delete from division_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_stats_t_create__ShouldGiveAnErrorIfRecordForSameDivisionSeasonAndPhaseExists()
{
     division_stats_s expected = { 0 };

     expected.division_id   = 3;
     expected.season        = 2;
     expected.season_phase  = sp_Regular;
     expected.wins          = 388;
     expected.losses        = 380;
     expected.home_wins     = 205;
     expected.home_losses   = 179;
     expected.road_wins     = 166;
     expected.road_losses   = 218;
     expected.league_wins   = 135;
     expected.league_losses = 121;
     expected.runs_scored   = 2642;
     expected.runs_allowed  = 2619;

     assertEquals( "division_stats_t_create()", SQLITE_OK, division_stats_t_create( db, &expected ) );

     expected.wins          = 392;
     expected.losses        = 376;
     expected.home_wins     = 195;
     expected.home_losses   = 189;
     expected.road_wins     = 167;
     expected.road_losses   = 217;
     expected.league_wins   = 138;
     expected.league_losses = 118;
     expected.runs_scored   = 2698;
     expected.runs_allowed  = 2703;

     assertEquals( "division_stats_t_create()", SQLITE_CONSTRAINT, division_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from division_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheDivisionIdSeasonAndPhase()
{
     division_stats_s expected = { 0 };

     expected.division_id   = 3;
     expected.season        = 2;
     expected.season_phase  = sp_Regular;
     expected.wins          = 388;
     expected.losses        = 380;
     expected.home_wins     = 205;
     expected.home_losses   = 179;
     expected.road_wins     = 166;
     expected.road_losses   = 218;
     expected.league_wins   = 135;
     expected.league_losses = 121;
     expected.runs_scored   = 2642;
     expected.runs_allowed  = 2619;

     insert_a_division_stats( &expected );

     division_stats_s actual = { 0 };

     actual.division_id  = expected.division_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( "division_stats_t_read()", SQLITE_OK, division_stats_t_read( db, &actual ) );

     assertEquals( "division_id",   expected.division_id,   actual.division_id     );
     assertEquals( "season",        expected.season,        actual.season          );
     assertEquals( "season_phase",  expected.season_phase,  actual.season_phase    );
     assertEquals( "wins",          expected.wins,          actual.wins            );
     assertEquals( "losses",        expected.losses,        actual.losses          );
     assertEquals( "home_wins",     expected.home_wins,     actual.home_wins       );
     assertEquals( "home_losses",   expected.home_losses,   actual.home_losses     );
     assertEquals( "road_wins",     expected.road_wins,     actual.road_wins       );
     assertEquals( "road_losses",   expected.road_losses,   actual.road_losses     );
     assertEquals( "league_wins",   expected.league_wins,   actual.league_wins     );
     assertEquals( "league_losses", expected.league_losses, actual.league_losses   );
     assertEquals( "runs_scored",   expected.runs_scored,   actual.runs_scored     );
     assertEquals( "runs_allowed",  expected.runs_allowed,  actual.runs_allowed    );

     sqlite3_exec( db, "delete from division_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_stats_t_update__ShouldModifyMatchingRecord_GivenTheDivisionIdSeasonAndPhase()
{
     division_stats_s expected = { 0 };

     expected.division_id   = 3;
     expected.season        = 2;
     expected.season_phase  = sp_Regular;
     expected.wins          = 388;
     expected.losses        = 380;
     expected.home_wins     = 205;
     expected.home_losses   = 179;
     expected.road_wins     = 166;
     expected.road_losses   = 218;
     expected.league_wins   = 135;
     expected.league_losses = 121;
     expected.runs_scored   = 2642;
     expected.runs_allowed  = 2619;

     insert_a_division_stats( &expected );

     expected.wins          = 392;
     expected.losses        = 376;
     expected.home_wins     = 195;
     expected.home_losses   = 189;
     expected.road_wins     = 167;
     expected.road_losses   = 217;
     expected.league_wins   = 138;
     expected.league_losses = 118;
     expected.runs_scored   = 2698;
     expected.runs_allowed  = 2703;

     assertEquals( "division_stats_t_update()", SQLITE_OK, division_stats_t_update( db, &expected ) );

     division_stats_s *actual = get_a_division_stats( expected.division_id );

     assertEquals( "division_id",   expected.division_id,   actual->division_id     );
     assertEquals( "season",        expected.season,        actual->season          );
     assertEquals( "season_phase",  expected.season_phase,  actual->season_phase    );
     assertEquals( "wins",          expected.wins,          actual->wins            );
     assertEquals( "losses",        expected.losses,        actual->losses          );
     assertEquals( "home_wins",     expected.home_wins,     actual->home_wins       );
     assertEquals( "home_losses",   expected.home_losses,   actual->home_losses     );
     assertEquals( "road_wins",     expected.road_wins,     actual->road_wins       );
     assertEquals( "road_losses",   expected.road_losses,   actual->road_losses     );
     assertEquals( "league_wins",   expected.league_wins,   actual->league_wins     );
     assertEquals( "league_losses", expected.league_losses, actual->league_losses   );
     assertEquals( "runs_scored",   expected.runs_scored,   actual->runs_scored     );
     assertEquals( "runs_allowed",  expected.runs_allowed,  actual->runs_allowed    );

     sqlite3_exec( db, "delete from division_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheDivisionIdSeasonAndPhase()
{
     division_stats_s expected = { 0 };

     expected.division_id   = 3;
     expected.season        = 2;
     expected.season_phase  = sp_Regular;
     expected.wins          = 388;
     expected.losses        = 380;
     expected.home_wins     = 205;
     expected.home_losses   = 179;
     expected.road_wins     = 166;
     expected.road_losses   = 218;
     expected.league_wins   = 135;
     expected.league_losses = 121;
     expected.runs_scored   = 2642;
     expected.runs_allowed  = 2619;

     insert_a_division_stats( &expected );

     assertEquals( "division_stats_t_delete()", SQLITE_OK, division_stats_t_delete( db, &expected ) );

     assertNull( "get_a_division_stats()", get_a_division_stats( expected.division_id ) );

     sqlite3_exec( db, "delete from division_stats_t", NULL, NULL, NULL );

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
     run_test( division_stats_t_create__ShouldInsertRecordsInTheDivisionStatsTTable,                  check_sqlite_error );
     run_test( division_stats_t_create__ShouldGiveAnErrorIfRecordForSameDivisionSeasonAndPhaseExists, check_sqlite_error );
     run_test( division_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheDivisionIdSeasonAndPhase,  check_sqlite_error );
     run_test( division_stats_t_update__ShouldModifyMatchingRecord_GivenTheDivisionIdSeasonAndPhase,  check_sqlite_error );
     run_test( division_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheDivisionIdSeasonAndPhase,  check_sqlite_error );
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
