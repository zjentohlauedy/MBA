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

     assertEquals( "team_stats_t_create()", SQLITE_OK, team_stats_t_create( db, &expected ) );

     team_stats_s *actual = get_a_team_stats( expected.team_id );

     assertEquals( "team_id",         expected.team_id,         actual->team_id         );
     assertEquals( "season",          expected.season,          actual->season          );
     assertEquals( "season_phase",    expected.season_phase,    actual->season_phase    );
     assertEquals( "wins",            expected.wins,            actual->wins            );
     assertEquals( "losses",          expected.losses,          actual->losses          );
     assertEquals( "home_wins",       expected.home_wins,       actual->home_wins       );
     assertEquals( "home_losses",     expected.home_losses,     actual->home_losses     );
     assertEquals( "road_wins",       expected.road_wins,       actual->road_wins       );
     assertEquals( "road_losses",     expected.road_losses,     actual->road_losses     );
     assertEquals( "division_wins",   expected.division_wins,   actual->division_wins   );
     assertEquals( "division_losses", expected.division_losses, actual->division_losses );
     assertEquals( "league_wins",     expected.league_wins,     actual->league_wins     );
     assertEquals( "league_losses",   expected.league_losses,   actual->league_losses   );
     assertEquals( "runs_scored",     expected.runs_scored,     actual->runs_scored     );
     assertEquals( "runs_allowed",    expected.runs_allowed,    actual->runs_allowed    );

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

     assertEquals( "team_stats_t_create()", SQLITE_OK, team_stats_t_create( db, &expected ) );

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

     assertEquals( "team_stats_t_create()", SQLITE_CONSTRAINT, team_stats_t_create( db, &expected ) );

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

     assertEquals( "team_stats_t_read()", SQLITE_OK, team_stats_t_read( db, &actual ) );

     assertEquals( "team_id",         expected.team_id,         actual.team_id         );
     assertEquals( "season",          expected.season,          actual.season          );
     assertEquals( "season_phase",    expected.season_phase,    actual.season_phase    );
     assertEquals( "wins",            expected.wins,            actual.wins            );
     assertEquals( "losses",          expected.losses,          actual.losses          );
     assertEquals( "home_wins",       expected.home_wins,       actual.home_wins       );
     assertEquals( "home_losses",     expected.home_losses,     actual.home_losses     );
     assertEquals( "road_wins",       expected.road_wins,       actual.road_wins       );
     assertEquals( "road_losses",     expected.road_losses,     actual.road_losses     );
     assertEquals( "division_wins",   expected.division_wins,   actual.division_wins   );
     assertEquals( "division_losses", expected.division_losses, actual.division_losses );
     assertEquals( "league_wins",     expected.league_wins,     actual.league_wins     );
     assertEquals( "league_losses",   expected.league_losses,   actual.league_losses   );
     assertEquals( "runs_scored",     expected.runs_scored,     actual.runs_scored     );
     assertEquals( "runs_allowed",    expected.runs_allowed,    actual.runs_allowed    );

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

     assertEquals( "team_stats_t_update()", SQLITE_OK, team_stats_t_update( db, &expected ) );

     team_stats_s *actual = get_a_team_stats( expected.team_id );

     assertEquals( "team_id",         expected.team_id,         actual->team_id         );
     assertEquals( "season",          expected.season,          actual->season          );
     assertEquals( "season_phase",    expected.season_phase,    actual->season_phase    );
     assertEquals( "wins",            expected.wins,            actual->wins            );
     assertEquals( "losses",          expected.losses,          actual->losses          );
     assertEquals( "home_wins",       expected.home_wins,       actual->home_wins       );
     assertEquals( "home_losses",     expected.home_losses,     actual->home_losses     );
     assertEquals( "road_wins",       expected.road_wins,       actual->road_wins       );
     assertEquals( "road_losses",     expected.road_losses,     actual->road_losses     );
     assertEquals( "division_wins",   expected.division_wins,   actual->division_wins   );
     assertEquals( "division_losses", expected.division_losses, actual->division_losses );
     assertEquals( "league_wins",     expected.league_wins,     actual->league_wins     );
     assertEquals( "league_losses",   expected.league_losses,   actual->league_losses   );
     assertEquals( "runs_scored",     expected.runs_scored,     actual->runs_scored     );
     assertEquals( "runs_allowed",    expected.runs_allowed,    actual->runs_allowed    );

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

     assertEquals( "team_stats_t_delete()", SQLITE_OK, team_stats_t_delete( db, &expected ) );

     assertEquals( "get_a_team_stats()", NULL, get_a_team_stats( expected.team_id ) );

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
