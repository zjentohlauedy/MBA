#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_list.h"
#include "file_formats.h"
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

static league_s *createLeague( const int league_id, const char *name )
{
     league_s *league = NULL;

     if ( (league = malloc( sizeof(league_s) )) == NULL ) return NULL;

     memset( league, '\0', sizeof(league_s) );

     /**/    league->league_id = league_id;
     strcpy( league->name,       name      );

     return league;
}

static org_s *createOrg( void )
{
     org_s *org = NULL;

     if ( (org = malloc( sizeof(org_s) )) == NULL ) return NULL;

     memset( org, '\0', sizeof(org_s) );

     return org;
}

static league_division_s *convertDivisions( const fileleague_s *league_data, const int league_id )
{
     data_list_s        list            = { 0 };
     league_division_s  league_division = { 0 };
     league_division_s  sentinel        = LEAGUE_DIVISION_SENTINEL;
     division_s        *divisions[2]    = { 0 };

     if ( league_id == 1 )
     {
          if ( (divisions[0] = createDivision( 1, league_data->division1 )) == NULL ) {                       return NULL; }
          if ( (divisions[1] = createDivision( 2, league_data->division2 )) == NULL ) { free( divisions[0] ); return NULL; }
     }
     else
     {
          if ( (divisions[0] = createDivision( 3, league_data->division3 )) == NULL ) {                       return NULL; }
          if ( (divisions[1] = createDivision( 4, league_data->division4 )) == NULL ) { free( divisions[0] ); return NULL; }
     }

     league_division.league_id   =               league_id;
     league_division.division_id = divisions[0]->division_id;
     league_division.division    = divisions[0];

     if ( add_to_data_list( &list, &league_division, sizeof(league_division_s), 5 ) < 0 )
     {
          free_division( divisions[0] );
          free_division( divisions[1] );

          return NULL;
     }

     league_division.league_id   =               league_id;
     league_division.division_id = divisions[1]->division_id;
     league_division.division    = divisions[1];

     add_to_data_list( &list, &league_division, sizeof(league_division_s), 5 );
     add_to_data_list( &list, &sentinel,        sizeof(league_division_s), 5 );

     return list.data;
}

static org_league_s *convertLeagues( const fileleague_s *league_data )
{
     data_list_s   list       = { 0 };
     org_league_s  org_league = { 0 };
     league_s     *leagues[2] = { 0 };

     if ( (leagues[0] = createLeague( 1, league_data->league1 )) == NULL ) {                     return NULL; }
     if ( (leagues[1] = createLeague( 2, league_data->league2 )) == NULL ) { free( leagues[0] ); return NULL; }

     if ( (leagues[0]->divisions = convertDivisions( league_data, leagues[0]->league_id )) == NULL )
     {
          free_league( leagues[0] );
          free_league( leagues[1] );

          return NULL;
     }

     if ( (leagues[1]->divisions = convertDivisions( league_data, leagues[1]->league_id )) == NULL )
     {
          free_league( leagues[0] );
          free_league( leagues[1] );

          return NULL;
     }

     org_league.league = leagues[0];

     if ( add_to_data_list( &list, &org_league, sizeof(org_league_s), 5 ) < 0 )
     {
          free_league( leagues[0] );
          free_league( leagues[1] );

          return NULL;
     }

     org_league.league = leagues[1];

     add_to_data_list( &list, &org_league, sizeof(org_league_s), 5 );

     org_league.league = NULL;

     add_to_data_list( &list, &org_league, sizeof(org_league_s), 5 );

     return list.data;
}

org_s *convertOrg( const fileleague_s *league_data )
{
     org_s *org = NULL;

     if ( (org = createOrg()) == NULL ) return NULL;

     if ( (org->leagues = convertLeagues( league_data )) == NULL )
     {
          free( org );

          return NULL;
     }

     return org;
}

static void freeLeagues( org_league_s *leagues )
{
     for ( int i = 0; leagues[i].league != NULL; ++i )
     {
          free_league( leagues[i].league );
     }

     free( leagues );
}

void freeOrg( org_s *org )
{
     if ( org->leagues != NULL )
     {
          freeLeagues( org->leagues );
     }

     free( org );
}
