#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_team_versus_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_versus_stats_s team_versus_stats;

     team_versus_stats_s **tvs = (team_versus_stats_s **)output;

     if ( cols < 8 ) return SQLITE_ERROR;

     memset( &team_versus_stats, '\0', sizeof(team_versus_stats_s) );

     /**/    team_versus_stats.team_id      = atoi( data[ 0] );
     /**/    team_versus_stats.season       = atoi( data[ 1] );
     /**/    team_versus_stats.season_phase = atoi( data[ 2] );
     /**/    team_versus_stats.opponent     = atoi( data[ 3] );
     /**/    team_versus_stats.wins         = atoi( data[ 4] );
     /**/    team_versus_stats.losses       = atoi( data[ 5] );
     /**/    team_versus_stats.runs_scored  = atoi( data[ 6] );
     /**/    team_versus_stats.runs_allowed = atoi( data[ 7] );

     *tvs = &team_versus_stats;

     return SQLITE_OK;
}

static team_versus_stats_s *get_a_team_versus_stats( int team_id )
{
     team_versus_stats_s *team_versus_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_versus_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_versus_stats_data, &team_versus_stats, NULL );

     return team_versus_stats;
}


static void insert_a_team_versus_stats( team_versus_stats_s *team_versus_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_versus_stats_t (team_id, season, season_phase, opponent, wins, losses, runs_scored, runs_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d)",
               team_versus_stats->team_id,
               team_versus_stats->season,
               team_versus_stats->season_phase,
               team_versus_stats->opponent,
               team_versus_stats->wins,
               team_versus_stats->losses,
               team_versus_stats->runs_scored,
               team_versus_stats->runs_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_versus_stats_t_create__ShouldInsertRecordsInTheTeamVersusStatsTTable()
{
     team_versus_stats_s *expected = buildTeamVersusStats( 123, 5, sp_Regular, 321 );

     assertEquals( SQLITE_OK, team_versus_stats_t_create( db, expected ) );

     team_versus_stats_s *actual = get_a_team_versus_stats( expected->team_id );

     compareTeamVersusStats( expected, actual );

     sqlite3_exec( db, "delete from team_versus_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *team_versus_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonPhaseAndOpponentExists()
{
     team_versus_stats_s *expected1 = buildTeamVersusStats( 123, 5, sp_Regular, 321 );
     team_versus_stats_s *expected2 = buildTeamVersusStats( 123, 5, sp_Regular, 321 );

     assertEquals( SQLITE_OK,         team_versus_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, team_versus_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from team_versus_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *team_versus_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonPhaseAndOpponent()
{
     team_versus_stats_s *expected = buildTeamVersusStats( 123, 5, sp_Regular, 321 );

     insert_a_team_versus_stats( expected );

     team_versus_stats_s actual = { 0 };

     actual.team_id      = expected->team_id;
     actual.season       = expected->season;
     actual.season_phase = expected->season_phase;
     actual.opponent     = expected->opponent;

     assertEquals( SQLITE_OK, team_versus_stats_t_read( db, &actual ) );

     compareTeamVersusStats( expected, &actual );

     sqlite3_exec( db, "delete from team_versus_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *team_versus_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_versus_stats_s *expected1 = buildTeamVersusStats( 123, 5, sp_Regular, 321 );
     team_versus_stats_s *expected2 = buildTeamVersusStats( 123, 8, sp_Regular, 321 );

     insert_a_team_versus_stats( expected1 );
     insert_a_team_versus_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_versus_stats_t_read_by_team( db, expected1->team_id, &list ) );

     assertEquals( 2, list.count );

     team_versus_stats_s *actual = list.data;

     assertNotNull( actual );

     compareTeamVersusStats( expected1, &actual[0] );
     compareTeamVersusStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from team_versus_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_versus_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonPhaseAndOpponent()
{
     team_versus_stats_s *expected1 = buildTeamVersusStats( 123, 5, sp_Regular, 321 );
     team_versus_stats_s *expected2 = buildTeamVersusStats( 123, 5, sp_Regular, 321 );

     insert_a_team_versus_stats( expected1 );

     assertEquals( SQLITE_OK, team_versus_stats_t_update( db, expected2 ) );

     team_versus_stats_s *actual = get_a_team_versus_stats( expected1->team_id );

     compareTeamVersusStats( expected2, actual );

     sqlite3_exec( db, "delete from team_versus_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *team_versus_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonPhaseAndOpponent()
{
     team_versus_stats_s *expected = buildTeamVersusStats( 123, 5, sp_Regular, 321 );

     insert_a_team_versus_stats( expected );

     assertEquals( SQLITE_OK, team_versus_stats_t_delete( db, expected ) );

     assertNull( get_a_team_versus_stats( expected->team_id ) );

     sqlite3_exec( db, "delete from team_versus_stats_t", NULL, NULL, NULL );

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
     run_test( team_versus_stats_t_create__ShouldInsertRecordsInTheTeamVersusStatsTTable,                    check_sqlite_error );
     run_test( team_versus_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonPhaseAndOpponentExists, check_sqlite_error );
     run_test( team_versus_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonPhaseAndOpponent,  check_sqlite_error );
     run_test( team_versus_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,            check_sqlite_error );
     run_test( team_versus_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonPhaseAndOpponent,  check_sqlite_error );
     run_test( team_versus_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonPhaseAndOpponent,  check_sqlite_error );
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
