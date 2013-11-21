#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

struct league   { char name[20];                           };
struct division { char name[20]; int league;               };
struct team     { char name[20]; int league; int division; };


static int findTeam( struct team *teams, char *team )
{
     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          if ( strcmp( teams[i].name, team ) == 0 ) return i;
     }

     return -1;
}

records_s *calculateRecords( const schedule_s *schedule, const fileleagname_s *league_file )
{
     struct league   leagues   [ TOTAL_LEAGUES   ];
     struct division divisions [ TOTAL_DIVISIONS ];
     struct team     teams     [ TOTAL_TEAMS     ];

     if ( schedule->days == NULL ) return NULL;

     records_s *records = NULL;

     if ( (records = malloc( sizeof(records_s) )) == NULL ) return NULL;

     if ( (records->leagues = malloc( sizeof(league_stats_s) * TOTAL_LEAGUES )) == NULL )
     {
          freeRecords( records );

          return NULL;
     }

     if ( (records->divisions = malloc( sizeof(division_stats_s) * TOTAL_DIVISIONS )) == NULL )
     {
          freeRecords( records );

          return NULL;
     }

     if ( (records->teams = malloc( sizeof(team_stats_s) * TOTAL_TEAMS )) == NULL )
     {
          freeRecords( records );

          return NULL;
     }

     memset( records->leagues,   '\0', sizeof(league_stats_s)   * TOTAL_LEAGUES   );
     memset( records->divisions, '\0', sizeof(division_stats_s) * TOTAL_DIVISIONS );
     memset( records->teams,     '\0', sizeof(team_stats_s)     * TOTAL_TEAMS     );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          strcpy( leagues[i].name, league_file->leagues[i].name );
     }

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          strcpy( divisions[i].name, league_file->divisions[i].name );

          divisions[i].league = i / DIVISIONS_PER_LEAGUE;
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          strcpy( teams[i].name, league_file->teams[i].name );

          teams[i].league   = i / TEAMS_PER_LEAGUE;
          teams[i].division = i / TEAMS_PER_DIVISION;
     }

     for ( int i = 0; schedule->days[i].games != NULL; ++i )
     {
          schedule_day_s *day = &schedule->days[i];

          for ( int j = 0; day->games[j].played != -1; ++j )
          {
               schedule_game_s *game = &day->games[j];

               int road_idx;
               int home_idx;

               if ( (road_idx = findTeam( teams, game->road.name )) < 0 )
               {
                    printf( "Team <%s> not found!\n", game->road.name );

                    freeRecords( records );

                    return NULL;
               }

               if ( (home_idx = findTeam( teams, game->home.name )) < 0 )
               {
                    printf( "Team <%s> not found!\n", game->home.name );

                    freeRecords( records );

                    return NULL;
               }

               if ( game->road.score > game->home.score )
               {
                    records->teams[road_idx].wins++;
                    records->teams[home_idx].losses++;

                    records->teams[road_idx].road_wins++;
                    records->teams[home_idx].home_losses++;

                    if ( teams[road_idx].division == teams[home_idx].division )
                    {
                         records->teams[road_idx].division_wins++;
                         records->teams[home_idx].division_losses++;
                    }
                    else
                    {
                         records->divisions[teams[road_idx].division].wins++;
                         records->divisions[teams[home_idx].division].losses++;

                         records->divisions[teams[road_idx].division].road_wins++;
                         records->divisions[teams[home_idx].division].home_losses++;

                         if ( teams[road_idx].league == teams[home_idx].league )
                         {
                              records->divisions[teams[road_idx].division].league_wins++;
                              records->divisions[teams[home_idx].division].league_losses++;
                         }
                         else
                         {
                              records->leagues[teams[road_idx].league].wins++;
                              records->leagues[teams[home_idx].league].losses++;

                              records->leagues[teams[road_idx].league].road_wins++;
                              records->leagues[teams[home_idx].league].home_losses++;
                         }
                    }

                    if ( teams[road_idx].league == teams[home_idx].league )
                    {
                         records->teams[road_idx].league_wins++;
                         records->teams[home_idx].league_losses++;
                    }
               }
               else
               {
                    records->teams[home_idx].wins++;
                    records->teams[road_idx].losses++;

                    records->teams[home_idx].home_wins++;
                    records->teams[road_idx].road_losses++;

                    if ( teams[road_idx].division == teams[home_idx].division )
                    {
                         records->teams[home_idx].division_wins++;
                         records->teams[road_idx].division_losses++;
                    }
                    else
                    {
                         records->divisions[teams[home_idx].division].wins++;
                         records->divisions[teams[road_idx].division].losses++;

                         records->divisions[teams[home_idx].division].home_wins++;
                         records->divisions[teams[road_idx].division].road_losses++;

                         if ( teams[road_idx].league == teams[home_idx].league )
                         {
                              records->divisions[teams[home_idx].division].league_wins++;
                              records->divisions[teams[road_idx].division].league_losses++;
                         }
                         else
                         {
                              records->leagues[teams[home_idx].league].wins++;
                              records->leagues[teams[road_idx].league].losses++;

                              records->leagues[teams[home_idx].league].home_wins++;
                              records->leagues[teams[road_idx].league].road_losses++;
                         }
                    }

                    if ( teams[road_idx].league == teams[home_idx].league )
                    {
                         records->teams[home_idx].league_wins++;
                         records->teams[road_idx].league_losses++;
                    }
               }

               records->teams[road_idx].runs_scored += game->road.score;
               records->teams[home_idx].runs_scored += game->home.score;

               records->teams[road_idx].runs_allowed += game->home.score;
               records->teams[home_idx].runs_allowed += game->road.score;

               if ( teams[road_idx].division != teams[home_idx].division )
               {
                    records->divisions[teams[road_idx].division].runs_scored += game->road.score;
                    records->divisions[teams[home_idx].division].runs_scored += game->home.score;

                    records->divisions[teams[road_idx].division].runs_allowed += game->home.score;
                    records->divisions[teams[home_idx].division].runs_allowed += game->road.score;
               }

               if ( teams[road_idx].league != teams[home_idx].league )
               {
                    records->leagues[teams[road_idx].league].runs_scored += game->road.score;
                    records->leagues[teams[home_idx].league].runs_scored += game->home.score;

                    records->leagues[teams[road_idx].league].runs_allowed += game->home.score;
                    records->leagues[teams[home_idx].league].runs_allowed += game->road.score;
               }
          }
     }

     return records;
}

void freeRecords( records_s *records )
{
     if ( records->leagues   != NULL ) free( records->leagues   );
     if ( records->divisions != NULL ) free( records->divisions );
     if ( records->teams     != NULL ) free( records->teams     );

     free( records );
}
