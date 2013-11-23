#include "compares.h"
#include "unit_test.h"

char *_compareTeamBattingStats( team_batting_stats_s *expected, team_batting_stats_s *actual )
{
     assertEqualsInt( expected->team_id,         actual->team_id        );
     assertEqualsInt( expected->season,          actual->season         );
     assertEqualsInt( expected->season_phase,    actual->season_phase   );
     assertEqualsInt( expected->games,           actual->games          );
     assertEqualsInt( expected->at_bats,         actual->at_bats        );
     assertEqualsInt( expected->runs,            actual->runs           );
     assertEqualsInt( expected->hits,            actual->hits           );
     assertEqualsInt( expected->doubles,         actual->doubles        );
     assertEqualsInt( expected->triples,         actual->triples        );
     assertEqualsInt( expected->home_runs,       actual->home_runs      );
     assertEqualsInt( expected->runs_batted_in,  actual->runs_batted_in );
     assertEqualsInt( expected->walks,           actual->walks          );
     assertEqualsInt( expected->strike_outs,     actual->strike_outs    );
     assertEqualsInt( expected->steals,          actual->steals         );
     assertEqualsInt( expected->errors,          actual->errors         );

     return NULL;
}
