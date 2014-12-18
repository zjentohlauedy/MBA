#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ewbfiles.h"


int main( const int argc, const char *argv[] )
{
     const acc_pch_stats_s  clear_pitcher_stats = { 0 };
     const acc_bat_stats_s  clear_batter_stats  = { 0 };
     /**/  fileplayer_s    *playersFile;
     /**/  int              idx;


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
          if ( playersFile[idx].last_name[0] == '\0' ) continue;

          unsigned int pos = playersFile[idx].position[0] / 0x10;

          if ( pos == pos_Pitcher )
          {
               struct pitching_s *pitching = &(playersFile[idx].filestats.filepitching);

               pitching->simulated = clear_pitcher_stats;
          }
          else
          {
               struct batting_s *batting = &(playersFile[idx].filestats.filebatting);

               batting->simulated = clear_batter_stats;
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
