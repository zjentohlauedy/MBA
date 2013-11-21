#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"

schedule_s *buildSchedule( const fileleagname_s *league_data )
{
     schedule_s      *schedule = NULL;
     schedule_day_s  *days     = NULL;
     schedule_game_s *games    = NULL;

     if ( (schedule = malloc( sizeof(schedule_s) )) == NULL ) return NULL;

     if ( (days = malloc( sizeof(schedule_day_s) * TOTAL_TEAMS )) == NULL )
     {
          free( schedule );

          return NULL;
     }

     memset( schedule, '\0', sizeof(schedule_s)                   );
     memset( days,     '\0', sizeof(schedule_day_s) * TOTAL_TEAMS );

     schedule->days = days;

     struct schedule_teams { char name[20]; int idx; } teams[32] = { 0 };

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          strcpy( teams[i].name, league_data->teams[i].name );

          teams[i].idx = i;
     }

     for ( int i = 0; i < TOTAL_TEAMS - 1; ++i )
     {
          if ( (games = malloc( sizeof(schedule_game_s) * ((TOTAL_TEAMS / 2) + 1) )) == NULL )
          {
               freeSchedule( schedule );

               return NULL;
          }

          memset( games, '\0', sizeof(schedule_game_s) * ((TOTAL_TEAMS / 2) + 1) );

          for ( int j = 0; j < TOTAL_TEAMS / 2; ++j )
          {
               int road  = j;
               int home  = (TOTAL_TEAMS - 1) - j;

               strcpy( games[j].road.name, teams[road].name );
               strcpy( games[j].home.name, teams[home].name );

               games[j].road.score = teams[road].idx;
               games[j].home.score = teams[home].idx;

               games[j].played = bl_True;
          }

          games[TOTAL_TEAMS / 2].played = -1;

          days[i].games = games;

          // rotate teams
          struct schedule_teams x = teams[TOTAL_TEAMS - 2];

          for ( int k = TOTAL_TEAMS - 2; k > 0; --k )
          {
               teams[k] = teams[k - 1];
          }

          teams[0] = x;
     }

     return schedule;
}
