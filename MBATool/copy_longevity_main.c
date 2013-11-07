#include <stdio.h>
#include <stdlib.h>
#include "file_formats.h"

int main( const int argc, const char *argv[] )
{
     fileplayer_s   *players_file1;
     fileplayer_s   *players_file2;


     if ( argc < 4 )
     {
	  printf( "Usage: %s <S01 file> <S02 file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (players_file1 = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[1] );

          return EXIT_SUCCESS;
     }

     if ( (players_file2 = readPlayersFile( argv[2] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[2] );

          free( players_file1 );

          return EXIT_SUCCESS;
     }

     if ( copyLongevity( players_file1, players_file2 ) == bl_False )
     {
          printf( "%s\n", getCopyLongevityError() );
          printf( "Error copying player ids.\n" );

          free( players_file1 );
          free( players_file2 );

          return EXIT_FAILURE;
     }

     if ( ! writePlayersFile( argv[3], players_file1 ) )
     {
          printf( "Cannot save player changes.\n" );

          free( players_file1 );
          free( players_file2 );

          return EXIT_FAILURE;
     }

     free( players_file1 );
     free( players_file2 );

     return EXIT_SUCCESS;
}
