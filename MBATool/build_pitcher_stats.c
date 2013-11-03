#include <stdio.h>
#include <stdlib.h>
#include "builders.h"
#include "player.h"

pitcher_stats_s *buildPitcherStats( const int player_id, const int season, const season_phase_e season_phase )
{
     pitcher_stats_s *pitcher_stats = NULL;

     if ( (pitcher_stats = malloc( sizeof(pitcher_stats_s) )) == NULL ) return NULL;

     pitcher_stats->player_id       = player_id;
     pitcher_stats->season          = season;
     pitcher_stats->season_phase    = season_phase;

     pitcher_stats->wins            = rand() %  50;
     pitcher_stats->losses          = rand() %  50;
     pitcher_stats->games           = rand() %  50;
     pitcher_stats->saves           = rand() %  50;
     pitcher_stats->innings.innings = rand() % 300;
     pitcher_stats->innings.outs    = rand() %   3;
     pitcher_stats->hits            = rand() % 300;
     pitcher_stats->earned_runs     = rand() % 100;
     pitcher_stats->home_runs       = rand() %  50;
     pitcher_stats->walks           = rand() %  50;
     pitcher_stats->strike_outs     = rand() % 500;

     return pitcher_stats;
}

void buildIntoPitcherStats( pitcher_stats_s *pitcher_stats, const int player_id, const int season, const season_phase_e season_phase )
{
     pitcher_stats->player_id       = player_id;
     pitcher_stats->season          = season;
     pitcher_stats->season_phase    = season_phase;

     pitcher_stats->wins            = rand() %  50;
     pitcher_stats->losses          = rand() %  50;
     pitcher_stats->games           = rand() %  50;
     pitcher_stats->saves           = rand() %  50;
     pitcher_stats->innings.innings = rand() % 300;
     pitcher_stats->innings.outs    = rand() %   3;
     pitcher_stats->hits            = rand() % 300;
     pitcher_stats->earned_runs     = rand() % 100;
     pitcher_stats->home_runs       = rand() %  50;
     pitcher_stats->walks           = rand() %  50;
     pitcher_stats->strike_outs     = rand() % 500;
}
