#include <stdio.h>
#include <string.h>
#include "file_formats.h"


static char error_message[999 + 1] = { 0 };


char *getCopyPlayerIdsError( void )
{
     return error_message;
}


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


boolean_e copyPlayerIds( fileplayer_s *players_file1, fileplayer_s *players_file2 )
{
     fileplayer_s   *matchingPlayer;


     clearErrorMessage();

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          if ( EMPTY( players_file1[i].last_name ) ) continue;

          if ( (matchingPlayer = findMatchingPlayer( &players_file1[i], players_file2 )) == NULL )
          {
               sprintf( error_message, "Player <%.*s, %.*s> not found!",
                       sizeof(players_file1[i].last_name),  players_file1[i].last_name,
                       sizeof(players_file1[i].first_name), players_file1[i].first_name );

               return bl_False;
          }

          acc_player_id_s *id_info1 = &(players_file1[i].acc_stats.action.id_info);
          acc_player_id_s *id_info2 = &(matchingPlayer->acc_stats.action.id_info);

          if ( word2int( id_info1->player_id ) != 0 )
          {
               sprintf( error_message, "Player ID field for <%.*s, %.*s> already contains a number!",
                       sizeof(players_file1[i].last_name),  players_file1[i].last_name,
                       sizeof(players_file1[i].first_name), players_file1[i].first_name );

               return bl_False;
          }

          if ( byte2int( id_info1->checksum ) != 0 )
          {
               sprintf( error_message, "Checksum field for <%.*s, %.*s> already contains a number!",
                       sizeof(players_file1[i].last_name),  players_file1[i].last_name,
                       sizeof(players_file1[i].first_name), players_file1[i].first_name );

               return bl_False;
          }

          int2word( id_info1->player_id, word2int( id_info2->player_id ) );
          int2byte( id_info1->checksum,  byte2int( id_info2->checksum  ) );
     }

     return bl_True;
}
