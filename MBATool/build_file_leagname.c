#include <stdio.h>
#include "builders.h"
#include "file_formats.h"

fileleagname_s *buildFileLeagName( void )
{
     static fileleagname_s league_data = { 0 };

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          sprintf( league_data.leagues[i].name, "League%d", i + 1 );
     }

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          sprintf( league_data.divisions[i].name, "Division%d", i + 1 );
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          sprintf(  league_data.teams[i].name,    "Team%d", i + 1 );
          int2byte( league_data.teams[i].stadium,           i     );
          int2byte( league_data.teams[i].color,             i + 1 );
          int2byte( league_data.teams[i].wins,              i     );
          int2byte( league_data.teams[i].losses,            i     );
          int2byte( league_data.teams[i].team_id,           i + 1 );
     }

     return &league_data;
}
