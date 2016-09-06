#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ewbfiles.h"
#include "league.h"


static void printPlayers( fileplayer_s *playerFile )
{
     for ( int idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          const fileplayer_s *player = playerFile + idx;

          char first_name [sizeof(player->first_name)];
          char last_name  [sizeof(player->last_name) ];

          memcpy( first_name, player->first_name, sizeof(player->first_name) );
          memcpy( last_name,  player->last_name,  sizeof(player->last_name)  );

          untermName( first_name, sizeof(player->first_name) );
          untermName( last_name,  sizeof(player->last_name)  );

          printf( "%.*s, %.*s\n", sizeof(player->last_name), last_name, sizeof(player->first_name), first_name );
     }
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playerFile;


     if ( argc < 2 )
     {
	  printf( "Usage: %s <playerfile>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     // Load Players.S
     if ( (playerFile = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Cannot load players file." );

          return EXIT_FAILURE;
     }

     printPlayers( playerFile );

     free( playerFile );

     return EXIT_SUCCESS;
}
