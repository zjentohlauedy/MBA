#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_team_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_stats_s team_stats;

     team_stats_s **ts = (team_stats_s **)output;

     if ( cols < 15 ) return SQLITE_ERROR;

     memset( &team_stats, '\0', sizeof(team_stats_s) );

     team_stats.team_id         = atoi( data[ 0] );
     team_stats.season          = atoi( data[ 1] );
     team_stats.season_phase    = atoi( data[ 2] );
     team_stats.wins            = atoi( data[ 3] );
     team_stats.losses          = atoi( data[ 4] );
     team_stats.home_wins       = atoi( data[ 5] );
     team_stats.home_losses     = atoi( data[ 6] );
     team_stats.road_wins       = atoi( data[ 7] );
     team_stats.road_losses     = atoi( data[ 8] );
     team_stats.division_wins   = atoi( data[ 9] );
     team_stats.division_losses = atoi( data[10] );
     team_stats.league_wins     = atoi( data[11] );
     team_stats.league_losses   = atoi( data[12] );
     team_stats.runs_scored     = atoi( data[13] );
     team_stats.runs_allowed    = atoi( data[14] );

     *ts = &team_stats;

     return SQLITE_OK;
}

static team_stats_s *get_a_team_stats( int team_id )
{
     team_stats_s *team_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_stats_data, &team_stats, NULL );

     return team_stats;
}

static void insert_a_team_stats( team_stats_s *team_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_stats_t (team_id, season, season_phase, wins, losses, home_wins, home_losses, road_wins, road_losses, division_wins, division_losses, league_wins, league_losses, runs_scored, runs_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               team_stats->team_id,
               team_stats->season,
               team_stats->season_phase,
               team_stats->wins,
               team_stats->losses,
               team_stats->home_wins,
               team_stats->home_losses,
               team_stats->road_wins,
               team_stats->road_losses,
               team_stats->division_wins,
               team_stats->division_losses,
               team_stats->league_wins,
               team_stats->league_losses,
               team_stats->runs_scored,
               team_stats->runs_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_stats_t_create__ShouldInsertRecordsInTheTeamStatsTTable()
{
     team_stats_s expected = { 0 };

     expected.team_id         = 123;
     expected.season          = 2;
     expected.season_phase    = sp_Regular;
     expected.wins            = 85;
     expected.losses          = 67;
     expected.home_wins       = 56;
     expected.home_losses     = 20;
     expected.road_wins       = 29;
     expected.road_losses     = 47;
     expected.division_wins   = 30;
     expected.division_losses = 26;
     expected.league_wins     = 52;
     expected.league_losses   = 36;
     expected.runs_scored     = 234;
     expected.runs_allowed    = 222;

     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected ) );

     team_stats_s *actual = get_a_team_stats( expected.team_id );

     assertEquals( expected.team_id,         actual->team_id         );
     assertEquals( expected.season,          actual->season          );
     assertEquals( expected.season_phase,    actual->season_phase    );
     assertEquals( expected.wins,            actual->wins            );
     assertEquals( expected.losses,          actual->losses          );
     assertEquals( expected.home_wins,       actual->home_wins       );
     assertEquals( expected.home_losses,     actual->home_losses     );
     assertEquals( expected.road_wins,       actual->road_wins       );
     assertEquals( expected.road_losses,     actual->road_losses     );
     assertEquals( expected.division_wins,   actual->division_wins   );
     assertEquals( expected.division_losses, actual->division_losses );
     assertEquals( expected.league_wins,     actual->league_wins     );
     assertEquals( expected.league_losses,   actual->league_losses   );
     assertEquals( expected.runs_scored,     actual->runs_scored     );
     assertEquals( expected.runs_allowed,    actual->runs_allowed    );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndPhaseExists()
{
     team_stats_s expected = { 0 };

     expected.team_id         = 123;
     expected.season          = 2;
     expected.season_phase    = sp_Regular;
     expected.wins            = 85;
     expected.losses          = 67;
     expected.home_wins       = 56;
     expected.home_losses     = 20;
     expected.road_wins       = 29;
     expected.road_losses     = 47;
     expected.division_wins   = 30;
     expected.division_losses = 26;
     expected.league_wins     = 52;
     expected.league_losses   = 36;
     expected.runs_scored     = 234;
     expected.runs_allowed    = 222;

     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected ) );

     expected.wins            = 92;
     expected.losses          = 60;
     expected.home_wins       = 50;
     expected.home_losses     = 26;
     expected.road_wins       = 42;
     expected.road_losses     = 24;
     expected.division_wins   = 36;
     expected.division_losses = 20;
     expected.league_wins     = 60;
     expected.league_losses   = 28;
     expected.runs_scored     = 199;
     expected.runs_allowed    = 141;

     assertEquals( SQLITE_CONSTRAINT, team_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_stats_s expected = { 0 };

     expected.team_id         = 123;
     expected.season          = 2;
     expected.season_phase    = sp_Regular;
     expected.wins            = 85;
     expected.losses          = 67;
     expected.home_wins       = 56;
     expected.home_losses     = 20;
     expected.road_wins       = 29;
     expected.road_losses     = 47;
     expected.division_wins   = 30;
     expected.division_losses = 26;
     expected.league_wins     = 52;
     expected.league_losses   = 36;
     expected.runs_scored     = 234;
     expected.runs_allowed    = 222;

     insert_a_team_stats( &expected );

     team_stats_s actual = { 0 };

     actual.team_id      = expected.team_id;
     actual.season       = expected.season;
     actual.season_phase = expected.season_phase;

     assertEquals( SQLITE_OK, team_stats_t_read( db, &actual ) );

     assertEquals( expected.team_id,         actual.team_id         );
     assertEquals( expected.season,          actual.season          );
     assertEquals( expected.season_phase,    actual.season_phase    );
     assertEquals( expected.wins,            actual.wins            );
     assertEquals( expected.losses,          actual.losses          );
     assertEquals( expected.home_wins,       actual.home_wins       );
     assertEquals( expected.home_losses,     actual.home_losses     );
     assertEquals( expected.road_wins,       actual.road_wins       );
     assertEquals( expected.road_losses,     actual.road_losses     );
     assertEquals( expected.division_wins,   actual.division_wins   );
     assertEquals( expected.division_losses, actual.division_losses );
     assertEquals( expected.league_wins,     actual.league_wins     );
     assertEquals( expected.league_losses,   actual.league_losses   );
     assertEquals( expected.runs_scored,     actual.runs_scored     );
     assertEquals( expected.runs_allowed,    actual.runs_allowed    );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_stats_s expected1 = { 0 };
     team_stats_s expected2 = { 0 };

     expected1.team_id         = 123;
     expected1.season          = 2;
     expected1.season_phase    = sp_Regular;
     expected1.wins            = 85;
     expected1.losses          = 67;
     expected1.home_wins       = 56;
     expected1.home_losses     = 20;
     expected1.road_wins       = 29;
     expected1.road_losses     = 47;
     expected1.division_wins   = 30;
     expected1.division_losses = 26;
     expected1.league_wins     = 52;
     expected1.league_losses   = 36;
     expected1.runs_scored     = 234;
     expected1.runs_allowed    = 222;

     expected2.team_id         = 123;
     expected2.season          = 3;
     expected2.season_phase    = sp_Regular;
     expected2.wins            = 92;
     expected2.losses          = 60;
     expected2.home_wins       = 50;
     expected2.home_losses     = 26;
     expected2.road_wins       = 42;
     expected2.road_losses     = 24;
     expected2.division_wins   = 36;
     expected2.division_losses = 20;
     expected2.league_wins     = 60;
     expected2.league_losses   = 28;
     expected2.runs_scored     = 199;
     expected2.runs_allowed    = 141;

     insert_a_team_stats( &expected1 );
     insert_a_team_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_stats_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( 2, list.count );

     team_stats_s *actual = list.data;

     assertNotNull( actual );

     assertEquals( expected1.team_id,         actual[0].team_id         );
     assertEquals( expected1.season,          actual[0].season          );
     assertEquals( expected1.season_phase,    actual[0].season_phase    );
     assertEquals( expected1.wins,            actual[0].wins            );
     assertEquals( expected1.losses,          actual[0].losses          );
     assertEquals( expected1.home_wins,       actual[0].home_wins       );
     assertEquals( expected1.home_losses,     actual[0].home_losses     );
     assertEquals( expected1.road_wins,       actual[0].road_wins       );
     assertEquals( expected1.road_losses,     actual[0].road_losses     );
     assertEquals( expected1.division_wins,   actual[0].division_wins   );
     assertEquals( expected1.division_losses, actual[0].division_losses );
     assertEquals( expected1.league_wins,     actual[0].league_wins     );
     assertEquals( expected1.league_losses,   actual[0].league_losses   );
     assertEquals( expected1.runs_scored,     actual[0].runs_scored     );
     assertEquals( expected1.runs_allowed,    actual[0].runs_allowed    );

     assertEquals( expected2.team_id,         actual[1].team_id         );
     assertEquals( expected2.season,          actual[1].season          );
     assertEquals( expected2.season_phase,    actual[1].season_phase    );
     assertEquals( expected2.wins,            actual[1].wins            );
     assertEquals( expected2.losses,          actual[1].losses          );
     assertEquals( expected2.home_wins,       actual[1].home_wins       );
     assertEquals( expected2.home_losses,     actual[1].home_losses     );
     assertEquals( expected2.road_wins,       actual[1].road_wins       );
     assertEquals( expected2.road_losses,     actual[1].road_losses     );
     assertEquals( expected2.division_wins,   actual[1].division_wins   );
     assertEquals( expected2.division_losses, actual[1].division_losses );
     assertEquals( expected2.league_wins,     actual[1].league_wins     );
     assertEquals( expected2.league_losses,   actual[1].league_losses   );
     assertEquals( expected2.runs_scored,     actual[1].runs_scored     );
     assertEquals( expected2.runs_allowed,    actual[1].runs_allowed    );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_stats_s expected = { 0 };

     expected.team_id         = 123;
     expected.season          = 2;
     expected.season_phase    = sp_Regular;
     expected.wins            = 85;
     expected.losses          = 67;
     expected.home_wins       = 56;
     expected.home_losses     = 20;
     expected.road_wins       = 29;
     expected.road_losses     = 47;
     expected.division_wins   = 30;
     expected.division_losses = 26;
     expected.league_wins     = 52;
     expected.league_losses   = 36;
     expected.runs_scored     = 234;
     expected.runs_allowed    = 222;

     insert_a_team_stats( &expected );

     expected.wins            = 92;
     expected.losses          = 60;
     expected.home_wins       = 50;
     expected.home_losses     = 26;
     expected.road_wins       = 42;
     expected.road_losses     = 24;
     expected.division_wins   = 36;
     expected.division_losses = 20;
     expected.league_wins     = 60;
     expected.league_losses   = 28;
     expected.runs_scored     = 199;
     expected.runs_allowed    = 141;

     assertEquals( SQLITE_OK, team_stats_t_update( db, &expected ) );

     team_stats_s *actual = get_a_team_stats( expected.team_id );

     assertEquals( expected.team_id,         actual->team_id         );
     assertEquals( expected.season,          actual->season          );
     assertEquals( expected.season_phase,    actual->season_phase    );
     assertEquals( expected.wins,            actual->wins            );
     assertEquals( expected.losses,          actual->losses          );
     assertEquals( expected.home_wins,       actual->home_wins       );
     assertEquals( expected.home_losses,     actual->home_losses     );
     assertEquals( expected.road_wins,       actual->road_wins       );
     assertEquals( expected.road_losses,     actual->road_losses     );
     assertEquals( expected.division_wins,   actual->division_wins   );
     assertEquals( expected.division_losses, actual->division_losses );
     assertEquals( expected.league_wins,     actual->league_wins     );
     assertEquals( expected.league_losses,   actual->league_losses   );
     assertEquals( expected.runs_scored,     actual->runs_scored     );
     assertEquals( expected.runs_allowed,    actual->runs_allowed    );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndPhase()
{
     team_stats_s expected = { 0 };

     expected.team_id         = 123;
     expected.season          = 2;
     expected.season_phase    = sp_Regular;
     expected.wins            = 85;
     expected.losses          = 67;
     expected.home_wins       = 56;
     expected.home_losses     = 20;
     expected.road_wins       = 29;
     expected.road_losses     = 47;
     expected.division_wins   = 30;
     expected.division_losses = 26;
     expected.league_wins     = 52;
     expected.league_losses   = 36;
     expected.runs_scored     = 234;
     expected.runs_allowed    = 222;

     insert_a_team_stats( &expected );

     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected ) );

     assertNull( get_a_team_stats( expected.team_id ) );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

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
     run_test( team_stats_t_create__ShouldInsertRecordsInTheTeamStatsTTable,                  check_sqlite_error );
     run_test( team_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndPhaseExists, check_sqlite_error );
     run_test( team_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
     run_test( team_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,    check_sqlite_error );
     run_test( team_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
     run_test( team_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndPhase,  check_sqlite_error );
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
