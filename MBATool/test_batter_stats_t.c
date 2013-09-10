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

     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected ) );

     batter_stats_s *actual = get_a_batter_stats( expected.player_id );

     assertEquals( expected.player_id,      actual->player_id      );
     assertEquals( expected.season,         actual->season         );
     assertEquals( expected.season_phase,   actual->season_phase   );
     assertEquals( expected.games,          actual->games          );
     assertEquals( expected.at_bats,        actual->at_bats        );
     assertEquals( expected.runs,           actual->runs           );
     assertEquals( expected.hits,           actual->hits           );
     assertEquals( expected.doubles,        actual->doubles        );
     assertEquals( expected.triples,        actual->triples        );
     assertEquals( expected.home_runs,      actual->home_runs      );
     assertEquals( expected.runs_batted_in, actual->runs_batted_in );
     assertEquals( expected.walks,          actual->walks          );
     assertEquals( expected.strike_outs,    actual->strike_outs    );
     assertEquals( expected.steals,         actual->steals         );
     assertEquals( expected.errors,         actual->errors         );

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

     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected ) );

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

     assertEquals( SQLITE_CONSTRAINT, batter_stats_t_create( db, &expected ) );

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

     assertEquals( SQLITE_OK, batter_stats_t_read( db, &actual ) );

     assertEquals( expected.player_id,      actual.player_id      );
     assertEquals( expected.season,         actual.season         );
     assertEquals( expected.season_phase,   actual.season_phase   );
     assertEquals( expected.games,          actual.games          );
     assertEquals( expected.at_bats,        actual.at_bats        );
     assertEquals( expected.runs,           actual.runs           );
     assertEquals( expected.hits,           actual.hits           );
     assertEquals( expected.doubles,        actual.doubles        );
     assertEquals( expected.triples,        actual.triples        );
     assertEquals( expected.home_runs,      actual.home_runs      );
     assertEquals( expected.runs_batted_in, actual.runs_batted_in );
     assertEquals( expected.walks,          actual.walks          );
     assertEquals( expected.strike_outs,    actual.strike_outs    );
     assertEquals( expected.steals,         actual.steals         );
     assertEquals( expected.errors,         actual.errors         );

     sqlite3_exec( db, "delete from batter_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *batter_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId()
{
     batter_stats_s expected1 = { 0 };
     batter_stats_s expected2 = { 0 };

     expected1.player_id      = 123;
     expected1.season         = 7;
     expected1.season_phase   = sp_Playoff;
     expected1.games          = 134;
     expected1.at_bats        = 543;
     expected1.runs           = 101;
     expected1.hits           = 220;
     expected1.doubles        = 35;
     expected1.triples        = 16;
     expected1.home_runs      = 56;
     expected1.runs_batted_in = 115;
     expected1.walks          = 62;
     expected1.strike_outs    = 131;
     expected1.steals         = 45;
     expected1.errors         = 5;

     expected2.player_id      = 123;
     expected2.season         = 8;
     expected2.season_phase   = sp_Playoff;
     expected2.games          = 143;
     expected2.at_bats        = 555;
     expected2.runs           = 99;
     expected2.hits           = 206;
     expected2.doubles        = 22;
     expected2.triples        = 11;
     expected2.home_runs      = 44;
     expected2.runs_batted_in = 109;
     expected2.walks          = 58;
     expected2.strike_outs    = 144;
     expected2.steals         = 37;
     expected2.errors         = 8;

     insert_a_batter_stats( &expected1 );
     insert_a_batter_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, batter_stats_t_read_by_player( db, expected1.player_id, &list ) );

     assertEquals( 2, list.count );

     batter_stats_s *actual = list.data;

     assertNotNull( actual );

     assertEquals( expected1.player_id,      actual[0].player_id      );
     assertEquals( expected1.season,         actual[0].season         );
     assertEquals( expected1.season_phase,   actual[0].season_phase   );
     assertEquals( expected1.games,          actual[0].games          );
     assertEquals( expected1.at_bats,        actual[0].at_bats        );
     assertEquals( expected1.runs,           actual[0].runs           );
     assertEquals( expected1.hits,           actual[0].hits           );
     assertEquals( expected1.doubles,        actual[0].doubles        );
     assertEquals( expected1.triples,        actual[0].triples        );
     assertEquals( expected1.home_runs,      actual[0].home_runs      );
     assertEquals( expected1.runs_batted_in, actual[0].runs_batted_in );
     assertEquals( expected1.walks,          actual[0].walks          );
     assertEquals( expected1.strike_outs,    actual[0].strike_outs    );
     assertEquals( expected1.steals,         actual[0].steals         );
     assertEquals( expected1.errors,         actual[0].errors         );

     assertEquals( expected2.player_id,      actual[1].player_id      );
     assertEquals( expected2.season,         actual[1].season         );
     assertEquals( expected2.season_phase,   actual[1].season_phase   );
     assertEquals( expected2.games,          actual[1].games          );
     assertEquals( expected2.at_bats,        actual[1].at_bats        );
     assertEquals( expected2.runs,           actual[1].runs           );
     assertEquals( expected2.hits,           actual[1].hits           );
     assertEquals( expected2.doubles,        actual[1].doubles        );
     assertEquals( expected2.triples,        actual[1].triples        );
     assertEquals( expected2.home_runs,      actual[1].home_runs      );
     assertEquals( expected2.runs_batted_in, actual[1].runs_batted_in );
     assertEquals( expected2.walks,          actual[1].walks          );
     assertEquals( expected2.strike_outs,    actual[1].strike_outs    );
     assertEquals( expected2.steals,         actual[1].steals         );
     assertEquals( expected2.errors,         actual[1].errors         );

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

     assertEquals( SQLITE_OK, batter_stats_t_update( db, &expected ) );

     batter_stats_s *actual = get_a_batter_stats( expected.player_id );

     assertEquals( expected.player_id,      actual->player_id      );
     assertEquals( expected.season,         actual->season         );
     assertEquals( expected.season_phase,   actual->season_phase   );
     assertEquals( expected.games,          actual->games          );
     assertEquals( expected.at_bats,        actual->at_bats        );
     assertEquals( expected.runs,           actual->runs           );
     assertEquals( expected.hits,           actual->hits           );
     assertEquals( expected.doubles,        actual->doubles        );
     assertEquals( expected.triples,        actual->triples        );
     assertEquals( expected.home_runs,      actual->home_runs      );
     assertEquals( expected.runs_batted_in, actual->runs_batted_in );
     assertEquals( expected.walks,          actual->walks          );
     assertEquals( expected.strike_outs,    actual->strike_outs    );
     assertEquals( expected.steals,         actual->steals         );
     assertEquals( expected.errors,         actual->errors         );

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

     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected ) );

     assertNull( get_a_batter_stats( expected.player_id ) );

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
     run_test( batter_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId,  check_sqlite_error );
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
