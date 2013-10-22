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


static fileplayer_s *findMatchingPlayer( fileplayer_s *player, fileplayer_s *players )
{
     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          if ( memcmp( player->first_name, players[i].first_name, sizeof(player->first_name) ) == 0 &&
               memcmp( player->last_name,  players[i].last_name,  sizeof(player->last_name)  ) == 0    )
          {
               return &players[i];
          }
     }

     return NULL;
}

int main( const int argc, const char *argv[] )
{
     fileplayer_s   *playersFile1;
     fileplayer_s   *playersFile2;
     fileplayer_s   *matchingPlayer;
     int             idx;


     if ( argc < 4 )
     {
	  printf( "Usage: %s <S01 file> <S02 file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (playersFile1 = readPlayersFile( argv[1] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[1] );

          return EXIT_SUCCESS;
     }

     if ( (playersFile2 = readPlayersFile( argv[2] )) == NULL )
     {
          printf( "Unable to load players file <%s>.\n", argv[2] );

          free( playersFile1 );

          return EXIT_SUCCESS;
     }

     for ( idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          if ( playersFile1[idx].last_name[0] == '\0' ) continue;

          if ( (matchingPlayer = findMatchingPlayer( &playersFile1[idx], playersFile2 )) == NULL )
          {
               printf( "Player <%.*s, %.*s> not found!\n",
                       sizeof(playersFile1[idx].last_name),  playersFile1[idx].last_name,
                       sizeof(playersFile1[idx].first_name), playersFile1[idx].first_name );

               free( playersFile1 );
               free( playersFile2 );

               return EXIT_FAILURE;
          }

          int pos = nibble( playersFile1[idx].position[0], n_High );

          acc_player_id_s *id_info1 = NULL;
          acc_player_id_s *id_info2 = NULL;

          if ( pos == pos_Pitcher )
          {
               struct pitching_s *pitching1 = &(playersFile1[idx].filestats.filepitching);
               struct pitching_s *pitching2 = &(matchingPlayer->filestats.filepitching);

               id_info1 = (acc_player_id_s *)&(pitching1->action);
               id_info2 = (acc_player_id_s *)&(pitching2->action);
          }
          else
          {
               struct batting_s *batting1 = &(playersFile1[idx].filestats.filebatting);
               struct batting_s *batting2 = &(matchingPlayer->filestats.filebatting);

               id_info1 = (acc_player_id_s *)&(batting1->action);
               id_info2 = (acc_player_id_s *)&(batting2->action);

               playersFile1[idx].filestats.filebatting.ratings[2] = matchingPlayer->filestats.filebatting.ratings[2];
          }

          if ( word2int( id_info1->player_id ) != 0 )
          {
               printf( "Player ID field for <%.*s, %.*s> already contains a number!\n",
                       sizeof(playersFile1[idx].last_name),  playersFile1[idx].last_name,
                       sizeof(playersFile1[idx].first_name), playersFile1[idx].first_name );

               free( playersFile1 );
               free( playersFile2 );

               return EXIT_FAILURE;
          }

          if ( id_info1->checksum[0] != 0 )
          {
               printf( "Checksum field for <%.*s, %.*s> already contains a number!\n",
                       sizeof(playersFile1[idx].last_name),  playersFile1[idx].last_name,
                       sizeof(playersFile1[idx].first_name), playersFile1[idx].first_name );

               free( playersFile1 );
               free( playersFile2 );

               return EXIT_FAILURE;
          }

          int2word( id_info1->player_id, word2int( id_info2->player_id ) );

          id_info1->checksum[0] = id_info2->checksum[0];
     }

     if ( ! writePlayersFile( argv[3], playersFile1 ) )
     {
          printf( "Cannot save player changes.\n" );

          free( playersFile1 );
          free( playersFile2 );

          return EXIT_FAILURE;
     }

     free( playersFile1 );
     free( playersFile2 );

     return EXIT_SUCCESS;
}
