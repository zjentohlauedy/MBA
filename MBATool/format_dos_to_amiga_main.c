#include <stdio.h>
#include <stdlib.h>
#include "file_formats.h"

int main( const int argc, const char *argv[] )
{
     fileplayer_s *players_file;


     if ( argc < 3 )
     {
	  printf( "Usage: %s <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (players_file = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[1] );

          return EXIT_SUCCESS;
     }

     if ( ! formatDosToAmiga( players_file ) )
     {
          printf( "%s\n", getFormatDosToAmigaError() );
          printf( "Cannot complete player changes.\n" );

          free( players_file );

          return EXIT_FAILURE;
     }

     if ( ! writePlayersFile( argv[2], players_file ) )
     {
          printf( "Cannot save player changes.\n" );

          free( players_file );

          return EXIT_FAILURE;
     }

     free( players_file );

     return EXIT_SUCCESS;
}
