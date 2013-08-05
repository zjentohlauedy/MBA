#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

typedef enum
{
     cl_LtBlue = 0,
     cl_Blue,
     cl_DkRed,
     cl_Red,
     cl_Gold,
     cl_Black,
     cl_Brown,
     cl_Green

} colors_e;

typedef struct
{
     unsigned char name   [12];
     unsigned char color  [ 1];
     unsigned char wins   [ 1];
     unsigned char losses [ 1];
     unsigned char term   [ 1];

} team_s;

typedef struct
{
     unsigned char header[52];
     unsigned char league1[12];
     unsigned char league2[12];
     unsigned char division1[12];
     unsigned char division2[12];
     unsigned char division3[12];
     unsigned char division4[12];
     team_s        teams[32];

} league_s;

static struct {

     unsigned char color1;
     unsigned char color2;
     int           percent;

} colors[] = {

     { /* Sabres    */ cl_Blue,   cl_LtBlue, 80 },
     { /* Portsmen  */ cl_LtBlue, cl_Green,  95 },
     { /* Settlers  */ cl_Brown,  cl_Green,  75 },
     { /* Kings     */ cl_LtBlue, cl_LtBlue, 50 },
     { /* Voyagers  */ cl_Blue,   cl_Blue,   50 },
     { /* Rockets   */ cl_DkRed,  cl_DkRed,  50 },
     { /* Knights   */ cl_Black,  cl_Black,  50 },
     { /* Drizzle   */ cl_LtBlue, cl_Brown,  80 },

     { /* Scorpions */ cl_Black,  cl_Brown,  65 },
     { /* Lightning */ cl_LtBlue, cl_Gold,   80 },
     { /* Goblins   */ cl_Green,  cl_Green,  50 },
     { /* Photons   */ cl_Red,    cl_Red,    50 },
     { /* Dragons   */ cl_Gold,   cl_Green,  90 },
     { /* Hammers   */ cl_Black,  cl_DkRed,  65 },
     { /* Expos     */ cl_Blue,   cl_DkRed,  50 },
     { /* Dynamo    */ cl_Red,    cl_LtBlue, 90 },

     { /* Aces      */ cl_Black,  cl_Black,  50 },
     { /* Cyclone   */ cl_Green,  cl_Blue,   50 },
     { /* Stormers  */ cl_Blue,   cl_Black,  80 },
     { /* Express   */ cl_Gold,   cl_LtBlue, 75 },
     { /* Warriors  */ cl_DkRed,  cl_Brown,  95 },
     { /* Wanderers */ cl_LtBlue, cl_Red,    90 },
     { /* Thunder   */ cl_Blue,   cl_Blue,   50 },
     { /* Glory     */ cl_LtBlue, cl_Gold,   65 },

     { /* Eclipse   */ cl_Black,  cl_Gold,   75 },
     { /* Legends   */ cl_Blue,   cl_Blue,   50 },
     { /* Waves     */ cl_Blue,   cl_Green,  90 },
     { /* Horizon   */ cl_Green,  cl_Green,  50 },
     { /* Sharks    */ cl_LtBlue, cl_LtBlue, 50 },
     { /* Flames    */ cl_Red,    cl_Gold,   80 },
     { /* Techs     */ cl_Blue,   cl_Blue,   50 },
     { /* Quasars   */ cl_LtBlue, cl_Black,  80 }
};

static char *getColorName( unsigned int color )
{
     switch ( color )
     {
     case cl_LtBlue: return "Lt. Blue";
     case cl_Blue:   return "Blue";
     case cl_DkRed:  return "Dark Red";
     case cl_Red:    return "Red";
     case cl_Gold:   return "Gold";
     case cl_Black:  return "Black";
     case cl_Brown:  return "Brown";
     case cl_Green:  return "Green";
     default:        return "Unknown";
     }
}

int roll( int range )
{
     return (int)ceil( ((double)rand() / (double)RAND_MAX) * (double)range );
}

int main( int argc, char *argv[] )
{
     league_s  before;
     league_s *league;
     time_t    t;
     char     *filename_r;
     char     *filename_w;
     char      input_buf[sizeof(league_s)];
     int       fd_r;
     int       fd_w;
     int       bytes_read;
     int       bytes_written;


     t = time( NULL );

     srand( t );

     if ( argc < 3 )
     {
	  printf( "Usage: %s <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     filename_r = argv[1];
     filename_w = argv[2];

     if ( (fd_r = open( filename_r, O_RDONLY )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename_r, strerror(errno) );

	  return EXIT_FAILURE;
     }

     if ( (bytes_read = read( fd_r, input_buf, sizeof(league_s) )) < sizeof(league_s) )
     {
	  printf( "Warning: data may be truncated!\n" );
     }

     if ( close( fd_r ) < 0 )
     {
	  printf( "Error closing input file: %s\n", strerror(errno) );

	  return EXIT_FAILURE;
     }

     league = (league_s *)input_buf;
     before = *league;

     int i;

     for ( i = 0; i < 32; ++i )
     {
	  int x = roll( 100 );

	  if ( (x - colors[i].percent) <= 0 )
	  {
	       league->teams[i].color[0] = colors[i].color1;
	  }
	  else
	  {
	       league->teams[i].color[0] = colors[i].color2;
	  }
     }

     for ( i = 0; i < 32; ++i )
     {
	  if ( before.teams[i].color[0] != league->teams[i].color[0] )
	  {
	       printf( "%s changed from %s to %s uniforms.\n",
		       league->teams[i].name,
		       getColorName( before.teams[i].color[0] ),
		       getColorName( league->teams[i].color[0] ) );
	  }
     }

     if ( (fd_w = creat( filename_w, S_IRUSR | S_IWUSR )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename_w, strerror(errno) );

	  return EXIT_FAILURE;
     }

     if ( (bytes_written = write( fd_w, input_buf, sizeof(league_s) )) < bytes_read )
     {
	  if ( bytes_written < 0 )
	  {
	       printf( "Error writing to output file: %s\n", strerror( errno ) );

	       if ( close( fd_w ) < 0 )
	       {
		    printf( "Error closing output file: %s\n", strerror(errno) );
	       }

	       return EXIT_FAILURE;
	  }

	  printf( "Warning: incomplete buffer written to output file!\n" );
     }

     if ( close( fd_w ) < 0 )
     {
	  printf( "Error closing output file: %s\n", strerror(errno) );

	  return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
