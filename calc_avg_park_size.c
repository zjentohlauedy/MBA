#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ewbfiles.h"


static int hyp( short x, short y )
{
     return (int)round( sqrt( pow((double)x, 2.0) + pow((double)y, 2.0) ) );
}


int main( const int argc, const char *argv[] )
{
     fileparks_s *parksFile;

     struct
     {
          int left_line;
          int left_lc;
          int left_ctr;
          int ctr_cl;
          int ctr_left;
          int ctr_right;
          int ctr_rl;
          int right_ctr;
          int right_rc;
          int right_line;
     }
     totals = { 0 };

     struct
     {
          int left1;
          int left2;
          int left3;
          int left4;
          int center;
          int right4;
          int right3;
          int right2;
          int right1;

     }
     total_heights = { 0 };

     if ( argc < 2 )
     {
	  printf( "Usage: %s <input file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (parksFile = readParksFile( argv[1] )) == NULL )
     {
          printf( "Cannot load parks file." );

          return EXIT_FAILURE;
     }

     // Do stuff
     for ( int i = 0; i < 32; ++i )
     {
          coords_s *outfield = parksFile->park_details[i].outfield;

          totals.left_line  += hyp( (short)word2int( outfield[0].x ), (short)word2int( outfield[0].y ) );
          totals.left_lc    += hyp( (short)word2int( outfield[1].x ), (short)word2int( outfield[1].y ) );
          totals.left_ctr   += hyp( (short)word2int( outfield[2].x ), (short)word2int( outfield[2].y ) );
          totals.ctr_cl     += hyp( (short)word2int( outfield[3].x ), (short)word2int( outfield[3].y ) );
          totals.ctr_left   += hyp( (short)word2int( outfield[4].x ), (short)word2int( outfield[4].y ) );
          totals.ctr_right  += hyp( (short)word2int( outfield[5].x ), (short)word2int( outfield[5].y ) );
          totals.ctr_rl     += hyp( (short)word2int( outfield[6].x ), (short)word2int( outfield[6].y ) );
          totals.right_ctr  += hyp( (short)word2int( outfield[7].x ), (short)word2int( outfield[7].y ) );
          totals.right_rc   += hyp( (short)word2int( outfield[8].x ), (short)word2int( outfield[8].y ) );
          totals.right_line += hyp( (short)word2int( outfield[9].x ), (short)word2int( outfield[9].y ) );

          heights_s *heights = &(parksFile->park_details[i].heights);

          total_heights.left1  += heights->left1[0];
          total_heights.left2  += heights->left2[0];
          total_heights.left3  += heights->left3[0];
          total_heights.left4  += heights->left4[0];
          total_heights.center += heights->center[0];
          total_heights.right4 += heights->right4[0];
          total_heights.right3 += heights->right3[0];
          total_heights.right2 += heights->right2[0];
          total_heights.right1 += heights->right1[0];
     }

     free( parksFile );

     printf( "   %2d   %2d   %2d   %2d   %2d   %2d   %2d   %2d   %2d\n",
             (int)round( (double)total_heights.left1  / 32.0 ),
             (int)round( (double)total_heights.left2  / 32.0 ),
             (int)round( (double)total_heights.left3  / 32.0 ),
             (int)round( (double)total_heights.left4  / 32.0 ),
             (int)round( (double)total_heights.center / 32.0 ),
             (int)round( (double)total_heights.right4 / 32.0 ),
             (int)round( (double)total_heights.right3 / 32.0 ),
             (int)round( (double)total_heights.right2 / 32.0 ),
             (int)round( (double)total_heights.right1 / 32.0 ) );

     printf( "%3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d\n",
             (int)round( (double)totals.left_line  / 32.0 ),
             (int)round( (double)totals.left_lc    / 32.0 ),
             (int)round( (double)totals.left_ctr   / 32.0 ),
             (int)round( (double)totals.ctr_cl     / 32.0 ),
             (int)round( (double)totals.ctr_left   / 32.0 ),
             (int)round( (double)totals.ctr_right  / 32.0 ),
             (int)round( (double)totals.ctr_rl     / 32.0 ),
             (int)round( (double)totals.right_ctr  / 32.0 ),
             (int)round( (double)totals.right_rc   / 32.0 ),
             (int)round( (double)totals.right_line / 32.0 ) );

     return EXIT_SUCCESS;
}
