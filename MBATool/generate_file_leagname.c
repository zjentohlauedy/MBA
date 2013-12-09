#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"
#include "org.h"

fileleagname_s *generateFileLeagName( org_s *org )
{
     fileleagname_s *league_file = NULL;

     if ( (league_file = malloc( sizeof(fileleagname_s) )) == NULL ) return NULL;

     memset( league_file, '\0', sizeof(fileleagname_s) );

     if ( org->leagues == NULL ) return league_file;

     for ( int i = 0; org->leagues[i].league != NULL; ++i )
     {
          league_s *league = org->leagues[i].league;

          strcpy( league_file->leagues[i].name, league->name );

          for ( int j = 0; league->divisions[j].division != NULL; ++j )
          {
               division_s *division = league->divisions[j].division;

               int div_idx = (i * DIVISIONS_PER_LEAGUE) + j;

               strcpy( league_file->divisions[div_idx].name, division->name );

               for ( int k = 0; division->teams[k].team != NULL; ++k )
               {
                    int team_idx = (i * TEAMS_PER_LEAGUE) + (j * TEAMS_PER_DIVISION) + k;

                    strcpy(   league_file->teams[team_idx].name,    division->teams[k].team->name          );
                    int2byte( league_file->teams[team_idx].color,   division->teams[k].team->primary_color );
                    int2byte( league_file->teams[team_idx].team_id, division->teams[k].team->team_id       );
                    int2byte( league_file->teams[team_idx].stadium,                          team_idx      );
               }
          }
     }

     return league_file;
}
