#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_player_s *buildTeamPlayer( const int team_id, const int season, const int player_id )
{
     team_player_s *team_player = NULL;

     if ( (team_player = malloc( sizeof(team_player_s) )) == NULL ) return NULL;

     memset( team_player, '\0', sizeof(team_player_s) );

     buildIntoTeamPlayer( team_player, team_id, season, player_id );

     return team_player;
}

void buildIntoTeamPlayer( team_player_s *team_player, const int team_id, const int season, const int player_id )
{
     team_player->team_id   = team_id;
     team_player->season    = season;
     team_player->player_id = player_id;
}
