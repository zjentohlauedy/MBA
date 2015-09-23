#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewbfiles.h"


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playerFile;


     if ( argc != 3 )
     {
	  printf( "Usage: %s <input playerfile> <output playerfile>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     // Load Players.S
     if ( (playerFile = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Cannot load players file.\n" );

          return EXIT_FAILURE;
     }

     for ( int idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
	  if ( playerFile[idx].last_name[0] == '\0' ) continue;

          unsigned int pos = playerFile[idx].position[0] / 0x10;

          if   ( pos != pos_Pitcher )
          {
               struct batting_s *batting = &(playerFile[idx].filestats.filebatting);

               batting->injury_days[0] = 0;
          }
          else
          {
               struct pitching_s *pitching = &(playerFile[idx].filestats.filepitching);

               pitching->injury_days[0] = 0;
          }
     }

     // Write Players.S
     if ( ! writePlayersFile( argv[2], playerFile ) )
     {
          printf( "Cannot save player changes.\n" );

          free( playerFile );

          return EXIT_FAILURE;
     }

     free( playerFile );

     return EXIT_SUCCESS;
}
