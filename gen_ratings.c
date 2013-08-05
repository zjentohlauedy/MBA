#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void main()
{    //                      0  1   2    3     4     5     6    7    8   9 10
     int probabilities[] = { 0, 4, 72, 300, 1000, 4000, 2000, 500, 100, 16, 8 };
     int ratings[]       = { 0, 0,  0,   0,    0,    0,    0,   0,   0,  0, 0 };
     int i;
     int j;

     srand( time( NULL ) );

     for ( i = 0; i < 800; ++i )
     {
          int roll = (int)roundf( (float)rand() / (float)RAND_MAX * 8000.0 );

          for ( j = 0; j <= 10; ++j )
          {
               roll -= probabilities[j];

               if ( roll <= 0 )
               {
                    ratings[j]++;

                    break;
               }
          }
     }

     for ( i = 0; i <= 10; ++i )
     {
          printf( "% 2d: %3d\n", i, ratings[i] );
     }
}
