#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

static league_s *createLeague( const int league_id, const char *name )
{
     league_s *league = NULL;

     if ( (league = malloc( sizeof(league_s) )) == NULL ) return NULL;

     memset( league, '\0', sizeof(league_s) );

     /**/    league->league_id = league_id;
     strcpy( league->name,       name      );

     return league;
}

static void freeLeagues( league_s *leagues[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( leagues[i] != NULL ) free_league( leagues[i] );
     }
}

static boolean_e addLeagueToList( data_list_s *list, league_s *league )
{
     org_league_s org_league = { 0 };

     org_league.league = league;

     if ( add_to_data_list( list, &org_league, sizeof(org_league_s), 5 ) < 0 ) return bl_False;

     return bl_True;
}

static league_stats_s *convertLeagueStats( league_stats_s *league_stats )
{
     data_list_s    list     = { 0 };
     league_stats_s sentinel = LEAGUE_STATS_SENTINEL;

     if ( add_to_data_list( &list,  league_stats, sizeof(league_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,     sizeof(league_stats_s), 5 );

     return list.data;
}

org_league_s *convertLeagues( const org_data_s *org_data )
{
     data_list_s   list                   = { 0 };
     org_league_s  sentinel               = { 0 };
     league_s     *leagues[TOTAL_LEAGUES] = { 0 };


     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          int league_id = i + 1;

          if ( (leagues[i] = createLeague( league_id, org_data->league_data->leagues[i].name )) == NULL )
          {
               freeLeagues( leagues, TOTAL_LEAGUES );

               return NULL;
          }

          if ( (leagues[i]->divisions = convertDivisions( org_data, league_id )) == NULL )
          {
               freeLeagues( leagues, TOTAL_LEAGUES );

               return NULL;
          }

          if ( (leagues[i]->stats = convertLeagueStats( &(org_data->records->leagues[i]) )) == NULL )
          {
               freeLeagues( leagues, TOTAL_LEAGUES );

               return NULL;
          }

          if ( addLeagueToList( &list, leagues[i] ) != bl_True )
          {
               freeLeagues( leagues, TOTAL_LEAGUES );

               return NULL;
          }
     }

     add_to_data_list( &list, &sentinel, sizeof(org_league_s), 5 );

     return list.data;
}

void freeOrgLeagues( org_league_s *org_leagues )
{
     for ( int i = 0; org_leagues[i].league != NULL; ++i )
     {
          free_league( org_leagues[i].league );
     }

     free( org_leagues );
}
