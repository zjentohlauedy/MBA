#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

pitcher_s *buildPitcher( const int player_id )
{
     pitcher_s *pitcher = NULL;

     if ( (pitcher = malloc( sizeof(pitcher_s) )) == NULL ) return NULL;

     memset( pitcher, '\0', sizeof(pitcher_s) );

     buildIntoPitcher( pitcher, player_id );

     return pitcher;
}

void buildIntoPitcher( pitcher_s *pitcher, const int player_id )
{
     pitcher->player_id = player_id;
     pitcher->speed     = (rand() % 10) + 1;
     pitcher->control   = (rand() % 10) + 1;
     pitcher->bunt      = (rand() % 10) + 1;
     pitcher->fatigue   = (rand() %  7) + 4;
}
