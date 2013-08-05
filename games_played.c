#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewbfiles.h"


int main( int argc, char *argv[] )
{
     fileleague_s *leagueFile;

     if ( argc < 2 )
     {
	  printf( "Usage: %s <league file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file." );

          return EXIT_FAILURE;
     }

     free( leagueFile );

     printf( "%d\n", leagueFile->teams[0].wins[0] + leagueFile->teams[0].losses[0] );

     return EXIT_SUCCESS;
}
