#include <stdio.h>
#include <stdlib.h>
#include "builders.h"
#include "team.h"


team_pitching_stats_s *buildTeamPitchingStats( const int team_id, const int season, const season_phase_e season_phase )
{
     team_pitching_stats_s *team_pitching_stats = NULL;

     if ( (team_pitching_stats = malloc( sizeof(team_pitching_stats_s) )) == NULL ) return NULL;

     team_pitching_stats->team_id         = team_id;
     team_pitching_stats->season          = season;
     team_pitching_stats->season_phase    = season_phase;

     team_pitching_stats->wins            = rand() %  50;
     team_pitching_stats->losses          = rand() %  50;
     team_pitching_stats->games           = rand() %  50;
     team_pitching_stats->saves           = rand() %  50;
     team_pitching_stats->innings.innings = rand() % 300;
     team_pitching_stats->innings.outs    = rand() %   3;
     team_pitching_stats->hits            = rand() % 300;
     team_pitching_stats->earned_runs     = rand() % 100;
     team_pitching_stats->home_runs       = rand() %  50;
     team_pitching_stats->walks           = rand() %  50;
     team_pitching_stats->strike_outs     = rand() % 500;

     return team_pitching_stats;
}

void buildIntoTeamPitchingStats( team_pitching_stats_s *team_pitching_stats, const int team_id, const int season, const season_phase_e season_phase )
{
     team_pitching_stats->team_id         = team_id;
     team_pitching_stats->season          = season;
     team_pitching_stats->season_phase    = season_phase;

     team_pitching_stats->wins            = rand() %  50;
     team_pitching_stats->losses          = rand() %  50;
     team_pitching_stats->games           = rand() %  50;
     team_pitching_stats->saves           = rand() %  50;
     team_pitching_stats->innings.innings = rand() % 300;
     team_pitching_stats->innings.outs    = rand() %   3;
     team_pitching_stats->hits            = rand() % 300;
     team_pitching_stats->earned_runs     = rand() % 100;
     team_pitching_stats->home_runs       = rand() %  50;
     team_pitching_stats->walks           = rand() %  50;
     team_pitching_stats->strike_outs     = rand() % 500;
}
