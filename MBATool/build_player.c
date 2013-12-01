#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_s *buildPlayer( const int player_id )
{
     player_s *player = NULL;

     if ( (player = malloc( sizeof(player_s) )) == NULL ) return NULL;

     memset( player, '\0', sizeof(player_s) );

     buildIntoPlayer( player, player_id );

     return player;
}

void buildIntoPlayer( player_s *player, const int player_id )
{
     player->player_id     = player_id;
     player->skin_tone     = (rand() %  2) + 1;
     player->handedness    = (rand() %  2) + 1;
     player->player_type   = (rand() %  2) + 1;
     player->rookie_season = (rand() %  5) + 1;
     player->longevity     = (rand() % 10) + 1;

     sprintf( player->first_name,      "Fname%d",  player_id );
     sprintf( player->last_name,       "Lname%d",  player_id );
     sprintf( player->first_phoenetic, "Fphoen%d", player_id );
     sprintf( player->last_phoenetic,  "Lphoen%d", player_id );
}
