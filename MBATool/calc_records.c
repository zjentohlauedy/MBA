#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

struct league   { char name[20];                           };
struct division { char name[20]; int league;               };
struct team     { char name[20]; int league; int division; };


static char error_message[999 + 1] = { 0 };


char *getCalculateRecordsError( void )
{
     return error_message;
}


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


static boolean_e isAllstarTeam( struct league *leagues, char *team )
{
     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          if ( strcmp( leagues[i].name, team ) == 0 ) return bl_True;
     }

     return bl_False;
}


static int findTeam( struct team *teams, char *team )
{
     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          if ( strcmp( teams[i].name, team ) == 0 ) return i;
     }

     return -1;
}


records_s *calculateRecords( const schedule_s *schedule, const fileleagname_s *league_file, const int season, const season_phase_e season_phase )
{
     struct league   leagues   [ TOTAL_LEAGUES   ];
     struct division divisions [ TOTAL_DIVISIONS ];
     struct team     teams     [ TOTAL_TEAMS     ];


     clearErrorMessage();

     if ( schedule->days == NULL )
     {
          sprintf( error_message, "Schedule does not contain any days." );

          return NULL;
     }

     records_s *records = NULL;

     if ( (records = calloc( 1, sizeof(records_s) )) == NULL ) return NULL;

     if ( (records->leagues = calloc( TOTAL_LEAGUES, sizeof(league_stats_s) )) == NULL )
     {
          sprintf( error_message, "Unable to allocate memory for league records" );

          freeRecords( records );

          return NULL;
     }

     if ( (records->divisions = calloc( TOTAL_DIVISIONS, sizeof(division_stats_s) )) == NULL )
     {
          sprintf( error_message, "Unable to allocate memory for division records" );

          freeRecords( records );

          return NULL;
     }

     if ( (records->teams = calloc( TOTAL_TEAMS, sizeof(team_stats_s) )) == NULL )
     {
          sprintf( error_message, "Unable to allocate memory for team records" );

          freeRecords( records );

          return NULL;
     }

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          strcpy( leagues[i].name, league_file->leagues[i].name );

          records->leagues[i].league_id    = i + 1;
          records->leagues[i].season       = season;
          records->leagues[i].season_phase = season_phase;
     }

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          strcpy( divisions[i].name, league_file->divisions[i].name );

          divisions[i].league = i / DIVISIONS_PER_LEAGUE;

          records->divisions[i].division_id  = i + 1;
          records->divisions[i].season       = season;
          records->divisions[i].season_phase = season_phase;
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          strcpy( teams[i].name, league_file->teams[i].name );

          teams[i].league   = i / TEAMS_PER_LEAGUE;
          teams[i].division = i / TEAMS_PER_DIVISION;

          records->teams[i].team_id      = byte2int( league_file->teams[i].team_id );
          records->teams[i].season       = season;
          records->teams[i].season_phase = season_phase;
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          if ( (records->versus[i] = calloc( TOTAL_TEAMS, sizeof(team_versus_stats_s) )) == NULL )
          {
               sprintf( error_message, "Unable to allocate memory for team versus records" );

               freeRecords( records );

               return NULL;
          }

          for ( int j = 0; j < TOTAL_TEAMS; ++j )
          {
               records->versus[i][j].team_id      = byte2int( league_file->teams[i].team_id );
               records->versus[i][j].season       = season;
               records->versus[i][j].season_phase = season_phase;
               records->versus[i][j].opponent     = byte2int( league_file->teams[j].team_id );
          }
     }

     for ( int i = 0; schedule->days[i].games != NULL; ++i )
     {
          schedule_day_s *day = &schedule->days[i];

          if ( season_phase == sp_Regular  &&  i >= REGULAR_SEASON_DAYS ) continue;
          if ( season_phase == sp_Playoff  &&  i <  REGULAR_SEASON_DAYS ) continue;

          for ( int j = 0; day->games[j].played != -1; ++j )
          {
               schedule_game_s *game = &day->games[j];

               if   ( isAllstarTeam( leagues, game->road.name ) ) { if ( season_phase != sp_Allstar ) break; }
               else                                               { if ( season_phase == sp_Allstar ) break; }

               int road_idx;
               int home_idx;

               if ( (road_idx = findTeam( teams, game->road.name )) < 0 )
               {
                    sprintf( error_message, "Road team <%s> not found!", game->road.name );

                    freeRecords( records );

                    return NULL;
               }

               if ( (home_idx = findTeam( teams, game->home.name )) < 0 )
               {
                    sprintf( error_message, "Home team <%s> not found!", game->road.name );

                    freeRecords( records );

                    return NULL;
               }

               if ( game->road.score > game->home.score )
               {
                    records->teams[road_idx].wins++;
                    records->teams[home_idx].losses++;

                    records->teams[road_idx].road_wins++;
                    records->teams[home_idx].home_losses++;

                    records->versus[road_idx][home_idx].wins++;
                    records->versus[home_idx][road_idx].losses++;

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

                    records->versus[home_idx][road_idx].wins++;
                    records->versus[road_idx][home_idx].losses++;

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

               records->versus[road_idx][home_idx].runs_scored  += game->road.score;
               records->versus[home_idx][road_idx].runs_scored  += game->home.score;

               records->versus[road_idx][home_idx].runs_allowed += game->home.score;
               records->versus[home_idx][road_idx].runs_allowed += game->road.score;

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

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          if ( records->versus[i] != NULL ) free( records->versus[i] );
     }

     free( records );
}
