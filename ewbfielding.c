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
     unsigned int count;
     unsigned int games;
     unsigned int put_outs;
     unsigned int assists;
     unsigned int errors;

} fielding_s;


static void print_results( const fielding_s *results )
{
     printf( "Pos   #   G   PO(/Game)   AS(/Game)   E   AVG\n" );
     //        C:  24 127  747( 5.90)   51( 0.40)   6  .992

     for ( int i = 1; i < 16; ++i )
     {
          const fielding_s *f = &results[i];

	  if ( f->count > 0 )
	  {
               int total = f->put_outs + f->assists + f->errors;

	       printf( "%2s:  %2d %3d %4d(%5.2f)  %3d(%5.2f)  %2d  .%03d\n",
		       positionName( i ),
		       f->count,
                       (int)round((double)f->games / (double)f->count),
		       (int)round((double)f->put_outs / (double)f->count),
                       (double)f->put_outs / (double)f->games,
		       (int)round((double)f->assists / (double)f->count),
                       (double)f->assists / (double)f->games,
		       (int)round((double)f->errors / (double)f->count),
		       total > 0 ? (int)((float)(f->put_outs + f->assists) / (float)(total) * 1000.0) : 0
		    );
	  }
     }
}


int main( const int argc, const char *argv[] )
{
     fielding_s    totals[16] = { 0 };
     fielding_s    maxpo[16] = { 0 };
     fielding_s    maxas[16] = { 0 };
     fielding_s    minpo[16] = { 0 };
     fielding_s    minas[16] = { 0 };
     fielding_s    maxer[16] = { 0 };
     fileplayer_s *playersFile;
     int           i;


     if ( argc < 2 )
     {
	  printf( "Must supply filename.\n" );

	  return EXIT_SUCCESS;
     }

     if ( (playersFile = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Cannot load players file." );

          return EXIT_FAILURE;
     }

     for ( i = 0; i < TOTAL_PLAYERS; ++i )
     {
	  if ( playersFile[i].last_name[0] == '\0' ) continue;

	  unsigned int pos = playersFile[i].position[0]>>4;

	  if ( pos == pos_Pitcher ) continue;

          struct batting_s *batting = &(playersFile[i].filestats.filebatting);

          int ab = word2int( batting->vl_ab ) + word2int( batting->vr_ab );

          if ( ab < 300 ) continue;

          unsigned int g   = batting->real_games[0];
          unsigned int po  = (batting->real_po[0]<<8) + batting->real_po[1];
          unsigned int as  = (batting->real_as[0]<<8) + batting->real_as[1];
          unsigned int err = batting->real_err[0];

          // totals
          totals[pos].count++;
          totals[pos].games    += g;
          totals[pos].put_outs += po;
          totals[pos].assists  += as;
          totals[pos].errors   += err;

          // max po
          if ( maxpo[pos].put_outs < po )
          {
               maxpo[pos].count    = 1;
               maxpo[pos].games    = g;
               maxpo[pos].put_outs = po;
               maxpo[pos].assists  = as;
               maxpo[pos].errors   = err;
          }

          // max as
          if ( maxas[pos].assists < as )
          {
               maxas[pos].count    = 1;
               maxas[pos].games    = g;
               maxas[pos].put_outs = po;
               maxas[pos].assists  = as;
               maxas[pos].errors   = err;
          }

          // min po
          if ( minpo[pos].put_outs == 0  ||  minpo[pos].put_outs > po )
          {
               minpo[pos].count    = 1;
               minpo[pos].games    = g;
               minpo[pos].put_outs = po;
               minpo[pos].assists  = as;
               minpo[pos].errors   = err;
          }

          // min as
          if ( minas[pos].assists == 0  ||  minas[pos].assists > as )
          {
               minas[pos].count    = 1;
               minas[pos].games    = g;
               minas[pos].put_outs = po;
               minas[pos].assists  = as;
               minas[pos].errors   = err;
          }

          // max er
          if ( maxer[pos].errors < err )
          {
               maxer[pos].count    = 1;
               maxer[pos].games    = g;
               maxer[pos].put_outs = po;
               maxer[pos].assists  = as;
               maxer[pos].errors   = err;
          }
     }

     free( playersFile );

     printf( "Averages:\n" );
     print_results( totals );
     printf( "\n" );
     printf( "Max Put Outs:\n" );
     print_results( maxpo );
     printf( "\n" );
     printf( "Min Put Outs:\n" );
     print_results( minpo );
     printf( "\n" );
     printf( "Max Assists:\n" );
     print_results( maxas );
     printf( "\n" );
     printf( "Min Assists:\n" );
     print_results( minas );
     printf( "\n" );
     printf( "Max Errors:\n" );
     print_results( maxer );
     printf( "\n" );

     return EXIT_SUCCESS;
}
