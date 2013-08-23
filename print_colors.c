#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "ewbfiles.h"


int main( const int argc, const char *argv[] )
{
     fileplayer_s   *playersFile;
     int             idx;


     if ( argc < 2 )
     {
	  printf( "Usage: %s <input file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (playersFile = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[1] );

          return EXIT_SUCCESS;
     }

     srand( time( NULL ) );

     for ( idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          char namebuf[sizeof(playersFile[idx].first_name) + 1 + sizeof(playersFile[idx].last_name) + 1];

          if ( playersFile[idx].last_name[0] == '\0' ) continue;

          untermName( playersFile[idx].first_name, sizeof(playersFile[idx].first_name) );
          untermName( playersFile[idx].last_name,  sizeof(playersFile[idx].last_name)  );

          sprintf( namebuf, "%.*s %.*s",
                   sizeof(playersFile[idx].first_name), playersFile[idx].first_name,
                   sizeof(playersFile[idx].last_name),  playersFile[idx].last_name );

          if ( playersFile[idx].position[0]>>4 == 0 )
          {
               printf( "%-*s %s\n", sizeof(namebuf), namebuf, playersFile[idx].filestats.filepitching.color[0] == '@' ? "Dark" : "Light" );
          }
          else
          {
               printf( "%-*s %s\n", sizeof(namebuf), namebuf, playersFile[idx].filestats.filebatting.color[0] == '@' ? "Dark" : "Light" );
          }
     }

     free( playersFile );

     return EXIT_SUCCESS;
}
