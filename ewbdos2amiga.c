#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ewbfiles.h"

#define MAX_VALUE 225


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playersFile;
     int           idx;


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
	  if ( playersFile[idx].last_name[0] != '\0' )
	  {
	       unsigned int pos = playersFile[idx].position[0] / 0x10;

	       if   ( pos != pos_Pitcher )
	       {
		    struct batting_s *batting = &(playersFile[idx].filestats.filebatting);

		    if ( batting->action.acc_rbi[0] >= MAX_VALUE ||
			 batting->action.acc_so[0]  >= MAX_VALUE    )
		    {
			 acc_pch_stats_s *pch = (acc_pch_stats_s *)&(batting->simulated);

			 unsigned int act_inn  = (pch->acc_innings[0]<<8) + pch->acc_innings[1];
			 unsigned int act_hits = (pch->acc_hits[0]<<8)    + pch->acc_hits[1];

			 if ( act_inn > 0  ||  act_hits > 0 )
			 {
			      printf( "WARNING: Player " );
			      printf( "%.*s, %.*s ",
				      sizeof(playersFile[idx].last_name),  playersFile[idx].last_name,
				      sizeof(playersFile[idx].first_name), playersFile[idx].first_name );
			      printf( "requires overflow and has pitching stats.\n" );
			 }
		    }

		    acc_bat_stats_s sim = batting->simulated;

		    batting->simulated = batting->action;

		    memset( &(batting->action), 0, sizeof(acc_bat_stats_s) );

		    // If RBI are over MAX_VALUE, move extra from sim to act
		    if ( batting->simulated.acc_rbi[0] >= MAX_VALUE )
		    {
			 unsigned char x = batting->simulated.acc_rbi[0] - MAX_VALUE;

			 batting->action.acc_rbi[0]     = sim.acc_rbi[0];

			 batting->simulated.acc_rbi[0] -= x;
			 batting->action.acc_rbi[0]    += x;
		    }

		    // Same thing with strike outs
		    if ( batting->simulated.acc_so[0] >= MAX_VALUE )
		    {
			 unsigned char x = batting->simulated.acc_so[0] - MAX_VALUE;

			 batting->action.acc_so[0]     = sim.acc_runs[0];

			 batting->simulated.acc_so[0] -= x;
			 batting->action.acc_so[0]  += x;
		    }
	       }
	       else
	       {
		    unsigned char games = playersFile[idx].filestats.filebatting.action.acc_games[0];

		    playersFile[idx].filestats.filebatting.action.acc_games[0] = 0;

		    playersFile[idx].filestats.filepitching.simulated.acc_starts[0] = games;
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
