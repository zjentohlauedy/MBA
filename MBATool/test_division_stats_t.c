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

     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected ) );

     division_stats_s *actual = get_a_division_stats( expected.division_id );

     assertEquals( expected.division_id,   actual->division_id     );
     assertEquals( expected.season,        actual->season          );
     assertEquals( expected.season_phase,  actual->season_phase    );
     assertEquals( expected.wins,          actual->wins            );
     assertEquals( expected.losses,        actual->losses          );
     assertEquals( expected.home_wins,     actual->home_wins       );
     assertEquals( expected.home_losses,   actual->home_losses     );
     assertEquals( expected.road_wins,     actual->road_wins       );
     assertEquals( expected.road_losses,   actual->road_losses     );
     assertEquals( expected.league_wins,   actual->league_wins     );
     assertEquals( expected.league_losses, actual->league_losses   );
     assertEquals( expected.runs_scored,   actual->runs_scored     );
     assertEquals( expected.runs_allowed,  actual->runs_allowed    );

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

     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected ) );

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

     assertEquals( SQLITE_CONSTRAINT, division_stats_t_create( db, &expected ) );

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

     assertEquals( SQLITE_OK, division_stats_t_read( db, &actual ) );

     assertEquals( expected.division_id,   actual.division_id     );
     assertEquals( expected.season,        actual.season          );
     assertEquals( expected.season_phase,  actual.season_phase    );
     assertEquals( expected.wins,          actual.wins            );
     assertEquals( expected.losses,        actual.losses          );
     assertEquals( expected.home_wins,     actual.home_wins       );
     assertEquals( expected.home_losses,   actual.home_losses     );
     assertEquals( expected.road_wins,     actual.road_wins       );
     assertEquals( expected.road_losses,   actual.road_losses     );
     assertEquals( expected.league_wins,   actual.league_wins     );
     assertEquals( expected.league_losses, actual.league_losses   );
     assertEquals( expected.runs_scored,   actual.runs_scored     );
     assertEquals( expected.runs_allowed,  actual.runs_allowed    );

     sqlite3_exec( db, "delete from division_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *division_stats_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheDivisionId()
{
     division_stats_s expected1 = { 0 };
     division_stats_s expected2 = { 0 };

     expected1.division_id   = 3;
     expected1.season        = 2;
     expected1.season_phase  = sp_Regular;
     expected1.wins          = 388;
     expected1.losses        = 380;
     expected1.home_wins     = 205;
     expected1.home_losses   = 179;
     expected1.road_wins     = 166;
     expected1.road_losses   = 218;
     expected1.league_wins   = 135;
     expected1.league_losses = 121;
     expected1.runs_scored   = 2642;
     expected1.runs_allowed  = 2619;

     expected2.division_id   = 3;
     expected2.season        = 5;
     expected2.season_phase  = sp_Regular;
     expected2.wins          = 392;
     expected2.losses        = 376;
     expected2.home_wins     = 195;
     expected2.home_losses   = 189;
     expected2.road_wins     = 167;
     expected2.road_losses   = 217;
     expected2.league_wins   = 138;
     expected2.league_losses = 118;
     expected2.runs_scored   = 2698;
     expected2.runs_allowed  = 2703;

     insert_a_division_stats( &expected1 );
     insert_a_division_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_stats_t_read_by_division( db, expected1.division_id, &list ) );

     assertEquals( 2, list.count );

     division_stats_s *actual = list.data;

     assertNotNull( actual );

     assertEquals( expected1.division_id,   actual[0].division_id     );
     assertEquals( expected1.season,        actual[0].season          );
     assertEquals( expected1.season_phase,  actual[0].season_phase    );
     assertEquals( expected1.wins,          actual[0].wins            );
     assertEquals( expected1.losses,        actual[0].losses          );
     assertEquals( expected1.home_wins,     actual[0].home_wins       );
     assertEquals( expected1.home_losses,   actual[0].home_losses     );
     assertEquals( expected1.road_wins,     actual[0].road_wins       );
     assertEquals( expected1.road_losses,   actual[0].road_losses     );
     assertEquals( expected1.league_wins,   actual[0].league_wins     );
     assertEquals( expected1.league_losses, actual[0].league_losses   );
     assertEquals( expected1.runs_scored,   actual[0].runs_scored     );
     assertEquals( expected1.runs_allowed,  actual[0].runs_allowed    );

     assertEquals( expected2.division_id,   actual[1].division_id     );
     assertEquals( expected2.season,        actual[1].season          );
     assertEquals( expected2.season_phase,  actual[1].season_phase    );
     assertEquals( expected2.wins,          actual[1].wins            );
     assertEquals( expected2.losses,        actual[1].losses          );
     assertEquals( expected2.home_wins,     actual[1].home_wins       );
     assertEquals( expected2.home_losses,   actual[1].home_losses     );
     assertEquals( expected2.road_wins,     actual[1].road_wins       );
     assertEquals( expected2.road_losses,   actual[1].road_losses     );
     assertEquals( expected2.league_wins,   actual[1].league_wins     );
     assertEquals( expected2.league_losses, actual[1].league_losses   );
     assertEquals( expected2.runs_scored,   actual[1].runs_scored     );
     assertEquals( expected2.runs_allowed,  actual[1].runs_allowed    );

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

     assertEquals( SQLITE_OK, division_stats_t_update( db, &expected ) );

     division_stats_s *actual = get_a_division_stats( expected.division_id );

     assertEquals( expected.division_id,   actual->division_id     );
     assertEquals( expected.season,        actual->season          );
     assertEquals( expected.season_phase,  actual->season_phase    );
     assertEquals( expected.wins,          actual->wins            );
     assertEquals( expected.losses,        actual->losses          );
     assertEquals( expected.home_wins,     actual->home_wins       );
     assertEquals( expected.home_losses,   actual->home_losses     );
     assertEquals( expected.road_wins,     actual->road_wins       );
     assertEquals( expected.road_losses,   actual->road_losses     );
     assertEquals( expected.league_wins,   actual->league_wins     );
     assertEquals( expected.league_losses, actual->league_losses   );
     assertEquals( expected.runs_scored,   actual->runs_scored     );
     assertEquals( expected.runs_allowed,  actual->runs_allowed    );

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

     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected ) );

     assertNull( get_a_division_stats( expected.division_id ) );

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
     run_test( division_stats_t_create__ShouldInsertRecordsInTheDivisionStatsTTable,                   check_sqlite_error );
     run_test( division_stats_t_create__ShouldGiveAnErrorIfRecordForSameDivisionSeasonAndPhaseExists,  check_sqlite_error );
     run_test( division_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheDivisionIdSeasonAndPhase,   check_sqlite_error );
     run_test( division_stats_t_read_by_division__ShouldRetrieveAllMatchingRecords_GivenTheDivisionId, check_sqlite_error );
     run_test( division_stats_t_update__ShouldModifyMatchingRecord_GivenTheDivisionIdSeasonAndPhase,   check_sqlite_error );
     run_test( division_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheDivisionIdSeasonAndPhase,   check_sqlite_error );
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
