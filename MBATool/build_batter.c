#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

batter_s *buildBatter( const int player_id )
{
     batter_s *batter = NULL;

     if ( (batter = malloc( sizeof(batter_s) )) == NULL ) return NULL;

     memset( batter, '\0', sizeof(batter_s) );

     buildIntoBatter( batter, player_id );

     return batter;
}

void buildIntoBatter( batter_s *batter, const int player_id )
{
     batter->player_id          = player_id;
     batter->power              = (rand() % 10) + 1;
     batter->hit_n_run          = (rand() % 10) + 1;
     batter->bunt               = (rand() % 10) + 1;
     batter->running            = (rand() % 10) + 1;
     batter->range              = (rand() % 10) + 1;
     batter->arm                = (rand() % 10) + 1;
     batter->primary_position   = (rand() %  8) + 1;
     batter->secondary_position = (rand() % 10) + 1;
}
