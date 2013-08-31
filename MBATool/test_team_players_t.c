#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_team_player_data( void *output, int cols, char *data[], char *names[] )
{
     static team_player_s team_player;

     team_player_s **tp = (team_player_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &team_player, '\0', sizeof(team_player_s) );

     team_player.team_id   = atoi( data[0] );
     team_player.season    = atoi( data[1] );
     team_player.player_id = atoi( data[2] );

     *tp = &team_player;

     return SQLITE_OK;
}

static team_player_s *get_a_team_player( int team_id )
{
     team_player_s *team_player = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_players_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_player_data, &team_player, NULL );

     return team_player;
}

static void insert_a_team_player( team_player_s *team_player )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into team_players_t (team_id, season, player_id) values (%d, %d, %d)",
               team_player->team_id,
               team_player->season,
               team_player->player_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static void check_sqlite_error()
{
     if ( sqlite3_errcode( db ) != 0 )
     {
          printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
     }
}

static char *team_players_t_create__ShouldInsertRecordsInTheTeamPlayersTTable()
{
     team_player_s expected = { 0 };

     expected.team_id   = 123;
     expected.season    = 4;
     expected.player_id = 567;

     assertEquals( "team_players_t_create()", SQLITE_OK, team_players_t_create( db, &expected ) );

     team_player_s *actual = get_a_team_player( expected.team_id );

     assertEquals( "team_id",   expected.team_id,   actual->team_id   );
     assertEquals( "season",    expected.season,    actual->season    );
     assertEquals( "player_id", expected.player_id, actual->player_id );

     sqlite3_exec( db, "delete from team_players_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_players_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     team_player_s expected = { 0 };

     expected.team_id   = 123;
     expected.season    = 4;
     expected.player_id = 567;

     assertEquals( "team_players_t_create()", SQLITE_OK, team_players_t_create( db, &expected ) );

     assertEquals( "team_players_t_create()", SQLITE_CONSTRAINT, team_players_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_players_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_players_t_read_by_team__ShouldRetrieveMatchingRecord_GivenTheTeamId()
{
     team_player_s expected = { 0 };

     expected.team_id   = 123;
     expected.season    = 4;
     expected.player_id = 567;

     insert_a_team_player( &expected );

     data_list_s list = { 0 };

     assertEquals( "team_players_t_read_by_team()", SQLITE_OK, team_players_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( "count", 1, list.count );

     team_player_s *actual = list.data;

     assertEquals( "team_id",   expected.team_id,   actual->team_id   );
     assertEquals( "season",    expected.season,    actual->season    );
     assertEquals( "player_id", expected.player_id, actual->player_id );

     free( actual );

     sqlite3_exec( db, "delete from team_players_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_players_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_player_s expected1 = { 0 };
     team_player_s expected2 = { 0 };

     expected1.team_id   = 123;
     expected1.season    = 4;
     expected1.player_id = 567;

     expected2.team_id   = 123;
     expected2.season    = 5;
     expected2.player_id = 234;

     insert_a_team_player( &expected1 );
     insert_a_team_player( &expected2 );

     data_list_s list = { 0 };

     assertEquals( "team_players_t_read_by_team()", SQLITE_OK, team_players_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( "count", 2, list.count );

     team_player_s *actual = list.data;

     assertEquals( "team_id",   expected1.team_id,   actual[0].team_id   );
     assertEquals( "season",    expected1.season,    actual[0].season    );
     assertEquals( "player_id", expected1.player_id, actual[0].player_id );

     assertEquals( "team_id",   expected2.team_id,   actual[1].team_id   );
     assertEquals( "season",    expected2.season,    actual[1].season    );
     assertEquals( "player_id", expected2.player_id, actual[1].player_id );

     assertEquals( "team_id", 0, actual[2].team_id );

     free( actual );

     sqlite3_exec( db, "delete from team_players_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_players_t_delete__ShouldDeleteMatchingRecord_GivenATeamPlayer()
{
     team_player_s expected = { 0 };

     expected.team_id   = 123;
     expected.season    = 4;
     expected.player_id = 567;

     insert_a_team_player( &expected );

     assertEquals( "team_players_t_delete()", SQLITE_OK, team_players_t_delete( db, &expected ) );

     team_player_s *actual = get_a_team_player( expected.team_id );

     assertEquals( "actual", NULL, actual );

     sqlite3_exec( db, "delete from team_players_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_players_t_delete__ShouldOnlyDeleteMatchingRecord_GivenATeamPlayer()
{
     team_player_s expected1 = { 0 };
     team_player_s expected2 = { 0 };

     expected1.team_id   = 123;
     expected1.season    = 4;
     expected1.player_id = 567;

     expected2.team_id   = 123;
     expected2.season    = 4;
     expected2.player_id = 234;

     insert_a_team_player( &expected1 );
     insert_a_team_player( &expected2 );

     assertEquals( "team_players_t_delete()", SQLITE_OK, team_players_t_delete( db, &expected2 ) );

     team_player_s *actual = get_a_team_player( expected1.team_id );

     assertEquals( "team_id",   expected1.team_id,   actual->team_id   );
     assertEquals( "season",    expected1.season,    actual->season    );
     assertEquals( "player_id", expected1.player_id, actual->player_id );

     sqlite3_exec( db, "delete from team_players_t", NULL, NULL, NULL );

     return NULL;
}

static void run_all_tests()
{
     run_test( team_players_t_create__ShouldInsertRecordsInTheTeamPlayersTTable,             check_sqlite_error );
     run_test( team_players_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,          check_sqlite_error );
     run_test( team_players_t_read_by_team__ShouldRetrieveMatchingRecord_GivenTheTeamId,     check_sqlite_error );
     run_test( team_players_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId, check_sqlite_error );
     run_test( team_players_t_delete__ShouldDeleteMatchingRecord_GivenATeamPlayer,           check_sqlite_error );
     run_test( team_players_t_delete__ShouldOnlyDeleteMatchingRecord_GivenATeamPlayer,       check_sqlite_error );
}

int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a db file name.\n" );

          return EXIT_FAILURE;
     }

     sqlite3_open( argv[1], &db );

     run_all_tests( db );

     show_test_results();

     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
