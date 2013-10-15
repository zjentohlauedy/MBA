#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "services.h"


static void free_batter( batter_s *batter )
{
     if ( batter == NULL ) return;

     if ( batter->stats     != NULL ) free( batter->stats     );
     if ( batter->accolades != NULL ) free( batter->accolades );

     free( batter );
}

static void free_pitcher( pitcher_s *pitcher )
{
     if ( pitcher == NULL ) return;

     if ( pitcher->stats     != NULL ) free( pitcher->stats     );
     if ( pitcher->accolades != NULL ) free( pitcher->accolades );

     free( pitcher );
}

void free_player( player_s *player )
{
     if ( player->player_type == pt_Batter  ) free_batter(  player->details.batting  );
     if ( player->player_type == pt_Pitcher ) free_pitcher( player->details.pitching );

     if ( player->accolades != NULL ) free( player->accolades );

     free( player );
}
