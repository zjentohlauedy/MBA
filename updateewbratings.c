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


typedef struct
{
     int start;
     int range;

} random_s;

static const random_s vs_batting_avg[10][10] = {
     { { 250, 35 }, { 235, 20 }, { 225, 15 }, { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 } },
     { { 235, 20 }, { 225, 15 }, { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 } },
     { { 225, 15 }, { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 } },
     { { 215, 15 }, { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 } },
     { { 205, 15 }, { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 } },
     { { 200, 10 }, { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 } },
     { { 195, 10 }, { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 } },
     { { 190, 10 }, { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 }, { 140, 15 } },
     { { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 }, { 140, 15 }, { 130, 15 } },
     { { 182,  8 }, { 180,  7 }, { 175, 10 }, { 170, 10 }, { 165, 10 }, { 160, 10 }, { 150, 15 }, { 140, 15 }, { 130, 15 }, { 115, 20 } }
};

static const random_s earned_run_avg[10][10] = {
     { { 290, 35 }, { 265, 35 }, { 245, 30 }, { 230, 25 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 } },
     { { 265, 30 }, { 245, 30 }, { 230, 25 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 } },
     { { 245, 25 }, { 230, 25 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 } },
     { { 230, 20 }, { 220, 20 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 } },
     { { 220, 15 }, { 210, 20 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 } },
     { { 210, 15 }, { 200, 20 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 } },
     { { 200, 15 }, { 190, 20 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 } },
     { { 190, 15 }, { 185, 15 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 }, { 120, 20 } },
     { { 185, 10 }, { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 }, { 120, 20 }, { 100, 25 } },
     { { 182,  8 }, { 180,  7 }, { 175, 10 }, { 165, 15 }, { 155, 15 }, { 145, 15 }, { 135, 15 }, { 120, 20 }, { 100, 25 }, {  75, 30 } }
};


static int roll( const int start, const int range )
{
     return (rand() % (range + 1)) + start;
}


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

     for ( idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
	  if ( playersFile[idx].last_name[0] != 0 )
	  {
	       unsigned int pos = playersFile[idx].position[0] / 0x10;

	       if   ( pos == pos_Pitcher )
	       {
		    struct pitching_s *p = &(playersFile[idx].filestats.filepitching);

                    float innings = (float)word2int( p->real_inn ) / 10.0;
                    int at_bats = word2int( p->vl_ab   ) + word2int( p->vr_ab   );
                    int hits = word2int( p->vl_hits ) + word2int( p->vr_hits );
                    int earned_runs = p->real_er[0];
                    float norm_inn  = ((int)innings);
                    /**/  norm_inn += (innings - norm_inn) / 3.0;
                    float era = ((float)earned_runs) / norm_inn * 9.0;
                    float vsba = ((float)hits) / ((float)at_bats);

                    int iera = (int)round(era * 100.0);
                    int ivsba = (int)round(vsba * 1000.0);
                    int speeds[100] = { 0 };
                    int controls[100] = { 0 };
                    int found = 0;

                    for ( int i = 0; i < 10; ++i )
                    {
                         for ( int j = 0; j < 10; ++j )
                         {
                              const random_s *cell = &vs_batting_avg[i][j];

                              if ( cell->start < ivsba  &&  (cell->start + cell->range) > ivsba )
                              {
                                   speeds[found] = i + 1;
                                   controls[found] = j + 1;
                                   found++;
                              }
                         }
                    }

                    int x = roll( 0, found);

                    printf( "%s, %s: %d %d\n", playersFile[idx].last_name, playersFile[idx].first_name, speeds[x], controls[x] );
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
