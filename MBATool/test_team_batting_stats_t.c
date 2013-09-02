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

     assertEquals( "team_batting_stats_t_create()", SQLITE_OK, team_batting_stats_t_create( db, &expected ) );

     team_batting_stats_s *actual = get_a_team_batting_stats( expected.team_id );

     assertEquals( "team_id",        expected.team_id,        actual->team_id        );
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

     assertEquals( "team_batting_stats_t_create()", SQLITE_OK, team_batting_stats_t_create( db, &expected ) );

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

     assertEquals( "team_batting_stats_t_create()", SQLITE_CONSTRAINT, team_batting_stats_t_create( db, &expected ) );

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

     assertEquals( "team_batting_stats_t_read()", SQLITE_OK, team_batting_stats_t_read( db, &actual ) );

     assertEquals( "team_id",        expected.team_id,        actual.team_id        );
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

     assertEquals( "team_batting_stats_t_update()", SQLITE_OK, team_batting_stats_t_update( db, &expected ) );

     team_batting_stats_s *actual = get_a_team_batting_stats( expected.team_id );

     assertEquals( "team_id",        expected.team_id,        actual->team_id        );
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

     assertEquals( "team_batting_stats_t_delete()", SQLITE_OK, team_batting_stats_t_delete( db, &expected ) );

     team_batting_stats_s *actual = get_a_team_batting_stats( expected.team_id );

     assertEquals( "actual", NULL, actual );

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
