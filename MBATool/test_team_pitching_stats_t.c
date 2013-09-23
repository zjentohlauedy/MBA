#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_team_pitching_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_pitching_stats_s team_pitching_stats;

     team_pitching_stats_s **tps = (team_pitching_stats_s **)output;

     if ( cols < 13 ) return SQLITE_ERROR;

     memset( &team_pitching_stats, '\0', sizeof(team_pitching_stats_s) );

     /**/    team_pitching_stats.team_id      = atoi( data[ 0] );
     /**/    team_pitching_stats.season       = atoi( data[ 1] );
     /**/    team_pitching_stats.season_phase = atoi( data[ 2] );
     /**/    team_pitching_stats.wins         = atoi( data[ 3] );
     /**/    team_pitching_stats.losses       = atoi( data[ 4] );
     /**/    team_pitching_stats.games        = atoi( data[ 5] );
     /**/    team_pitching_stats.saves        = atoi( data[ 6] );
     /**/    team_pitching_stats.innings      = atof( data[ 7] );
     /**/    team_pitching_stats.hits         = atoi( data[ 8] );
     /**/    team_pitching_stats.earned_runs  = atoi( data[ 9] );
     /**/    team_pitching_stats.home_runs    = atoi( data[10] );
     /**/    team_pitching_stats.walks        = atoi( data[11] );
     /**/    team_pitching_stats.strike_outs  = atoi( data[12] );

     *tps = &team_pitching_stats;

     return SQLITE_OK;
}

static team_pitching_stats_s *get_a_team_pitching_stats( int team_id )
{
     team_pitching_stats_s *team_pitching_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_pitching_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_pitching_stats_data, &team_pitching_stats, NULL );

     return team_pitching_stats;
}


static void insert_a_team_pitching_stats( team_pitching_stats_s *team_pitching_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_pitching_stats_t (team_id, season, season_phase, wins, losses, games, saves, innings, hits, earned_runs, home_runs, walks, strike_outs)"
               "values (%d, %d, %d, %d, %d, %d, %d, %.1f, %d, %d, %d, %d, %d)",
               team_pitching_stats->team_id,
               team_pitching_stats->season,
               team_pitching_stats->season_phase,
               team_pitching_stats->wins,
               team_pitching_stats->losses,
               team_pitching_stats->games,
               team_pitching_stats->saves,
               team_pitching_stats->innings,
               team_pitching_stats->hits,
               team_pitching_stats->earned_runs,
               team_pitching_stats->home_runs,
               team_pitching_stats->walks,
               team_pitching_stats->strike_outs );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_pitching_stats_t_create__ShouldInsertRecordsInTheTeamPitchingStatsTTable()
{
     team_pitching_stats_s expected = { 0 };

     expected.team_id      = 123;
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

     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected ) );

     team_pitching_stats_s *actual = get_a_team_pitching_stats( expected.team_id );

     assertEqualsInt( expected.team_id,      actual->team_id      );
     assertEqualsInt( expected.season,       actual->season       );
     assertEqualsInt( expected.season_phase, actual->season_phase );
     assertEqualsInt( expected.wins,         actual->wins         );
     assertEqualsInt( expected.losses,       actual->losses       );
     assertEqualsInt( expected.games,        actual->games        );
     assertEqualsInt( expected.saves,        actual->saves        );
     assertEqualsDbl( expected.innings,      actual->innings      );
     assertEqualsInt( expected.hits,         actual->hits         );
     assertEqualsInt( expected.earned_runs,  actual->earned_runs  );
     assertEqualsInt( expected.home_runs,    actual->home_runs    );
     assertEqualsInt( expected.walks,        actual->walks        );
     assertEqualsInt( expected.strike_outs,  actual->strike_outs  );

     sqlite3_exec( db, "delete from team_pitching_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_pitching_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndPhaseExists()
{
     team_pitching_stats_s expected = { 0 };

     expected.team_id      = 123;
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

     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected ) );

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

     assertEquals( SQLITE_CONSTRAINT, team_pitching_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_pitching_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_pitching_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_pitching_stats_s expected = { 0 };

     expected.team_id    = 123;
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

     insert_a_team_pitching_stats( &expected );

     team_pitching_stats_s actual = { 0 };

     actual.team_id      = expected.team_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( SQLITE_OK, team_pitching_stats_t_read( db, &actual ) );

     assertEqualsInt( expected.team_id,      actual.team_id      );
     assertEqualsInt( expected.season,       actual.season       );
     assertEqualsInt( expected.season_phase, actual.season_phase );
     assertEqualsInt( expected.wins,         actual.wins         );
     assertEqualsInt( expected.losses,       actual.losses       );
     assertEqualsInt( expected.games,        actual.games        );
     assertEqualsInt( expected.saves,        actual.saves        );
     assertEqualsDbl( expected.innings,      actual.innings      );
     assertEqualsInt( expected.hits,         actual.hits         );
     assertEqualsInt( expected.earned_runs,  actual.earned_runs  );
     assertEqualsInt( expected.home_runs,    actual.home_runs    );
     assertEqualsInt( expected.walks,        actual.walks        );
     assertEqualsInt( expected.strike_outs,  actual.strike_outs  );

     sqlite3_exec( db, "delete from team_pitching_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_pitching_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_pitching_stats_s expected1 = { 0 };
     team_pitching_stats_s expected2 = { 0 };

     expected1.team_id      = 123;
     expected1.season       = 5;
     expected1.season_phase = sp_Regular;
     expected1.wins         = 12;
     expected1.losses       = 15;
     expected1.games        = 54;
     expected1.saves        = 11;
     expected1.innings      = 321.2;
     expected1.hits         = 165;
     expected1.earned_runs  = 23;
     expected1.home_runs    = 14;
     expected1.walks        = 25;
     expected1.strike_outs  = 234;

     expected2.team_id      = 123;
     expected2.season       = 8;
     expected2.season_phase = sp_Regular;
     expected2.wins         = 21;
     expected2.losses       = 51;
     expected2.games        = 45;
     expected2.saves        = 99;
     expected2.innings      = 123.1;
     expected2.hits         = 156;
     expected2.earned_runs  = 32;
     expected2.home_runs    = 41;
     expected2.walks        = 52;
     expected2.strike_outs  = 321;

     insert_a_team_pitching_stats( &expected1 );
     insert_a_team_pitching_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_pitching_stats_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( 2, list.count );

     team_pitching_stats_s *actual = list.data;

     assertNotNull( actual );

     assertEqualsInt( expected1.team_id,      actual[0].team_id      );
     assertEqualsInt( expected1.season,       actual[0].season       );
     assertEqualsInt( expected1.season_phase, actual[0].season_phase );
     assertEqualsInt( expected1.wins,         actual[0].wins         );
     assertEqualsInt( expected1.losses,       actual[0].losses       );
     assertEqualsInt( expected1.games,        actual[0].games        );
     assertEqualsInt( expected1.saves,        actual[0].saves        );
     assertEqualsDbl( expected1.innings,      actual[0].innings      );
     assertEqualsInt( expected1.hits,         actual[0].hits         );
     assertEqualsInt( expected1.earned_runs,  actual[0].earned_runs  );
     assertEqualsInt( expected1.home_runs,    actual[0].home_runs    );
     assertEqualsInt( expected1.walks,        actual[0].walks        );
     assertEqualsInt( expected1.strike_outs,  actual[0].strike_outs  );

     assertEqualsInt( expected2.team_id,      actual[1].team_id      );
     assertEqualsInt( expected2.season,       actual[1].season       );
     assertEqualsInt( expected2.season_phase, actual[1].season_phase );
     assertEqualsInt( expected2.wins,         actual[1].wins         );
     assertEqualsInt( expected2.losses,       actual[1].losses       );
     assertEqualsInt( expected2.games,        actual[1].games        );
     assertEqualsInt( expected2.saves,        actual[1].saves        );
     assertEqualsDbl( expected2.innings,      actual[1].innings      );
     assertEqualsInt( expected2.hits,         actual[1].hits         );
     assertEqualsInt( expected2.earned_runs,  actual[1].earned_runs  );
     assertEqualsInt( expected2.home_runs,    actual[1].home_runs    );
     assertEqualsInt( expected2.walks,        actual[1].walks        );
     assertEqualsInt( expected2.strike_outs,  actual[1].strike_outs  );

     free( actual );

     sqlite3_exec( db, "delete from team_pitching_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_pitching_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_pitching_stats_s expected = { 0 };

     expected.team_id      = 123;
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

     insert_a_team_pitching_stats( &expected );

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

     assertEquals( SQLITE_OK, team_pitching_stats_t_update( db, &expected ) );

     team_pitching_stats_s *actual = get_a_team_pitching_stats( expected.team_id );

     assertEqualsInt( expected.team_id,      actual->team_id      );
     assertEqualsInt( expected.season,       actual->season       );
     assertEqualsInt( expected.season_phase, actual->season_phase );
     assertEqualsInt( expected.wins,         actual->wins         );
     assertEqualsInt( expected.losses,       actual->losses       );
     assertEqualsInt( expected.games,        actual->games        );
     assertEqualsInt( expected.saves,        actual->saves        );
     assertEqualsDbl( expected.innings,      actual->innings      );
     assertEqualsInt( expected.hits,         actual->hits         );
     assertEqualsInt( expected.earned_runs,  actual->earned_runs  );
     assertEqualsInt( expected.home_runs,    actual->home_runs    );
     assertEqualsInt( expected.walks,        actual->walks        );
     assertEqualsInt( expected.strike_outs,  actual->strike_outs  );

     sqlite3_exec( db, "delete from team_pitching_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_pitching_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_pitching_stats_s expected = { 0 };

     expected.team_id      = 123;
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

     insert_a_team_pitching_stats( &expected );

     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected ) );

     assertNull( get_a_team_pitching_stats( expected.team_id ) );

     sqlite3_exec( db, "delete from team_pitching_stats_t", NULL, NULL, NULL );

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
     run_test( team_pitching_stats_t_create__ShouldInsertRecordsInTheTeamPitchingStatsTTable,          check_sqlite_error );
     run_test( team_pitching_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndPhaseExists, check_sqlite_error );
     run_test( team_pitching_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
     run_test( team_pitching_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,    check_sqlite_error );
     run_test( team_pitching_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
     run_test( team_pitching_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
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
