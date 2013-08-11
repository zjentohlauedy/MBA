#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_player_accolade_data( void *output, int cols, char *data[], char *names[] )
{
     player_accolade_s *pa = (player_accolade_s *)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     pa->player_id = atoi( data[0] );
     pa->season    = atoi( data[1] );
     pa->accolade  = atoi( data[2] );

     return SQLITE_OK;
}

static player_accolade_s *get_a_player_accolade( int player_id )
{
     static player_accolade_s player_accolade;

     char query[999+1];

     memset( &player_accolade, '\0', sizeof(player_accolade_s) );

     snprintf( query, sizeof(query), "select * from player_accolades_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_accolade_data, &player_accolade, NULL );

     return &player_accolade;
}

static void insert_a_player_accolade( player_accolade_s *player_accolade )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into player_accolades_t (player_id, season, accolade) values (%d, %d, %d)",
               player_accolade->player_id,
               player_accolade->season,
               player_accolade->accolade );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}

static char *insert_player_accolade__ShouldInsertRecordsInThePlayerAccoladesTTable()
{
     player_accolade_s expected = { 0 };

     expected.player_id = 123;
     expected.season    = 5;
     expected.accolade  = acc_League_Title;

     assertEquals( "insert_player_accolade()", SQLITE_OK, insert_player_accolade( db, &expected ) );

     player_accolade_s *actual = get_a_player_accolade( expected.player_id );

     assertEquals( "player_id", expected.player_id, actual->player_id );
     assertEquals( "season",    expected.season,    actual->season    );
     assertEquals( "accolade",  expected.accolade,  actual->accolade  );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *get_player_accolade__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     player_accolade_s expected = { 0 };

     expected.player_id = 123;
     expected.season    = 5;
     expected.accolade  = acc_League_Title;

     insert_a_player_accolade( &expected );

     player_accolade_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( "get_player_accolade()", SQLITE_OK, get_player_accolade( db, &actual ) );

     assertEquals( "player_id", expected.player_id, actual.player_id );
     assertEquals( "season",    expected.season,    actual.season    );
     assertEquals( "accolade",  expected.accolade,  actual.accolade  );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *update_player_accolade__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     player_accolade_s expected = { 0 };

     expected.player_id = 123;
     expected.season    = 5;
     expected.accolade  = acc_League_Title;

     insert_a_player_accolade( &expected );

     expected.season    = 2;
     expected.accolade  = acc_All_Star;

     assertEquals( "update_player_accolade()", SQLITE_OK, update_player_accolade( db, &expected ) );

     player_accolade_s *actual = get_a_player_accolade( expected.player_id );

     assertEquals( "player_id", expected.player_id, actual->player_id );
     assertEquals( "season",    expected.season,    actual->season    );
     assertEquals( "accolade",  expected.accolade,  actual->accolade  );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *delete_player_accolade__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     player_accolade_s expected = { 0 };

     expected.player_id = 123;
     expected.season    = 5;
     expected.accolade  = acc_League_Title;

     insert_a_player_accolade( &expected );

     assertEquals( "delete_player_accolade()", SQLITE_OK, delete_player_accolade( db, &expected ) );

     player_accolade_s *actual = get_a_player_accolade( expected.player_id );

     assertEquals( "player_id", 0, actual->player_id );
     assertEquals( "season",    0, actual->season    );
     assertEquals( "accolade",  0, actual->accolade  );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

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
     run_test( insert_player_accolade__ShouldInsertRecordsInThePlayerAccoladesTTable, check_sqlite_error );
     run_test( get_player_accolade__ShouldRetrieveMatchingRecord_GivenThePlayerId,    check_sqlite_error );
     run_test( update_player_accolade__ShouldModifyMatchingRecord_GivenThePlayerId,   check_sqlite_error );
     run_test( delete_player_accolade__ShouldDeleteMatchingRecord_GivenThePlayerId,   check_sqlite_error );
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
