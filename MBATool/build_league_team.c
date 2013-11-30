#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

league_team_s *buildLeagueTeam( const int league_id, const int team_id )
{
     league_team_s *league_team = NULL;

     if ( (league_team = malloc( sizeof(league_team_s) )) == NULL ) return NULL;

     memset( league_team, '\0', sizeof(league_team_s) );

     buildIntoLeagueTeam( league_team, league_id, team_id );

     return league_team;
}

void buildIntoLeagueTeam( league_team_s *league_team, const int league_id, const int team_id )
{
     league_team->league_id = league_id;
     league_team->team_id   = team_id;
}
