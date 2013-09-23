#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_team_batting_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_batting_stats_s team_batting_stats;

     team_batting_stats_s **tbs = (team_batting_stats_s **)output;

     if ( cols < 15 ) return SQLITE_ERROR;

     memset( &team_batting_stats, '\0', sizeof(team_batting_stats_s) );

     team_batting_stats.team_id        = atoi( data[ 0] );
     team_batting_stats.season         = atoi( data[ 1] );
     team_batting_stats.season_phase   = atoi( data[ 2] );
     team_batting_stats.games          = atoi( data[ 3] );
     team_batting_stats.at_bats        = atoi( data[ 4] );
     team_batting_stats.runs           = atoi( data[ 5] );
     team_batting_stats.hits           = atoi( data[ 6] );
     team_batting_stats.doubles        = atoi( data[ 7] );
     team_batting_stats.triples        = atoi( data[ 8] );
     team_batting_stats.home_runs      = atoi( data[ 9] );
     team_batting_stats.runs_batted_in = atoi( data[10] );
     team_batting_stats.walks          = atoi( data[11] );
     team_batting_stats.strike_outs    = atoi( data[12] );
     team_batting_stats.steals         = atoi( data[13] );
     team_batting_stats.errors         = atoi( data[14] );

     *tbs = &team_batting_stats;

     return SQLITE_OK;
}

static team_batting_stats_s *get_a_team_batting_stats( int team_id )
{
     team_batting_stats_s *team_batting_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_batting_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_batting_stats_data, &team_batting_stats, NULL );

     return team_batting_stats;
}


static void insert_a_team_batting_stats( team_batting_stats_s *team_batting_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_batting_stats_t (team_id, season, season_phase, games, at_bats, runs, hits, doubles, triples, home_runs, runs_batted_in, walks, strike_outs, steals, errors)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               team_batting_stats->team_id,
               team_batting_stats->season,
               team_batting_stats->season_phase,
               team_batting_stats->games,
               team_batting_stats->at_bats,
               team_batting_stats->runs,
               team_batting_stats->hits,
               team_batting_stats->doubles,
               team_batting_stats->triples,
               team_batting_stats->home_runs,
               team_batting_stats->runs_batted_in,
               team_batting_stats->walks,
               team_batting_stats->strike_outs,
               team_batting_stats->steals,
               team_batting_stats->errors );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_batting_stats_t_create__ShouldInsertRecordsInTheTeamBattingStatsTTable()
{
     team_batting_stats_s expected = { 0 };

     expected.team_id        = 123;
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

     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected ) );

     team_batting_stats_s *actual = get_a_team_batting_stats( expected.team_id );

     assertEquals( expected.team_id,        actual->team_id        );
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

     sqlite3_exec( db, "delete from team_batting_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_batting_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndPhaseExists()
{
     team_batting_stats_s expected = { 0 };

     expected.team_id        = 123;
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

     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected ) );

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

     assertEquals( SQLITE_CONSTRAINT, team_batting_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_batting_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_batting_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_batting_stats_s expected = { 0 };

     expected.team_id        = 123;
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

     insert_a_team_batting_stats( &expected );

     team_batting_stats_s actual = { 0 };

     actual.team_id      = expected.team_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( SQLITE_OK, team_batting_stats_t_read( db, &actual ) );

     assertEquals( expected.team_id,        actual.team_id        );
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

     sqlite3_exec( db, "delete from team_batting_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_batting_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_batting_stats_s expected1 = { 0 };
     team_batting_stats_s expected2 = { 0 };

     expected1.team_id        = 123;
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

     expected2.team_id        = 123;
     expected2.season         = 10;
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

     insert_a_team_batting_stats( &expected1 );
     insert_a_team_batting_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_batting_stats_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( 2, list.count );

     team_batting_stats_s *actual = list.data;

     assertNotNull( actual );

     assertEquals( expected1.team_id,        actual[0].team_id        );
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

     assertEquals( expected2.team_id,        actual[1].team_id        );
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

     free( actual );

     sqlite3_exec( db, "delete from team_batting_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_batting_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_batting_stats_s expected = { 0 };

     expected.team_id        = 123;
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

     insert_a_team_batting_stats( &expected );

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

     assertEquals( SQLITE_OK, team_batting_stats_t_update( db, &expected ) );

     team_batting_stats_s *actual = get_a_team_batting_stats( expected.team_id );

     assertEquals( expected.team_id,        actual->team_id        );
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

     sqlite3_exec( db, "delete from team_batting_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_batting_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_batting_stats_s expected = { 0 };

     expected.team_id        = 123;
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

     insert_a_team_batting_stats( &expected );

     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected ) );

     assertNull( get_a_team_batting_stats( expected.team_id ) );

     sqlite3_exec( db, "delete from team_batting_stats_t", NULL, NULL, NULL );

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
     run_test( team_batting_stats_t_create__ShouldInsertRecordsInTheTeamBattingStatsTTable,           check_sqlite_error );
     run_test( team_batting_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndPhaseExists, check_sqlite_error );
     run_test( team_batting_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
     run_test( team_batting_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,    check_sqlite_error );
     run_test( team_batting_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
     run_test( team_batting_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
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
