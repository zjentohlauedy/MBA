#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ewbfiles.h"


int main( const int argc, const char *argv[] )
{
     fileplayer_s   *playersFile;
     int             idx;

     const struct
     {
          unsigned int put_outs;
          unsigned int assists;

     } fielding[16] = {

	  {    0,   0 }, // P
	  {  600,  75 }, // C
	  { 1300, 100 }, // 1b
	  {  350, 400 }, // 2b
	  {  150, 300 }, // 3b
	  {  300, 450 }, // SS
	  {  300,  10 }, // LF
	  {  300,  10 }, // CF
	  {  300,  10 }, // RF
	  {  300, 300 }, // IF
	  {  300,  10 }, // OF
	  {    0,   0 },
	  {    0,   0 },
	  {    0,   0 },
	  {    0,   0 },
	  {    0,   0 }
     };


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
		    struct pitching_s *pitching = &(playersFile[idx].filestats.filepitching);

                    pitching->real_fa[0] = 1;
	       }
	       else
	       {
		    struct batting_s *batting = &(playersFile[idx].filestats.filebatting);

                    unsigned int po = fielding[pos].put_outs;
                    unsigned int as = fielding[pos].assists;

                    int2word( batting->real_po, po );
                    int2word( batting->real_as, as );

                    batting->real_err[0] = 0;
                    batting->real_fa2[0] = 1;

                    playersFile[idx].position[0] &= 0xF0;
                    playersFile[idx].position[0] += pos_Designate;
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
