#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

static division_s *createDivision( const int division_id, const char *name )
{
     division_s *division = NULL;

     if ( (division = malloc( sizeof(division_s) )) == NULL ) return NULL;

     memset( division, '\0', sizeof(division_s) );

     /**/    division->division_id = division_id;
     strcpy( division->name,         name        );

     return division;
}

static void freeDivisions( division_s *divisions[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( divisions[i] != NULL ) free_division( divisions[i] );
     }
}

static boolean_e addDivisionToList( data_list_s *list, const int league_id, division_s *division )
{
     league_division_s league_division = { 0 };

     league_division.league_id   = league_id;
     league_division.division_id = division->division_id;
     league_division.division    = division;

     if ( add_to_data_list( list, &league_division, sizeof(league_division_s), 5 ) < 0 ) return bl_False;

     return bl_True;
}

static division_stats_s *convertDivisionStats( division_stats_s *division_stats )
{
     data_list_s      list     = { 0 };
     division_stats_s sentinel = DIVISION_STATS_SENTINEL;

     if ( add_to_data_list( &list,  division_stats, sizeof(division_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,       sizeof(division_stats_s), 5 );

     return list.data;
}

league_division_s *convertDivisions( const org_data_s *org_data, const int league_id )
{
     data_list_s        list                            = { 0 };
     league_division_s  sentinel                        = LEAGUE_DIVISION_SENTINEL;
     division_s        *divisions[DIVISIONS_PER_LEAGUE] = { 0 };


     int idx = (league_id - 1) * DIVISIONS_PER_LEAGUE;

     for ( int i = 0; i < DIVISIONS_PER_LEAGUE; ++i )
     {
          int division_id = idx + i + 1;

          if ( (divisions[i] = createDivision( division_id, org_data->league_data->divisions[idx + i].name )) == NULL )
          {
               freeDivisions( divisions, DIVISIONS_PER_LEAGUE );

               return NULL;
          }

          if ( (divisions[i]->teams = convertTeams( org_data, division_id )) == NULL )
          {
               freeDivisions( divisions, DIVISIONS_PER_LEAGUE );

               return NULL;
          }

          if ( (divisions[i]->stats = convertDivisionStats( &(org_data->records->divisions[i]) )) == NULL )
          {
               freeDivisions( divisions, DIVISIONS_PER_LEAGUE );

               return NULL;
          }

          if ( addDivisionToList( &list, league_id, divisions[i] ) != bl_True )
          {
               freeDivisions( divisions, DIVISIONS_PER_LEAGUE );

               return NULL;
          }
     }

     add_to_data_list( &list, &sentinel, sizeof(league_division_s), 5 );

     return list.data;
}
