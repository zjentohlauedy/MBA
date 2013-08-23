#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include "ewbfiles.h"


int main( const int argc, const char *argv[] )
{
     fileleague_s *leagueFile;


     if ( argc < 3 )
     {
	  printf( "Usage: %s <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file." );

          return EXIT_FAILURE;
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          leagueFile->teams[i].wins[0]   = 0;
          leagueFile->teams[i].losses[0] = 0;
     }

     if ( ! writeLeagueFile( argv[2], leagueFile ) )
     {
          printf( "Cannot save league changes.\n" );

          free( leagueFile );

          return EXIT_FAILURE;
     }

     free( leagueFile );

     return EXIT_SUCCESS;
}
