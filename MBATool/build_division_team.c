#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

division_team_s *buildDivisionTeam( const int division_id, const int team_id )
{
     division_team_s *division_team = NULL;

     if ( (division_team = malloc( sizeof(division_team_s) )) == NULL ) return NULL;

     memset( division_team, '\0', sizeof(division_team_s) );

     buildIntoDivisionTeam( division_team, division_id, team_id );

     return division_team;
}

void buildIntoDivisionTeam( division_team_s *division_team, const int division_id, const int team_id )
{
     division_team->division_id = division_id;
     division_team->team_id     = team_id;
}
