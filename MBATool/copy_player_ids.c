#include <stdio.h>
#include <string.h>
#include "file_formats.h"


boolean_e copyPlayerIds( fileplayer_s *players_file1, fileplayer_s *players_file2 )
{
     fileplayer_s   *matchingPlayer;
     int             idx;


     for ( idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          if ( players_file1[idx].last_name[0] == '\0' ) continue;

          if ( (matchingPlayer = findMatchingPlayer( &players_file1[idx], players_file2 )) == NULL )
          {
               printf( "Player <%.*s, %.*s> not found!\n",
                       sizeof(players_file1[idx].last_name),  players_file1[idx].last_name,
                       sizeof(players_file1[idx].first_name), players_file1[idx].first_name );

               return bl_False;
          }

          int pos = nibble( players_file1[idx].position[0], n_High );

          acc_player_id_s *id_info1 = NULL;
          acc_player_id_s *id_info2 = NULL;

          if ( pos == fpos_Pitcher )
          {
               filepitching_s *pitching1 = &(players_file1[idx].filestats.filepitching);
               filepitching_s *pitching2 = &(matchingPlayer->filestats.filepitching);

               id_info1 = (acc_player_id_s *)&(pitching1->action);
               id_info2 = (acc_player_id_s *)&(pitching2->action);
          }
          else
          {
               filebatting_s *batting1 = &(players_file1[idx].filestats.filebatting);
               filebatting_s *batting2 = &(matchingPlayer->filestats.filebatting);

               id_info1 = (acc_player_id_s *)&(batting1->action);
               id_info2 = (acc_player_id_s *)&(batting2->action);
          }

          if ( word2int( id_info1->player_id ) != 0 )
          {
               printf( "Player ID field for <%.*s, %.*s> already contains a number!\n",
                       sizeof(players_file1[idx].last_name),  players_file1[idx].last_name,
                       sizeof(players_file1[idx].first_name), players_file1[idx].first_name );

               return bl_False;
          }

          if ( id_info1->checksum[0] != 0 )
          {
               printf( "Checksum field for <%.*s, %.*s> already contains a number!\n",
                       sizeof(players_file1[idx].last_name),  players_file1[idx].last_name,
                       sizeof(players_file1[idx].first_name), players_file1[idx].first_name );

               return bl_False;
          }

          int2word( id_info1->player_id, word2int( id_info2->player_id ) );

          id_info1->checksum[0] = id_info2->checksum[0];
     }

     return bl_True;
}
