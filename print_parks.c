#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ewbfiles.h"


static int getWindPercentage( int value )
{
     switch ( value )
     {
     case  0: return  0;
     case  1: return 98;
     case  2: return 97;
     case  3: return 95;
     case  4: return 94;
     case  5: return 92;
     case  6: return 91;
     case  7: return 89;
     case  8: return 88;
     case  9: return 86;
     case 10: return 84;
     case 11: return 83;
     case 12: return 81;
     case 13: return 80;
     case 14: return 78;
     case 15: return 77;
     case 16: return 75;
     case 17: return 73;
     case 18: return 72;
     case 19: return 70;
     case 20: return 69;
     case 21: return 67;
     case 22: return 66;
     case 23: return 64;
     case 24: return 63;
     case 25: return 61;
     case 26: return 59;
     case 27: return 58;
     case 28: return 56;
     case 29: return 55;
     case 30: return 53;
     case 31: return 52;
     }

     return -1;
}


static char *getWindDirection( int direction )
{
     switch ( direction )
     {
     case 0: return "Normal";
     case 1: return "Out";
     case 2: return "In";
     case 3: return "Across";
     }

     return "Unknown";
}


static int getCapacity( int value )
{
     switch ( value )
     {
     case 0: return 35000;
     case 1: return 43000;
     case 2: return 50000;
     case 3: return 56000;
     }

     return -1;
}


static void print_park_details( park_details_s *details )
{
     if   ( details->details1[0] & 0x80 ) printf( "Inside " );
     else                                 printf( "Outside " );

     if   ( details->details1[0] & 0x40 ) printf( "Artificial " );
     else                                 printf( "Natural " );

     printf( "Color: %s\n", getParkColorName( nibble( details->details1[0], n_Low ) & 0x07 ) );

     printf( "Bleachers:\n" );
     printf( "  Left:         %d\n", 1 + ((details->bleachers[0] & 0xC0) >> 6) );
     printf( "  Left-Center:  %d\n", 1 + ((details->bleachers[0] & 0x0C) >> 2) );
     printf( "  Right-Center: %d\n", 1 + ((details->bleachers[0] & 0x03) >> 0) );
     printf( "  Right:        %d\n", 1 + ((details->bleachers[0] & 0x30) >> 4) );

     int details2 = word2int( details->details2 );

     printf( "Wind Speed: %d %s %d%%\n", (details2 & 0xF800) >> 11, getWindDirection( (details2 & 0x0600) >> 9 ), getWindPercentage( (details2 & 0x1F0) >> 4 ) );
     printf( "Capacity: %dk\n", getCapacity( details2 & 0x0003 ) );
/*
     distances_s *distances = &(details->distances);

     printf( "Fences: %d - %d - %d - %d - %d\n",
             word2int( distances->left_line ),
             word2int( distances->left_center ),
             word2int( distances->center ),
             word2int( distances->right_center ),
             word2int( distances->right_line ) );
*/
     heights_s *heights = &(details->heights);

     printf( "Heights:       %2d   %2d   %2d   %2d   %2d   %2d   %2d   %2d   %2d\n",
             heights->left1[0],
             heights->left2[0],
             heights->left3[0],
             heights->left4[0],
             heights->center[0],
             heights->right4[0],
             heights->right3[0],
             heights->right2[0],
             heights->right1[0] );

     printf( "Distances:" );
     for ( int i = 0; i < 10; ++i )
     {
          short x = (short)word2int( details->outfield[i].x );
          short y = (short)word2int( details->outfield[i].y );

          printf( "  %3d", (int)round( sqrt( pow((double)x, 2.0) + pow((double)y, 2.0) ) ) );
     }
     printf( "\n" );
/*
     printf( "Outfield: " );
     for ( int i = 0; i < 10; ++i )
     {
          printf( "(%d, %d) ", (short)word2int( details->outfield[i].x ), (short)word2int( details->outfield[i].y ) );
     }
     printf( "\n" );

     printf( "Infield1: " );
     for ( int i = 0; i < 8; ++i )
     {
          printf( "(%d, %d) ", (short)word2int( details->infield1[i].x ), (short)word2int( details->infield1[i].y ) );
     }
     printf( "\n" );

     printf( "Infield2: " );
     for ( int i = 0; i < 8; ++i )
     {
          printf( "(%d, %d) ", (short)word2int( details->infield2[i].x ), (short)word2int( details->infield2[i].y ) );
     }
     printf( "\n" );
*/
}


int main( int argc, char *argv[] )
{
     fileparks_s *parksFile;

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
          printf( "%s\n", parksFile->park_names[i].text );

          print_park_details( &parksFile->park_details[i] );

          printf( "\n" );
     }

     free( parksFile );

     return EXIT_SUCCESS;
}
