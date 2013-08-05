#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"


static void print_player( player_s *player )
{
     printf( "Id: %d; Name: %s %s; Color: %s; Hand: %s; Type: %s\n",
             player->player_id,
             player->first_name,
             player->last_name,
             (player->skin_tone == st_Light) ? "White" : "Black",
             (player->handedness == hnd_Right) ? "Right" : "Left",
             (player->player_type == pt_Pitcher) ? "Pitcher" : "Hitter"
          );
}


int main( int argc, char *argv[] )
{
     sqlite3 *db;
     player_s player;
     player_s player1;
     player_s player2;


     if ( sqlite3_open( "mba.db", &db ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     /**/    player1.player_id       = 1;
     strcpy( player1.first_name,      "FirstName1" );
     strcpy( player1.last_name,       "LastName1"  );
     strcpy( player1.first_phonetic,  "FirstPho1"  );
     strcpy( player1.last_phonetic,   "LastPho1"   );
     /**/    player1.skin_tone       = st_Light;
     /**/    player1.handedness      = hnd_Right;
     /**/    player1.player_type     = pt_Pitcher;
     /**/    player1.rookie_season   = 1;
     /**/    player1.longevity       = 5;

     /**/    player2.player_id       = 2;
     strcpy( player2.first_name,      "FirstName2" );
     strcpy( player2.last_name,       "LastName2"  );
     strcpy( player2.first_phonetic,  "FirstPho2"  );
     strcpy( player2.last_phonetic,   "LastPho2"   );
     /**/    player2.skin_tone       = st_Dark;
     /**/    player2.handedness      = hnd_Left;
     /**/    player2.player_type     = pt_Hitter;
     /**/    player2.rookie_season   = 2;
     /**/    player2.longevity       = 5;

     print_player( &player1 );
     print_player( &player2 );

     if ( insert_player( db, &player1 ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't insert player1: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     if ( insert_player( db, &player2 ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't insert player2: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     player.player_id = 1;

     if ( get_player( db, &player ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't get player 1: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     print_player( &player );


     player.player_id = 2;

     if ( get_player( db, &player ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't get player 2: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     print_player( &player );


     strcpy( player1.first_name,  "Doug"  );
     strcpy( player1.last_name,   "Guod"  );
     /**/    player1.handedness  = hnd_Left;

     if ( update_player( db, &player1 ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't update player1: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }


     player.player_id = 1;

     if ( get_player( db, &player ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't get player 1: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     print_player( &player );


     player.player_id = 2;

     if ( delete_player( db, &player ) != SQLITE_OK )
     {
          fprintf( stderr, "Can't delete player 2: %s\n", sqlite3_errmsg( db ) );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }


     sqlite3_close( db );

     return EXIT_SUCCESS;
}
