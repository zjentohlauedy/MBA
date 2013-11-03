#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_pitcher_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static pitcher_stats_s pitcher_stats;

     pitcher_stats_s **ps = (pitcher_stats_s **)output;

     if ( cols < 14 ) return SQLITE_ERROR;

     memset( &pitcher_stats, '\0', sizeof(pitcher_stats_s) );

     /**/    pitcher_stats.player_id       = atoi( data[ 0] );
     /**/    pitcher_stats.season          = atoi( data[ 1] );
     /**/    pitcher_stats.season_phase    = atoi( data[ 2] );
     /**/    pitcher_stats.wins            = atoi( data[ 3] );
     /**/    pitcher_stats.losses          = atoi( data[ 4] );
     /**/    pitcher_stats.games           = atoi( data[ 5] );
     /**/    pitcher_stats.saves           = atoi( data[ 6] );
     /**/    pitcher_stats.innings.innings = atoi( data[ 7] );
     /**/    pitcher_stats.innings.outs    = atoi( data[ 8] );
     /**/    pitcher_stats.hits            = atoi( data[ 9] );
     /**/    pitcher_stats.earned_runs     = atoi( data[10] );
     /**/    pitcher_stats.home_runs       = atoi( data[11] );
     /**/    pitcher_stats.walks           = atoi( data[12] );
     /**/    pitcher_stats.strike_outs     = atoi( data[13] );

     *ps = &pitcher_stats;

     return SQLITE_OK;
}

static pitcher_stats_s *get_a_pitcher_stats( int player_id )
{
     pitcher_stats_s *pitcher_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from pitcher_stats_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_pitcher_stats_data, &pitcher_stats, NULL );

     return pitcher_stats;
}


static void insert_a_pitcher_stats( pitcher_stats_s *pitcher_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into pitcher_stats_t (player_id, season, season_phase, wins, losses, games, saves, innings, outs, hits, earned_runs, home_runs, walks, strike_outs)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               pitcher_stats->player_id,
               pitcher_stats->season,
               pitcher_stats->season_phase,
               pitcher_stats->wins,
               pitcher_stats->losses,
               pitcher_stats->games,
               pitcher_stats->saves,
               pitcher_stats->innings.innings,
               pitcher_stats->innings.outs,
               pitcher_stats->hits,
               pitcher_stats->earned_runs,
               pitcher_stats->home_runs,
               pitcher_stats->walks,
               pitcher_stats->strike_outs );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *pitcher_stats_t_create__ShouldInsertRecordsInThePitcherStatsTTable()
{
     pitcher_stats_s *expected = buildPitcherStats( 123, 5, sp_Regular );

     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, expected ) );

     pitcher_stats_s *actual = get_a_pitcher_stats( expected->player_id );

     comparePitcherStats( expected, actual );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}


static char *pitcher_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndPhaseExists()
{
     pitcher_stats_s *expected1 = buildPitcherStats( 123, 5, sp_Regular );
     pitcher_stats_s *expected2 = buildPitcherStats( 123, 5, sp_Regular );

     assertEquals( SQLITE_OK,         pitcher_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, pitcher_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}


static char *pitcher_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     pitcher_stats_s *expected = buildPitcherStats( 123, 5, sp_Regular );

     insert_a_pitcher_stats( expected );

     pitcher_stats_s actual = { 0 };

     actual.player_id    = expected->player_id;
     actual.season       = expected->season;
     actual.season_phase = expected->season_phase;

     assertEquals( SQLITE_OK, pitcher_stats_t_read( db, &actual ) );

     comparePitcherStats( expected, &actual );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *pitcher_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId()
{
     pitcher_stats_s *expected1 = buildPitcherStats( 123, 5, sp_Regular );
     pitcher_stats_s *expected2 = buildPitcherStats( 123, 6, sp_Regular );

     insert_a_pitcher_stats( expected1 );
     insert_a_pitcher_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, pitcher_stats_t_read_by_player( db, expected1->player_id, &list ) );

     assertEquals(  2, list.count );
     assertNotNull(    list.data  );

     pitcher_stats_s *actual = list.data;

     comparePitcherStats( expected1, &actual[0] );
     comparePitcherStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *pitcher_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     pitcher_stats_s *expected1 = buildPitcherStats( 123, 5, sp_Regular );
     pitcher_stats_s *expected2 = buildPitcherStats( 123, 5, sp_Regular );

     insert_a_pitcher_stats( expected1 );

     assertEquals( SQLITE_OK, pitcher_stats_t_update( db, expected2 ) );

     pitcher_stats_s *actual = get_a_pitcher_stats( expected1->player_id );

     comparePitcherStats( expected2, actual );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char* pitcher_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     pitcher_stats_s *expected = buildPitcherStats( 123, 5, sp_Regular );

     insert_a_pitcher_stats( expected );

     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, expected ) );

     assertNull( get_a_pitcher_stats( expected->player_id ) );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     free( expected );

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
     run_test( pitcher_stats_t_create__ShouldInsertRecordsInThePitcherStatsTTable,                 check_sqlite_error );
     run_test( pitcher_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndPhaseExists, check_sqlite_error );
     run_test( pitcher_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndPhase,  check_sqlite_error );
     run_test( pitcher_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId,  check_sqlite_error );
     run_test( pitcher_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndPhase,  check_sqlite_error );
     run_test( pitcher_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndPhase,  check_sqlite_error );
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
