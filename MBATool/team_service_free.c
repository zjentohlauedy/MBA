#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"


void free_team( team_s *team )
{
     if ( team->players        != NULL ) free( team->players        );
     if ( team->stats          != NULL ) free( team->stats          );
     if ( team->pitching_stats != NULL ) free( team->pitching_stats );
     if ( team->batting_stats  != NULL ) free( team->batting_stats  );
     if ( team->accolades      != NULL ) free( team->accolades      );

     free( team );
}
