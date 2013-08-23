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


static const char *darkies[] = {
     "Alan Allen",
     "Brad Crayton",
     "Chad Fox",
     "Douglas Brown",
     "Emil Emmert",
     "Eric Baker",
     "Eric Floyd",
     "Eric Freeman",
     "Francisc Caron",
     "Gregory Wilson",
     "James Wallace",
     "Joseph Sanders",
     "Loren Carey",
     "Lucas Uribe",
     "Micah Lancaster",
     "Michael Ceballos",
     "Morris Meredith",
     "Percy Swafford",
     "Preston Robertson",
     "Prince Kincaid",
     "Theodore Logan",
     "Thomas Ora",
     "Vincent Drennan",
     "Wesley Stone"
};


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

     srand( time( NULL ) );

     for ( idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          char namebuf[sizeof(playersFile[idx].first_name) + 1 + sizeof(playersFile[idx].last_name) + 1];
          char fn[sizeof(playersFile[idx].first_name)];
          char ln[sizeof(playersFile[idx].last_name) ];

          if ( playersFile[idx].last_name[0] == '\0' ) continue;

          memcpy( fn, playersFile[idx].first_name, sizeof(playersFile[idx].first_name) );
          memcpy( ln, playersFile[idx].last_name,  sizeof(playersFile[idx].last_name)  );

          untermName( fn, sizeof(playersFile[idx].first_name) );
          untermName( ln, sizeof(playersFile[idx].last_name)  );

          sprintf( namebuf, "%.*s %.*s",
                   sizeof(playersFile[idx].first_name), fn,
                   sizeof(playersFile[idx].last_name),  ln );

          for ( int i = 0; i < 24; ++i )
          {
               if ( strcmp( namebuf, darkies[i] ) == 0 )
               {
                    printf( "Making %s Darker.\n", namebuf );

                    if   ( nibble( playersFile[idx].position[0], n_High ) == pos_Pitcher ) playersFile[idx].filestats.filepitching.color[0] = '@';
                    else                                                                   playersFile[idx].filestats.filebatting .color[0] = '@';
               }
          }
     }

     if ( ! writePlayersFile( argv[2], playersFile ) )
     {
          printf( "Cannot save player changes.\n" );

          free( playersFile );

          return EXIT_FAILURE;
     }

     free( playersFile );

     return EXIT_SUCCESS;
}
