#include <stdlib.h>
#include <time.h>
#include "file_formats.h"

void setLongevity( fileplayer_s *players_file )
{
     srand( time( NULL ) );

     for ( int player_id = 1, idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
          if ( players_file[idx].last_name[0] == '\0' ) continue;

          int pos = nibble( players_file[idx].position[0], n_High );

          acc_player_id_s *id_info = NULL;

          if ( pos != fpos_Pitcher )
          {
               struct batting_s *batting = &(players_file[idx].filestats.filebatting);

               unsigned char longevity = (rand() % 10) + 1;

               batting->ratings[2] += longevity;
          }
     }
}
