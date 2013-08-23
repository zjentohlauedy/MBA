#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "ewbfiles.h"


static int playercmp( const void *arg1, const void *arg2 )
{
     const fileplayer_s *p1 = (fileplayer_s *)arg1;
     const fileplayer_s *p2 = (fileplayer_s *)arg2;
     /**/  int           cmp;

     if ( p1->last_name[0] == '\0' )
     {
          if ( p2->last_name[0] == '\0' ) return 0;

          return 1;
     }

     if ( p2->last_name[0] == '\0' ) return -1;

     if ( (cmp = strncmp( p1->last_name, p2->last_name, sizeof(p1->last_name) )) != 0 ) return cmp;

     return strncmp( p1->first_name, p2->first_name, sizeof(p1->first_name) );
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s   *playersFile;
     int             idx;


     if ( argc < 3 )
     {
	  printf( "Usage: %s <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (playersFile = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[1] );

          return EXIT_SUCCESS;
     }

     qsort( playersFile, TOTAL_PLAYERS, sizeof(fileplayer_s), playercmp );

     if ( ! writePlayersFile( argv[2], playersFile ) )
     {
          printf( "Cannot save player changes.\n" );

          return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
