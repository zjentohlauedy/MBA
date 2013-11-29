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
          if ( EMPTY( players_file[idx].last_name ) ) continue;

          acc_player_id_s *id_info = &(players_file[idx].acc_stats.action.id_info);

          if ( word2int( id_info->player_id ) != 0 )
          {
               sprintf( error_message, "Player ID field already contains a number!" );

               return bl_False;
          }

          if ( byte2int( id_info->checksum ) != 0 )
          {
               sprintf( error_message, "Checksum field already contains a number!" );

               return bl_False;
          }

          int2word( id_info->player_id,               player_id   );
          int2byte( id_info->checksum,  calcChecksum( player_id ) );

          player_id++;
     }

     return bl_True;
}
