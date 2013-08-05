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

// strike out average = .120 at 5 power
static const double ab_per_game =  3.85;
static const double ab_per_walk = 16.92;
static const double er_per_hr   =  3.00;
static const double er_per_walk =  2.26;

//                                  0       1       2       3       4       5       6       7       8       9      10
static const double ab_per_2b [] = { 0,  50.00,  43.19,  42.23,  45.91,  24.39,  26.76,  18.93,  17.03,  15.98,  20.27 };
static const double ab_per_3b [] = { 0, 160.00, 159.27, 157.95, 176.28,  89.25,  69.06,  45.06,  45.17,  38.61,  38.70 };
static const double ab_per_hr [] = { 0, 130.00, 129.58, 126.68, 117.52,  72.88,  49.65,  21.23,  17.38,   9.43,   8.13 };
static const double ab_per_rbi[] = { 0,  19.00,  16.50,  16.50,  16.50,  15.00,  12.50,  10.00,   9.50,   7.50,   7.00 };
static const double h_per_run [] = { 0,   4.50,   4.25,   4.00,   3.50,   3.25,   3.00,   2.90,   2.75,   2.50,   2.35 };


int main( int argc, char *argv[] )
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
          if ( playersFile[idx].last_name[0] == '\0' ) continue;

          if ( playersFile[idx].position[0]>>4 == 0 )
          {
               int earned_runs = playersFile[idx].filestats.filepitching.real_er[0];

               // Home Runs
               int base_hr = (int)ceil( (double)earned_runs / er_per_hr );

               playersFile[idx].filestats.filepitching.vl_hr[0] = (int)ceil( (double)base_hr / 2.0 ) + rand() % base_hr;

               // Walks
               int base_walks = (int)ceil( (double)earned_runs / er_per_walk );

               playersFile[idx].filestats.filepitching.vl_bb[0] = (int)ceil( (double)base_walks / 2.0 ) + rand() % base_walks;
          }
          else
          {
               int at_bats = word2int( playersFile[idx].filestats.filebatting.vl_ab   ) + word2int( playersFile[idx].filestats.filebatting.vr_ab   );
               int hits    = word2int( playersFile[idx].filestats.filebatting.vl_hits ) + word2int( playersFile[idx].filestats.filebatting.vr_hits );
               int power = (playersFile[idx].filestats.filebatting.ratings[2])>>4;
               int speed = (playersFile[idx].filestats.filebatting.ratings[1])>>4;

               // Games
               playersFile[idx].filestats.filebatting.real_games[0] = (int)ceil( (double)at_bats / ab_per_game );

               // Walks
               int base_walks = (int)ceil( (double)at_bats / ab_per_walk );

               playersFile[idx].filestats.filebatting.vl_bb[0] = (int)ceil( (double)base_walks / 2.0 ) + rand() % base_walks;

               // Doubles, Triples and Home Runs (only if empty)
               if ( playersFile[idx].filestats.filebatting.vl_2b[0] == 0 )
               {
                    int base_2b = (int)ceil((double)at_bats / ab_per_2b[power] );
                    int base_3b = (int)ceil((double)at_bats / ab_per_3b[power] );
                    int base_hr = (int)ceil((double)at_bats / ab_per_hr[power] );

                    playersFile[idx].filestats.filebatting.vl_2b[0] = (int)ceil( (double)base_2b / 2.0 ) + rand() % base_2b;
                    playersFile[idx].filestats.filebatting.vl_3b[0] = (int)ceil( (double)base_3b / 2.0 ) + rand() % base_3b;
                    playersFile[idx].filestats.filebatting.vl_hr[0] = (int)ceil( (double)base_hr / 2.0 ) + rand() % base_hr;
               }

               // Runs
               int base_runs = (int)ceil((double)hits / h_per_run[speed] );

               playersFile[idx].filestats.filebatting.real_runs[0]  = (int)ceil((double)base_runs * 0.75 ) + rand() % (int)ceil( (double)base_runs / 2.0 );
               playersFile[idx].filestats.filebatting.real_runs[0] += playersFile[idx].filestats.filebatting.vl_hr[0];

               // RBI
               int base_rbi = (int)ceil((double)at_bats / ab_per_rbi[power] );

               playersFile[idx].filestats.filebatting.real_rbi[0]  = (int)ceil((double)base_rbi * 0.75 ) + rand() % (int)ceil( (double)base_rbi / 2.0 );
               playersFile[idx].filestats.filebatting.real_rbi[0] += playersFile[idx].filestats.filebatting.vl_hr[0];
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
