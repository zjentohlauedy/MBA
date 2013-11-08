#include <stdio.h>
#include "file_formats.h"


static char error_message[999 + 1] = { 0 };


char *getGenPlayerIdsError( void )
{
     return error_message;
}


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


boolean_e genPlayerIds( fileplayer_s *players_file )
{
     clearErrorMessage();

     for ( int player_id = 1, idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          if ( players_file[idx].last_name[0] == '\0' ) continue;

          acc_player_id_s *id_info = &(players_file[idx].acc_stats.amiga.action.id_info);

          if ( word2int( id_info->player_id ) != 0 )
          {
               sprintf( error_message, "Player ID field already contains a number!" );

               return bl_False;
          }

          if ( id_info->checksum[0] != 0 )
          {
               sprintf( error_message, "Checksum field already contains a number!" );

               return bl_False;
          }

          int2word( id_info->player_id, player_id );

          id_info->checksum[0] = calcChecksum( player_id );

          player_id++;
     }

     return bl_True;
}
