#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ewbfiles.h"


int main( int argc, char *argv[] )
{
     fileplayer_s *baseFile;
     fileplayer_s *branchFile;
     int           i;


     if ( argc < 4 )
     {
	  printf( "Usage: %s <base> <branch> <output>.\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     // Load Base Players.S
     if ( (baseFile = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Cannot load players file." );

          return EXIT_FAILURE;
     }

     // Load Branch Players.S
     if ( (branchFile = readPlayersFile( argv[2] )) == NULL )
     {
          printf( "Cannot load players file." );

          free( baseFile );

          return EXIT_FAILURE;
     }

     int firstT1Player = (PLAYERS_PER_TEAM) * 21;
     int firstT2Player = (PLAYERS_PER_TEAM) * 25;

     for ( i = 0; i < TOTAL_PLAYERS; ++i )
     {
	  if ( i >= firstT1Player  &&  i < (firstT1Player + 25) )
	  {
	       baseFile[i] = branchFile[i];
	  }

	  if ( i >= firstT2Player  &&  i < (firstT2Player + 25) )
	  {
	       baseFile[i] = branchFile[i];
	  }
     }

     if ( ! writePlayersFile( argv[3], baseFile ) )
     {
          printf( "Cannot save player changes.\n" );

	  free( baseFile );
	  free( branchFile );

          return EXIT_FAILURE;
     }

     free( baseFile );
     free( branchFile );

     return EXIT_SUCCESS;
}
