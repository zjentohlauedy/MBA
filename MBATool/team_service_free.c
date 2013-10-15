#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"


static void free_team_players( team_player_s *team_players )
{
     for ( int i = 0; team_players[i].team_id >= 0; ++i )
     {
          if ( team_players[i].player != NULL ) free_player( team_players[i].player );
     }

     free( team_players );
}

void free_team( team_s *team )
{
     if ( team->players        != NULL ) free( team->players        );
     if ( team->stats          != NULL ) free( team->stats          );
     if ( team->pitching_stats != NULL ) free( team->pitching_stats );
     if ( team->batting_stats  != NULL ) free( team->batting_stats  );
     if ( team->accolades      != NULL ) free( team->accolades      );

     free( team );
}
