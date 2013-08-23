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

     for ( idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
	  if ( playersFile[idx].last_name[0] != 0 )
	  {
	       unsigned int pos = playersFile[idx].position[0] / 0x10;

	       if   ( pos != pos_Pitcher ) continue;

               struct pitching_s *pitching = &(playersFile[idx].filestats.filepitching);

               float inn = (float)word2int( pitching->real_inn ) / 10.0;
               float innings  = ((int)inn);
               /**/  innings += (inn - innings) / 3.0;

               unsigned int earned_runs = pitching->real_er[0];

               float era = ((float)earned_runs) / innings * 9.0;

               unsigned int games =     (int)round( innings / ((1 - ((era - 0.50)/2.0)) + 7.5) );
               unsigned int completed = games - (int)ceil( (((era - 0.50)/2.0) * 10.0) );

               pitching->real_games[0]  = games;
               pitching->real_starts[0] = games;
               pitching->real_cg[0]     = completed;

               unsigned char fatigue = (pitching->ratings[1]>>4);
               unsigned char bunt    = (pitching->ratings[1] & 0x0F);

               if ( fatigue == 5 ) fatigue = 6;
               if ( fatigue <  5 ) fatigue = 5;

               pitching->ratings[1] = (fatigue * 0x10) + bunt;
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
