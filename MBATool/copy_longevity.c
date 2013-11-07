#include <stdio.h>
#include "file_formats.h"


static char error_message[999 + 1] = { 0 };


char *getCopyLongevityError( void )
{
     return error_message;
}


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


boolean_e copyLongevity( fileplayer_s *players_file1, fileplayer_s *players_file2 )
{
     fileplayer_s *matchingPlayer;

     clearErrorMessage();

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          if ( players_file1[i].last_name[0] == '\0' ) continue;

          if ( (matchingPlayer = findMatchingPlayer( &players_file1[i], players_file2 )) == NULL )
          {
               sprintf( error_message, "Player <%.*s, %.*s> not found!",
                       sizeof(players_file1[i].last_name),  players_file1[i].last_name,
                       sizeof(players_file1[i].first_name), players_file1[i].first_name );

               return bl_False;
          }

          int pos = nibble( players_file1[i].position[0], n_High );

          if ( pos != fpos_Pitcher )
          {
               players_file1[i].filestats.filebatting.ratings[2] = matchingPlayer->filestats.filebatting.ratings[2];
          }
     }

     return bl_True;
}
