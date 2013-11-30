#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_s *buildTeam( const int team_id )
{
     team_s *team = NULL;

     if ( (team = malloc( sizeof(team_s) )) == NULL ) return NULL;

     memset( team, '\0', sizeof(team_s) );

     buildIntoTeam( team, team_id );

     return team;
}

void buildIntoTeam( team_s *team, const int team_id )
{
     team->team_id = team_id;
     team->primary_color = (rand() % 8) + 1;
     team->secondary_color = (rand() % 8) + 1;

     sprintf( team->name,     "Team%d", team_id );
     sprintf( team->location, "City%d", team_id );
}
