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


int main( const int argc, const char *argv[] )
{
     fileplayer_s   *playersFile;
     int             idx;
     int             player_id;


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

     for ( player_id = 1, idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          if ( playersFile[idx].last_name[0] == '\0' ) continue;

          int pos = nibble( playersFile[idx].position[0], n_High );

          acc_player_id_s *id_info = NULL;

          if ( pos == pos_Pitcher )
          {
               struct pitching_s *pitching = &(playersFile[idx].filestats.filepitching);

               id_info = (acc_player_id_s *)&(pitching->action);
          }
          else
          {
               struct batting_s *batting = &(playersFile[idx].filestats.filebatting);

               id_info = (acc_player_id_s *)&(batting->action);

               unsigned char longevity = (rand() % 10) + 1;

               batting->ratings[2] += longevity;
          }

          if ( word2int( id_info->player_id ) != 0 )
          {
               printf( "Player ID field already contains a number!\n" );

               free( playersFile );

               return EXIT_FAILURE;
          }

          if ( id_info->checksum[0] != 0 )
          {
               printf( "Checksum field already contains a number!\n" );

               free( playersFile );

               return EXIT_FAILURE;
          }

          int2word( id_info->player_id, player_id );

          id_info->checksum[0] = calcChecksum( player_id );

          player_id++;
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
