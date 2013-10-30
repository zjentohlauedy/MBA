#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

static team_s *createTeam( const int team_id, const char *name, const char *location, const int color )
{
     team_s *team = NULL;

     if ( (team = malloc( sizeof(team_s) )) == NULL ) return NULL;

     memset( team, '\0', sizeof(team_s) );

     strcpy( team->name,             name     );
     strcpy( team->location,         location );
     /**/    team->team_id         = team_id;
     /**/    team->primary_color   = color;
     /**/    team->secondary_color = color;

     return team;
}

static void freeTeams( team_s *teams[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( teams[i] != NULL ) free_team( teams[i] );
     }
}

static boolean_e addTeamToList( data_list_s *list, const int division_id, team_s *team )
{
     division_team_s division_team = { 0 };

     division_team.division_id = division_id;
     division_team.team_id     = team->team_id;
     division_team.team        = team;

     if ( add_to_data_list( list, &division_team, sizeof(division_team_s), 10 ) < 0 ) return bl_False;

     return bl_True;
}

division_team_s *convertTeams( const org_data_s *org_data, const int division_id )
{
     data_list_s      list                      = { 0 };
     division_team_s  sentinel                  = DIVISION_TEAM_SENTINEL;
     team_s          *teams[TEAMS_PER_DIVISION] = { 0 };

     fileleagname_s *league_data = org_data->league_data;
     fileparks_s    *parks_data  = org_data->parks_data;

     int idx = (division_id - 1) * TEAMS_PER_DIVISION;

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          int team_id = idx + i + 1;

          if ( (teams[i] = createTeam( team_id, league_data->teams[idx + i].name, parks_data->park_names[idx + i].text, league_data->teams[idx + i].color[0] )) == NULL )
          {
               freeTeams( teams, TEAMS_PER_DIVISION );

               return NULL;
          }

          if ( (teams[i]->players = convertPlayers( org_data, team_id )) == NULL )
          {
               freeTeams( teams, TEAMS_PER_DIVISION );

               return NULL;
          }

          if ( addTeamToList( &list, division_id, teams[i] ) != bl_True )
          {
               freeTeams( teams, TEAMS_PER_DIVISION );

               return NULL;
          }
     }

     add_to_data_list( &list, &sentinel, sizeof(division_team_s), 10 );

     return list.data;
}
