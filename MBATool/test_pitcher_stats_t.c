#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_pitcher_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static pitcher_stats_s pitcher_stats;

     pitcher_stats_s **ps = (pitcher_stats_s **)output;

     if ( cols < 13 ) return SQLITE_ERROR;

     memset( &pitcher_stats, '\0', sizeof(pitcher_stats_s) );

     /**/    pitcher_stats.player_id    = atoi( data[ 0] );
     /**/    pitcher_stats.season       = atoi( data[ 1] );
     /**/    pitcher_stats.season_phase = atoi( data[ 2] );
     /**/    pitcher_stats.wins         = atoi( data[ 3] );
     /**/    pitcher_stats.losses       = atoi( data[ 4] );
     /**/    pitcher_stats.games        = atoi( data[ 5] );
     /**/    pitcher_stats.saves        = atoi( data[ 6] );
     /**/    pitcher_stats.innings      = atof( data[ 7] );
     /**/    pitcher_stats.hits         = atoi( data[ 8] );
     /**/    pitcher_stats.earned_runs  = atoi( data[ 9] );
     /**/    pitcher_stats.home_runs    = atoi( data[10] );
     /**/    pitcher_stats.walks        = atoi( data[11] );
     /**/    pitcher_stats.strike_outs  = atoi( data[12] );

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


     snprintf( query, sizeof(query), "insert into pitcher_stats_t (player_id, season, season_phase, wins, losses, games, saves, innings, hits, earned_runs, home_runs, walks, strike_outs)"
               "values (%d, %d, %d, %d, %d, %d, %d, %.1f, %d, %d, %d, %d, %d)",
               pitcher_stats->player_id,
               pitcher_stats->season,
               pitcher_stats->season_phase,
               pitcher_stats->wins,
               pitcher_stats->losses,
               pitcher_stats->games,
               pitcher_stats->saves,
               pitcher_stats->innings,
               pitcher_stats->hits,
               pitcher_stats->earned_runs,
               pitcher_stats->home_runs,
               pitcher_stats->walks,
               pitcher_stats->strike_outs );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *pitcher_stats_t_create__ShouldInsertRecordsInThePitcherStatsTTable()
{
     pitcher_stats_s expected = { 0 };

     expected.player_id    = 123;
     expected.season       = 5;
     expected.season_phase = sp_Regular;
     expected.wins         = 12;
     expected.losses       = 15;
     expected.games        = 54;
     expected.saves        = 11;
     expected.innings      = 321.2;
     expected.hits         = 165;
     expected.earned_runs  = 23;
     expected.home_runs    = 14;
     expected.walks        = 25;
     expected.strike_outs  = 234;

     assertEquals( "pitcher_stats_t_create()", SQLITE_OK, pitcher_stats_t_create( db, &expected ) );

     pitcher_stats_s *actual = get_a_pitcher_stats( expected.player_id );

     assertEquals( "player_id",    expected.player_id,    actual->player_id    );
     assertEquals( "season",       expected.season,       actual->season       );
     assertEquals( "season_phase", expected.season_phase, actual->season_phase );
     assertEquals( "wins",         expected.wins,         actual->wins         );
     assertEquals( "losses",       expected.losses,       actual->losses       );
     assertEquals( "games",        expected.games,        actual->games        );
     assertEquals( "saves",        expected.saves,        actual->saves        );
     assertEquals( "innings",      expected.innings,      actual->innings      );
     assertEquals( "hits",         expected.hits,         actual->hits         );
     assertEquals( "earned_runs",  expected.earned_runs,  actual->earned_runs  );
     assertEquals( "home_runs",    expected.home_runs,    actual->home_runs    );
     assertEquals( "walks",        expected.walks,        actual->walks        );
     assertEquals( "strike_outs",  expected.strike_outs,  actual->strike_outs  );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     return NULL;
}


static char *pitcher_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndPhaseExists()
{
     pitcher_stats_s expected = { 0 };

     expected.player_id    = 123;
     expected.season       = 5;
     expected.season_phase = sp_Regular;
     expected.wins         = 12;
     expected.losses       = 15;
     expected.games        = 54;
     expected.saves        = 11;
     expected.innings      = 321.2;
     expected.hits         = 165;
     expected.earned_runs  = 23;
     expected.home_runs    = 14;
     expected.walks        = 25;
     expected.strike_outs  = 234;

     assertEquals( "pitcher_stats_t_create()", SQLITE_OK, pitcher_stats_t_create( db, &expected ) );

     expected.wins         = 20;
     expected.losses       = 9;
     expected.games        = 34;
     expected.saves        = 6;
     expected.innings      = 222.1;
     expected.hits         = 200;
     expected.earned_runs  = 40;
     expected.home_runs    = 16;
     expected.walks        = 30;
     expected.strike_outs  = 250;

     assertEquals( "pitcher_stats_t_create()", SQLITE_CONSTRAINT, pitcher_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     return NULL;
}


static char *pitcher_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     pitcher_stats_s expected = { 0 };

     expected.player_id    = 123;
     expected.season       = 5;
     expected.season_phase = sp_Regular;
     expected.wins         = 12;
     expected.losses       = 15;
     expected.games        = 54;
     expected.saves        = 11;
     expected.innings      = 321.2;
     expected.hits         = 165;
     expected.earned_runs  = 23;
     expected.home_runs    = 14;
     expected.walks        = 25;
     expected.strike_outs  = 234;

     insert_a_pitcher_stats( &expected );

     pitcher_stats_s actual = { 0 };

     actual.player_id    = expected.player_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( "pitcher_stats_t_read()", SQLITE_OK, pitcher_stats_t_read( db, &actual ) );

     assertEquals( "player_id",    expected.player_id,    actual.player_id    );
     assertEquals( "season",       expected.season,       actual.season       );
     assertEquals( "season_phase", expected.season_phase, actual.season_phase );
     assertEquals( "wins",         expected.wins,         actual.wins         );
     assertEquals( "losses",       expected.losses,       actual.losses       );
     assertEquals( "games",        expected.games,        actual.games        );
     assertEquals( "saves",        expected.saves,        actual.saves        );
     assertEquals( "innings",      expected.innings,      actual.innings      );
     assertEquals( "hits",         expected.hits,         actual.hits         );
     assertEquals( "earned_runs",  expected.earned_runs,  actual.earned_runs  );
     assertEquals( "home_runs",    expected.home_runs,    actual.home_runs    );
     assertEquals( "walks",        expected.walks,        actual.walks        );
     assertEquals( "strike_outs",  expected.strike_outs,  actual.strike_outs  );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *pitcher_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     pitcher_stats_s expected = { 0 };

     expected.player_id    = 123;
     expected.season       = 5;
     expected.season_phase = sp_Regular;
     expected.wins         = 12;
     expected.losses       = 15;
     expected.games        = 54;
     expected.saves        = 11;
     expected.innings      = 321.2;
     expected.hits         = 165;
     expected.earned_runs  = 23;
     expected.home_runs    = 14;
     expected.walks        = 25;
     expected.strike_outs  = 234;

     insert_a_pitcher_stats( &expected );

     expected.wins         = 21;
     expected.losses       = 51;
     expected.games        = 45;
     expected.saves        = 99;
     expected.innings      = 123.1;
     expected.hits         = 156;
     expected.earned_runs  = 32;
     expected.home_runs    = 41;
     expected.walks        = 52;
     expected.strike_outs  = 321;

     assertEquals( "pitcher_stats_t_update()", SQLITE_OK, pitcher_stats_t_update( db, &expected ) );

     pitcher_stats_s *actual = get_a_pitcher_stats( expected.player_id );

     assertEquals( "player_id",    expected.player_id,    actual->player_id    );
     assertEquals( "season",       expected.season,       actual->season       );
     assertEquals( "season_phase", expected.season_phase, actual->season_phase );
     assertEquals( "wins",         expected.wins,         actual->wins         );
     assertEquals( "losses",       expected.losses,       actual->losses       );
     assertEquals( "games",        expected.games,        actual->games        );
     assertEquals( "saves",        expected.saves,        actual->saves        );
     assertEquals( "innings",      expected.innings,      actual->innings      );
     assertEquals( "hits",         expected.hits,         actual->hits         );
     assertEquals( "earned_runs",  expected.earned_runs,  actual->earned_runs  );
     assertEquals( "home_runs",    expected.home_runs,    actual->home_runs    );
     assertEquals( "walks",        expected.walks,        actual->walks        );
     assertEquals( "strike_outs",  expected.strike_outs,  actual->strike_outs  );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char* pitcher_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     pitcher_stats_s expected = { 0 };

     expected.player_id    = 123;
     expected.season       = 5;
     expected.season_phase = sp_Regular;
     expected.wins         = 12;
     expected.losses       = 15;
     expected.games        = 54;
     expected.saves        = 11;
     expected.innings      = 321.2;
     expected.hits         = 165;
     expected.earned_runs  = 23;
     expected.home_runs    = 14;
     expected.walks        = 25;
     expected.strike_outs  = 234;

     insert_a_pitcher_stats( &expected );

     assertEquals( "pitcher_stats_t_delete()", SQLITE_OK, pitcher_stats_t_delete( db, &expected ) );

     assertEquals( "get_a_pitcher_stats()", NULL, get_a_pitcher_stats( expected.player_id ) );

     sqlite3_exec( db, "delete from pitcher_stats_t", NULL, NULL, NULL );

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
