#include "compares.h"
#include "unit_test.h"

char *_compareTeamPitchingStats( team_pitching_stats_s *expected, team_pitching_stats_s *actual )
{
     assertEqualsInt( expected->team_id,         actual->team_id         );
     assertEqualsInt( expected->season,          actual->season          );
     assertEqualsInt( expected->season_phase,    actual->season_phase    );
     assertEqualsInt( expected->wins,            actual->wins            );
     assertEqualsInt( expected->losses,          actual->losses          );
     assertEqualsInt( expected->games,           actual->games           );
     assertEqualsInt( expected->saves,           actual->saves           );
     assertEqualsInt( expected->innings.innings, actual->innings.innings );
     assertEqualsInt( expected->innings.outs,    actual->innings.outs    );
     assertEqualsInt( expected->hits,            actual->hits            );
     assertEqualsInt( expected->earned_runs,     actual->earned_runs     );
     assertEqualsInt( expected->home_runs,       actual->home_runs       );
     assertEqualsInt( expected->walks,           actual->walks           );
     assertEqualsInt( expected->strike_outs,     actual->strike_outs     );

     return NULL;
}
