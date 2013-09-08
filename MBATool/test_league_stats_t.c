#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_league_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static league_stats_s league_stats;

     league_stats_s **ls = (league_stats_s **)output;

     if ( cols < 11 ) return SQLITE_ERROR;

     memset( &league_stats, '\0', sizeof(league_stats_s) );

     league_stats.league_id    = atoi( data[ 0] );
     league_stats.season       = atoi( data[ 1] );
     league_stats.season_phase = atoi( data[ 2] );
     league_stats.wins         = atoi( data[ 3] );
     league_stats.losses       = atoi( data[ 4] );
     league_stats.home_wins    = atoi( data[ 5] );
     league_stats.home_losses  = atoi( data[ 6] );
     league_stats.road_wins    = atoi( data[ 7] );
     league_stats.road_losses  = atoi( data[ 8] );
     league_stats.runs_scored  = atoi( data[ 9] );
     league_stats.runs_allowed = atoi( data[10] );

     *ls = &league_stats;

     return SQLITE_OK;
}

static league_stats_s *get_a_league_stats( int league_id )
{
     league_stats_s *league_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from league_stats_t where league_id = %d", league_id );

     sqlite3_exec( db, query, load_league_stats_data, &league_stats, NULL );

     return league_stats;
}

static void insert_a_league_stats( league_stats_s *league_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into league_stats_t (league_id, season, season_phase, wins, losses, home_wins, home_losses, road_wins, road_losses, runs_scored, runs_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               league_stats->league_id,
               league_stats->season,
               league_stats->season_phase,
               league_stats->wins,
               league_stats->losses,
               league_stats->home_wins,
               league_stats->home_losses,
               league_stats->road_wins,
               league_stats->road_losses,
               league_stats->runs_scored,
               league_stats->runs_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *league_stats_t_create__ShouldInsertRecordsInTheLeagueStatsTTable()
{
     league_stats_s expected = { 0 };

     expected.league_id    = 3;
     expected.season       = 2;
     expected.season_phase = sp_Regular;
     expected.wins         = 522;
     expected.losses       = 502;
     expected.home_wins    = 268;
     expected.home_losses  = 244;
     expected.road_wins    = 254;
     expected.road_losses  = 258;
     expected.runs_scored  = 3482;
     expected.runs_allowed = 3246;

     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected ) );

     league_stats_s *actual = get_a_league_stats( expected.league_id );

     assertNotNull( actual );

     assertEquals( expected.league_id,    actual->league_id    );
     assertEquals( expected.season,       actual->season       );
     assertEquals( expected.season_phase, actual->season_phase );
     assertEquals( expected.wins,         actual->wins         );
     assertEquals( expected.losses,       actual->losses       );
     assertEquals( expected.home_wins,    actual->home_wins    );
     assertEquals( expected.home_losses,  actual->home_losses  );
     assertEquals( expected.road_wins,    actual->road_wins    );
     assertEquals( expected.road_losses,  actual->road_losses  );
     assertEquals( expected.runs_scored,  actual->runs_scored  );
     assertEquals( expected.runs_allowed, actual->runs_allowed );

     sqlite3_exec( db, "delete from league_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_stats_t_create__ShouldGiveAnErrorIfRecordForSameLeagueSeasonAndPhaseExists()
{
     league_stats_s expected = { 0 };

     expected.league_id    = 3;
     expected.season       = 2;
     expected.season_phase = sp_Regular;
     expected.wins         = 522;
     expected.losses       = 502;
     expected.home_wins    = 268;
     expected.home_losses  = 244;
     expected.road_wins    = 254;
     expected.road_losses  = 258;
     expected.runs_scored  = 3482;
     expected.runs_allowed = 3246;

     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected ) );

     expected.wins         = 532;
     expected.losses       = 492;
     expected.home_wins    = 288;
     expected.home_losses  = 224;
     expected.road_wins    = 244;
     expected.road_losses  = 268;
     expected.runs_scored  = 3552;
     expected.runs_allowed = 3196;

     assertEquals( SQLITE_CONSTRAINT, league_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from league_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheLeagueIdSeasonAndPhase()
{
     league_stats_s expected = { 0 };

     expected.league_id    = 3;
     expected.season       = 2;
     expected.season_phase = sp_Regular;
     expected.wins         = 522;
     expected.losses       = 502;
     expected.home_wins    = 268;
     expected.home_losses  = 244;
     expected.road_wins    = 254;
     expected.road_losses  = 258;
     expected.runs_scored  = 3482;
     expected.runs_allowed = 3246;

     insert_a_league_stats( &expected );

     league_stats_s actual = { 0 };

     actual.league_id    = expected.league_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( SQLITE_OK, league_stats_t_read( db, &actual ) );

     assertEquals( expected.league_id,    actual.league_id    );
     assertEquals( expected.season,       actual.season       );
     assertEquals( expected.season_phase, actual.season_phase );
     assertEquals( expected.wins,         actual.wins         );
     assertEquals( expected.losses,       actual.losses       );
     assertEquals( expected.home_wins,    actual.home_wins    );
     assertEquals( expected.home_losses,  actual.home_losses  );
     assertEquals( expected.road_wins,    actual.road_wins    );
     assertEquals( expected.road_losses,  actual.road_losses  );
     assertEquals( expected.runs_scored,  actual.runs_scored  );
     assertEquals( expected.runs_allowed, actual.runs_allowed );

     sqlite3_exec( db, "delete from league_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_stats_t_update__ShouldModifyMatchingRecord_GivenTheLeagueIdSeasonAndPhase()
{
     league_stats_s expected = { 0 };

     expected.league_id    = 3;
     expected.season       = 2;
     expected.season_phase = sp_Regular;
     expected.wins         = 522;
     expected.losses       = 502;
     expected.home_wins    = 268;
     expected.home_losses  = 244;
     expected.road_wins    = 254;
     expected.road_losses  = 258;
     expected.runs_scored  = 3482;
     expected.runs_allowed = 3246;

     insert_a_league_stats( &expected );

     expected.wins         = 532;
     expected.losses       = 492;
     expected.home_wins    = 288;
     expected.home_losses  = 224;
     expected.road_wins    = 244;
     expected.road_losses  = 268;
     expected.runs_scored  = 3552;
     expected.runs_allowed = 3196;

     assertEquals( SQLITE_OK, league_stats_t_update( db, &expected ) );

     league_stats_s *actual = get_a_league_stats( expected.league_id );

     assertNotNull( actual );

     assertEquals( expected.league_id,    actual->league_id    );
     assertEquals( expected.season,       actual->season       );
     assertEquals( expected.season_phase, actual->season_phase );
     assertEquals( expected.wins,         actual->wins         );
     assertEquals( expected.losses,       actual->losses       );
     assertEquals( expected.home_wins,    actual->home_wins    );
     assertEquals( expected.home_losses,  actual->home_losses  );
     assertEquals( expected.road_wins,    actual->road_wins    );
     assertEquals( expected.road_losses,  actual->road_losses  );
     assertEquals( expected.runs_scored,  actual->runs_scored  );
     assertEquals( expected.runs_allowed, actual->runs_allowed );

     sqlite3_exec( db, "delete from league_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *league_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheLeagueIdSeasonAndPhase()
{
     league_stats_s expected = { 0 };

     expected.league_id    = 3;
     expected.season       = 2;
     expected.season_phase = sp_Regular;
     expected.wins         = 522;
     expected.losses       = 502;
     expected.home_wins    = 268;
     expected.home_losses  = 244;
     expected.road_wins    = 254;
     expected.road_losses  = 258;
     expected.runs_scored  = 3482;
     expected.runs_allowed = 3246;

     insert_a_league_stats( &expected );

     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected ) );

     league_stats_s *actual = get_a_league_stats( expected.league_id );

     assertNull( actual );

     sqlite3_exec( db, "delete from league_stats_t", NULL, NULL, NULL );

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
     run_test( league_stats_t_create__ShouldInsertRecordsInTheLeagueStatsTTable,                  check_sqlite_error );
     run_test( league_stats_t_create__ShouldGiveAnErrorIfRecordForSameLeagueSeasonAndPhaseExists, check_sqlite_error );
     run_test( league_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheLeagueIdSeasonAndPhase,  check_sqlite_error );
     run_test( league_stats_t_update__ShouldModifyMatchingRecord_GivenTheLeagueIdSeasonAndPhase,  check_sqlite_error );
     run_test( league_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheLeagueIdSeasonAndPhase,  check_sqlite_error );
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
