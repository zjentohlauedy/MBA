#include <stdio.h>
#include <stdlib.h>
#include "builders.h"
#include "team.h"


team_versus_stats_s *buildTeamVersusStats( const int team_id, const int season, const season_phase_e season_phase, const int opponent )
{
     team_versus_stats_s *team_versus_stats = NULL;

     if ( (team_versus_stats = malloc( sizeof(team_versus_stats_s) )) == NULL ) return NULL;

     buildIntoTeamVersusStats( team_versus_stats, team_id, season, season_phase, opponent );

     return team_versus_stats;
}

void buildIntoTeamVersusStats( team_versus_stats_s *team_versus_stats, const int team_id, const int season, const season_phase_e season_phase, const int opponent )
{
     team_versus_stats->team_id         = team_id;
     team_versus_stats->season          = season;
     team_versus_stats->season_phase    = season_phase;
     team_versus_stats->opponent        = opponent;

     team_versus_stats->wins            = rand() %   5;
     team_versus_stats->losses          = rand() %   5;
     team_versus_stats->runs_scored     = rand() %  50;
     team_versus_stats->runs_allowed    = rand() %  50;
}
