#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_batter_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static batter_stats_s batter_stats;

     batter_stats_s **bs = (batter_stats_s **)output;

     if ( cols < 15 ) return SQLITE_ERROR;

     memset( &batter_stats, '\0', sizeof(batter_stats_s) );

     batter_stats.player_id      = atoi( data[ 0] );
     batter_stats.season         = atoi( data[ 1] );
     batter_stats.season_phase   = atoi( data[ 2] );
     batter_stats.games          = atoi( data[ 3] );
     batter_stats.at_bats        = atoi( data[ 4] );
     batter_stats.runs           = atoi( data[ 5] );
     batter_stats.hits           = atoi( data[ 6] );
     batter_stats.doubles        = atoi( data[ 7] );
     batter_stats.triples        = atoi( data[ 8] );
     batter_stats.home_runs      = atoi( data[ 9] );
     batter_stats.runs_batted_in = atoi( data[10] );
     batter_stats.walks          = atoi( data[11] );
     batter_stats.strike_outs    = atoi( data[12] );
     batter_stats.steals         = atoi( data[13] );
     batter_stats.errors         = atoi( data[14] );

     *bs = &batter_stats;

     return SQLITE_OK;
}

static batter_stats_s *get_a_batter_stats( int player_id )
{
     batter_stats_s *batter_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from batter_stats_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_batter_stats_data, &batter_stats, NULL );

     return batter_stats;
}


static void insert_a_batter_stats( batter_stats_s *batter_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into batter_stats_t (player_id, season, season_phase, games, at_bats, runs, hits, doubles, triples, home_runs, runs_batted_in, walks, strike_outs, steals, errors)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               batter_stats->player_id,
               batter_stats->season,
               batter_stats->season_phase,
               batter_stats->games,
               batter_stats->at_bats,
               batter_stats->runs,
               batter_stats->hits,
               batter_stats->doubles,
               batter_stats->triples,
               batter_stats->home_runs,
               batter_stats->runs_batted_in,
               batter_stats->walks,
               batter_stats->strike_outs,
               batter_stats->steals,
               batter_stats->errors );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *batter_stats_t_create__ShouldInsertRecordsInTheBatterStatsTTable()
{
     batter_stats_s expected = { 0 };

     expected.player_id      = 123;
     expected.season         = 7;
     expected.season_phase   = sp_Playoff;
     expected.games          = 134;
     expected.at_bats        = 543;
     expected.runs           = 101;
     expected.hits           = 220;
     expected.doubles        = 35;
     expected.triples        = 16;
     expected.home_runs      = 56;
     expected.runs_batted_in = 115;
     expected.walks          = 62;
     expected.strike_outs    = 131;
     expected.steals         = 45;
     expected.errors         = 5;

     assertEquals( "batter_stats_t_create()", SQLITE_OK, batter_stats_t_create( db, &expected ) );

     batter_stats_s *actual = get_a_batter_stats( expected.player_id );

     assertEquals( "player_id",      expected.player_id,      actual->player_id      );
     assertEquals( "season",         expected.season,         actual->season         );
     assertEquals( "season_phase",   expected.season_phase,   actual->season_phase   );
     assertEquals( "games",          expected.games,          actual->games          );
     assertEquals( "at_bats",        expected.at_bats,        actual->at_bats        );
     assertEquals( "runs",           expected.runs,           actual->runs           );
     assertEquals( "hits",           expected.hits,           actual->hits           );
     assertEquals( "doubles",        expected.doubles,        actual->doubles        );
     assertEquals( "triples",        expected.triples,        actual->triples        );
     assertEquals( "home_runs",      expected.home_runs,      actual->home_runs      );
     assertEquals( "runs_batted_in", expected.runs_batted_in, actual->runs_batted_in );
     assertEquals( "walks",          expected.walks,          actual->walks          );
     assertEquals( "strike_outs",    expected.strike_outs,    actual->strike_outs    );
     assertEquals( "steals",         expected.steals,         actual->steals         );
     assertEquals( "errors",         expected.errors,         actual->errors         );

     sqlite3_exec( db, "delete from batter_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *batter_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndPhaseExists()
{
     batter_stats_s expected = { 0 };

     expected.player_id      = 123;
     expected.season         = 7;
     expected.season_phase   = sp_Playoff;
     expected.games          = 134;
     expected.at_bats        = 543;
     expected.runs           = 101;
     expected.hits           = 220;
     expected.doubles        = 35;
     expected.triples        = 16;
     expected.home_runs      = 56;
     expected.runs_batted_in = 115;
     expected.walks          = 62;
     expected.strike_outs    = 131;
     expected.steals         = 45;
     expected.errors         = 5;

     assertEquals( "batter_stats_t_create()", SQLITE_OK, batter_stats_t_create( db, &expected ) );

     expected.games          = 143;
     expected.at_bats        = 555;
     expected.runs           = 99;
     expected.hits           = 206;
     expected.doubles        = 22;
     expected.triples        = 11;
     expected.home_runs      = 44;
     expected.runs_batted_in = 109;
     expected.walks          = 58;
     expected.strike_outs    = 144;
     expected.steals         = 37;
     expected.errors         = 8;

     assertEquals( "batter_stats_t_create()", SQLITE_CONSTRAINT, batter_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from batter_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *batter_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     batter_stats_s expected = { 0 };

     expected.player_id      = 123;
     expected.season         = 7;
     expected.season_phase   = sp_Playoff;
     expected.games          = 134;
     expected.at_bats        = 543;
     expected.runs           = 101;
     expected.hits           = 220;
     expected.doubles        = 35;
     expected.triples        = 16;
     expected.home_runs      = 56;
     expected.runs_batted_in = 115;
     expected.walks          = 62;
     expected.strike_outs    = 131;
     expected.steals         = 45;
     expected.errors         = 5;

     insert_a_batter_stats( &expected );

     batter_stats_s actual = { 0 };

     actual.player_id    = expected.player_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( "batter_stats_t_read()", SQLITE_OK, batter_stats_t_read( db, &actual ) );

     assertEquals( "player_id",      expected.player_id,      actual.player_id      );
     assertEquals( "season",         expected.season,         actual.season         );
     assertEquals( "season_phase",   expected.season_phase,   actual.season_phase   );
     assertEquals( "games",          expected.games,          actual.games          );
     assertEquals( "at_bats",        expected.at_bats,        actual.at_bats        );
     assertEquals( "runs",           expected.runs,           actual.runs           );
     assertEquals( "hits",           expected.hits,           actual.hits           );
     assertEquals( "doubles",        expected.doubles,        actual.doubles        );
     assertEquals( "triples",        expected.triples,        actual.triples        );
     assertEquals( "home_runs",      expected.home_runs,      actual.home_runs      );
     assertEquals( "runs_batted_in", expected.runs_batted_in, actual.runs_batted_in );
     assertEquals( "walks",          expected.walks,          actual.walks          );
     assertEquals( "strike_outs",    expected.strike_outs,    actual.strike_outs    );
     assertEquals( "steals",         expected.steals,         actual.steals         );
     assertEquals( "errors",         expected.errors,         actual.errors         );

     sqlite3_exec( db, "delete from batter_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *batter_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     batter_stats_s expected = { 0 };

     expected.player_id      = 123;
     expected.season         = 7;
     expected.season_phase   = sp_Playoff;
     expected.games          = 134;
     expected.at_bats        = 543;
     expected.runs           = 101;
     expected.hits           = 220;
     expected.doubles        = 35;
     expected.triples        = 16;
     expected.home_runs      = 56;
     expected.runs_batted_in = 115;
     expected.walks          = 62;
     expected.strike_outs    = 131;
     expected.steals         = 45;
     expected.errors         = 5;

     insert_a_batter_stats( &expected );

     expected.games          = 143;
     expected.at_bats        = 555;
     expected.runs           = 99;
     expected.hits           = 206;
     expected.doubles        = 22;
     expected.triples        = 11;
     expected.home_runs      = 44;
     expected.runs_batted_in = 109;
     expected.walks          = 58;
     expected.strike_outs    = 144;
     expected.steals         = 37;
     expected.errors         = 8;

     assertEquals( "batter_stats_t_update()", SQLITE_OK, batter_stats_t_update( db, &expected ) );

     batter_stats_s *actual = get_a_batter_stats( expected.player_id );

     assertEquals( "player_id",      expected.player_id,      actual->player_id      );
     assertEquals( "season",         expected.season,         actual->season         );
     assertEquals( "season_phase",   expected.season_phase,   actual->season_phase   );
     assertEquals( "games",          expected.games,          actual->games          );
     assertEquals( "at_bats",        expected.at_bats,        actual->at_bats        );
     assertEquals( "runs",           expected.runs,           actual->runs           );
     assertEquals( "hits",           expected.hits,           actual->hits           );
     assertEquals( "doubles",        expected.doubles,        actual->doubles        );
     assertEquals( "triples",        expected.triples,        actual->triples        );
     assertEquals( "home_runs",      expected.home_runs,      actual->home_runs      );
     assertEquals( "runs_batted_in", expected.runs_batted_in, actual->runs_batted_in );
     assertEquals( "walks",          expected.walks,          actual->walks          );
     assertEquals( "strike_outs",    expected.strike_outs,    actual->strike_outs    );
     assertEquals( "steals",         expected.steals,         actual->steals         );
     assertEquals( "errors",         expected.errors,         actual->errors         );

     sqlite3_exec( db, "delete from batter_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *batter_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndPhase()
{
     batter_stats_s expected = { 0 };

     expected.player_id      = 123;
     expected.season         = 7;
     expected.season_phase   = sp_Playoff;
     expected.games          = 134;
     expected.at_bats        = 543;
     expected.runs           = 101;
     expected.hits           = 220;
     expected.doubles        = 35;
     expected.triples        = 16;
     expected.home_runs      = 56;
     expected.runs_batted_in = 115;
     expected.walks          = 62;
     expected.strike_outs    = 131;
     expected.steals         = 45;
     expected.errors         = 5;

     insert_a_batter_stats( &expected );

     assertEquals( "batter_stats_t_delete()", SQLITE_OK, batter_stats_t_delete( db, &expected ) );

     assertNull( "get_a_batter_stats()", get_a_batter_stats( expected.player_id ) );

     sqlite3_exec( db, "delete from batter_stats_t", NULL, NULL, NULL );

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
     run_test( batter_stats_t_create__ShouldInsertRecordsInTheBatterStatsTTable,                  check_sqlite_error );
     run_test( batter_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndPhaseExists, check_sqlite_error );
     run_test( batter_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndPhase,  check_sqlite_error );
     run_test( batter_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndPhase,  check_sqlite_error );
     run_test( batter_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndPhase,  check_sqlite_error );
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
