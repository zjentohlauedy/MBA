#include "compares.h"
#include "unit_test.h"

char *_compareTeamVersusStats( team_versus_stats_s *expected, team_versus_stats_s *actual )
{
     assertEqualsInt( expected->team_id,         actual->team_id         );
     assertEqualsInt( expected->season,          actual->season          );
     assertEqualsInt( expected->season_phase,    actual->season_phase    );
     assertEqualsInt( expected->opponent,        actual->opponent        );
     assertEqualsInt( expected->wins,            actual->wins            );
     assertEqualsInt( expected->losses,          actual->losses          );
     assertEqualsInt( expected->runs_scored,     actual->runs_scored     );
     assertEqualsInt( expected->runs_allowed,    actual->runs_allowed    );

     return NULL;
}
