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


struct
{
     int speed;
     int control;
     int fatigue;
     int bunt;
} pitcher_totals[11] = { 0 };

struct
{
     int power;
     int hit_n_run;
     int bunt;
     int running;
     int range;
     int arm;
} batter_totals[11] = { 0 };

static int total_pitchers = 0;
static int total_batters  = 0;


int main( int argc, char *argv[] )
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

          if ( playersFile[idx].position[0]>>4 == 0 )
          {
               // Pitcher
               struct pitching_s *p = &(playersFile[idx].filestats.filepitching);

               int speed   = (p->ratings[0]>>4);
               int control = (p->ratings[0] & 0x0F);
               int fatigue = (p->ratings[1]>>4);
               int bunt    = (p->ratings[1] & 0x0F);

               total_pitchers++;

               pitcher_totals[speed].speed++;
               pitcher_totals[control].control++;
               pitcher_totals[fatigue].fatigue++;
               pitcher_totals[bunt].bunt++;
          }
          else
          {
               // Batter
               struct batting_s *b = &(playersFile[idx].filestats.filebatting);

               int power     = (b->ratings[2]>>4);
               int hit_n_run = (b->ratings[3] & 0x0F);
               int bunt      = (b->ratings[3]>>4);
               int running   = (b->ratings[1]>>4);
               int range     = (b->ratings[1] & 0x0F);
               int arm       = (b->ratings[0] & 0x0F);

               total_batters++;

               batter_totals[power].power++;
               batter_totals[hit_n_run].hit_n_run++;
               batter_totals[bunt].bunt++;
               batter_totals[running].running++;
               batter_totals[range].range++;
               batter_totals[arm].arm++;
          }
     }

     free( playersFile );

     printf( "PITCHERS:\n" );
     printf( "\n" );
     printf( "Speed:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, pitcher_totals[i].speed, (pitcher_totals[i].speed == 0) ? 0 : (float)pitcher_totals[i].speed / (float)total_pitchers * 100.0 );
     }

     printf( "\n" );
     printf( "Control:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, pitcher_totals[i].control, (pitcher_totals[i].control == 0) ? 0 : (float)pitcher_totals[i].control / (float)total_pitchers * 100.0 );
     }

     printf( "\n" );
     printf( "Fatigue:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, pitcher_totals[i].fatigue, (pitcher_totals[i].fatigue == 0) ? 0 : (float)pitcher_totals[i].fatigue / (float)total_pitchers * 100.0 );
     }

     printf( "\n" );
     printf( "Bunt:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, pitcher_totals[i].bunt, (pitcher_totals[i].bunt == 0) ? 0 : (float)pitcher_totals[i].bunt / (float)total_pitchers * 100.0 );
     }

     printf( "\n" );
     printf( "\n" );
     printf( "BATTERS:\n" );
     printf( "\n" );
     printf( "Power:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, batter_totals[i].power, (batter_totals[i].power == 0) ? 0 : (float)batter_totals[i].power / (float)total_batters * 100.0 );
     }

     printf( "\n" );
     printf( "Hit_N_Run:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, batter_totals[i].hit_n_run, (batter_totals[i].hit_n_run == 0) ? 0 : (float)batter_totals[i].hit_n_run / (float)total_batters * 100.0 );
     }

     printf( "\n" );
     printf( "Bunt:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, batter_totals[i].bunt, (batter_totals[i].bunt == 0) ? 0 : (float)batter_totals[i].bunt / (float)total_batters * 100.0 );
     }

     printf( "\n" );
     printf( "Running:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, batter_totals[i].running, (batter_totals[i].running == 0) ? 0 : (float)batter_totals[i].running / (float)total_batters * 100.0 );
     }

     printf( "\n" );
     printf( "Range:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, batter_totals[i].range, (batter_totals[i].range == 0) ? 0 : (float)batter_totals[i].range / (float)total_batters * 100.0 );
     }

     printf( "\n" );
     printf( "Arm:\n" );

     for ( int i = 0; i < 11; ++i )
     {
          printf( "%-2d: %-3d %6.2f%%\n", i, batter_totals[i].arm, (batter_totals[i].arm == 0) ? 0 : (float)batter_totals[i].arm / (float)total_batters * 100.0 );
     }

     return EXIT_SUCCESS;
}
