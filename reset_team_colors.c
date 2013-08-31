#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include "ewbfiles.h"


static struct {

     unsigned char color1;
     unsigned char color2;
     int           alternate;

} colors[] = {

     { /* Sabres    */ cl_Blue,   cl_LtBlue, 0 },
     { /* Portsmen  */ cl_LtBlue, cl_Green,  0 },
     { /* Settlers  */ cl_Brown,  cl_Red,    0 },
     { /* Kings     */ cl_Red,    cl_Blue,   0 },
     { /* Voyagers  */ cl_Blue,   cl_Gold,   0 },
     { /* Rockets   */ cl_DkRed,  cl_Green,  0 },
     { /* Knights   */ cl_Black,  cl_Blue,   0 },
     { /* Drizzle   */ cl_LtBlue, cl_Brown,  0 },

     { /* Scorpions */ cl_Black,  cl_Brown,  0 },
     { /* Lightning */ cl_Gold,   cl_LtBlue, 0 },
     { /* Goblins   */ cl_Green,  cl_Black,  0 },
     { /* Photons   */ cl_Red,    cl_DkRed,  0 },
     { /* Dragons   */ cl_Gold,   cl_Green,  0 },
     { /* Hammers   */ cl_Black,  cl_DkRed,  0 },
     { /* Expos     */ cl_DkRed,  cl_Blue,   1 },
     { /* Dynamo    */ cl_Red,    cl_LtBlue, 0 },

     { /* Aces      */ cl_Black,  cl_Red,    0 },
     { /* Cyclone   */ cl_Gold,   cl_Blue,   1 },
     { /* Stormers  */ cl_Blue,   cl_Black,  0 },
     { /* Express   */ cl_Brown,  cl_Gold,   0 },
     { /* Warriors  */ cl_DkRed,  cl_Brown,  0 },
     { /* Wanderers */ cl_LtBlue, cl_Red,    0 },
     { /* Thunder   */ cl_Blue,   cl_Brown,  0 },
     { /* Glory     */ cl_Red,    cl_Black,  0 },

     { /* Eclipse   */ cl_Black,  cl_Gold,   0 },
     { /* Legends   */ cl_Blue,   cl_Red,    0 },
     { /* Waves     */ cl_Blue,   cl_Green,  0 },
     { /* Horizon   */ cl_Green,  cl_LtBlue, 0 },
     { /* Sharks    */ cl_LtBlue, cl_DkRed,  0 },
     { /* Flames    */ cl_Red,    cl_Gold,   0 },
     { /* Techs     */ cl_DkRed,  cl_Red,    0 },
     { /* Quasars   */ cl_LtBlue, cl_Black,  0 }
};


int main( const int argc, const char *argv[] )
{
     fileleague_s *leagueFile;
     fileleague_s  before;
     char          home[20];
     char          road[20];
     int           i;


     srand( time( NULL ) );

     if ( argc < 3 )
     {
	  printf( "Usage: %s <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file." );

          return EXIT_FAILURE;
     }

     before = *leagueFile;

     for ( i = 0; i < TOTAL_TEAMS; ++i )
     {
	  leagueFile->teams[i].color[0] = colors[i].color1;
     }

     for ( i = 0; i < TOTAL_TEAMS; ++i )
     {
	  if ( before.teams[i].color[0] != leagueFile->teams[i].color[0] )
	  {
	       printf( "%s changed from %s to %s uniforms.\n",
		       leagueFile->teams[i].name,
		       getColorName( before.teams[i].color[0] ),
		       getColorName( leagueFile->teams[i].color[0] ) );
	  }
     }

     if ( ! writeLeagueFile( argv[2], leagueFile ) )
     {
          printf( "Cannot save league changes.\n" );

          free( leagueFile );

          return EXIT_FAILURE;
     }

     free( leagueFile );

     return EXIT_SUCCESS;
}
