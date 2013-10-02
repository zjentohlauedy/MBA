#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "services.h"


void free_player( player_s *player )
{
     if ( player->player_type == pt_Batter  &&  player->details.batting != NULL )
     {
          if ( player->details.batting->stats != NULL ) free( player->details.batting->stats );

          player->details.batting->stats = NULL;

          if ( player->details.batting->accolades != NULL ) free( player->details.batting->accolades );

          player->details.batting->accolades = NULL;

          free( player->details.batting );

          player->details.batting = NULL;
     }

     if ( player->player_type == pt_Pitcher  &&   player->details.pitching != NULL )
     {
          if ( player->details.pitching->stats != NULL ) free( player->details.pitching->stats );

          player->details.pitching->stats = NULL;

          if ( player->details.pitching->accolades != NULL ) free( player->details.pitching->accolades );

          player->details.pitching->accolades = NULL;

          free( player->details.pitching );

          player->details.pitching = NULL;
     }

     if ( player->accolades != NULL ) free( player->accolades );

     player->accolades = NULL;

     free( player );
}
