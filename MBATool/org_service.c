#include <stdio.h>
#include <stdlib.h>
#include "org.h"


static void getTeamPlayers( sqlite3 *db, team_player_s *team_players )
{
     for ( int i = 0; team_players[i].team_id > 0; ++i )
     {
          team_players[i].player = get_player( db, team_players[i].player_id );
     }
}

static void getDivisionTeams( sqlite3 *db, division_team_s *division_teams )
{
     for ( int i = 0; division_teams[i].division_id > 0; ++i )
     {
          division_teams[i].team = get_team( db, division_teams[i].team_id );

          if ( division_teams[i].team->players != NULL )
          {
               getTeamPlayers( db, division_teams[i].team->players );
          }
     }
}

static void getLeagueDivisions( sqlite3 *db, league_division_s *league_divisions )
{
     for ( int i = 0; league_divisions[i].league_id > 0; ++i )
     {
          league_divisions[i].division = get_division( db, league_divisions[i].division_id );

          if ( league_divisions[i].division->teams != NULL )
          {
               getDivisionTeams( db, league_divisions[i].division->teams );
          }
     }
}

static void getLeagueTeams( sqlite3 *db, league_team_s *league_teams )
{
     for ( int i = 0; league_teams[i].league_id > 0; ++i )
     {
          league_teams[i].team = get_team( db, league_teams[i].team_id );

          if ( league_teams[i].team->players != NULL )
          {
               getTeamPlayers( db, league_teams[i].team->players );
          }
     }
}

static void cleanupOrgLeagues( data_list_s *org_league_list )
{
     org_league_s *org_leagues = org_league_list->data;

     for ( int i = 0; i < org_league_list->count; ++i )
     {
          if ( org_leagues[i].league != NULL ) free_league( org_leagues[i].league );
     }
}

org_s *getOrg( sqlite3 *db, const int season )
{
     org_s *org = NULL;

     if ( (org = malloc( sizeof(org_s) )) == NULL ) return NULL;

     data_list_s league_list = { 0 };

     leagues_t_read_all( db, &league_list );

     data_list_s  org_leagues = { 0 };

     league_s *leagues = league_list.data;

     for ( int i = 0; i < league_list.count; ++i )
     {
          org_league_s org_league = { 0 };
          league_s        *league = NULL;

          if ( (league = get_league( db, leagues[i].league_id )) == NULL )
          {
               free( leagues );
               free( org     );

               cleanupOrgLeagues( &org_leagues );

               return NULL;
          }

          if ( league->teams != NULL ) getLeagueTeams( db, league->teams );

          if ( league->divisions != NULL ) getLeagueDivisions( db, league->divisions );

          org_league.league = league;

          if ( add_to_data_list( &org_leagues, &org_league, sizeof(org_league_s), 5 ) < 0 )
          {
               free( leagues );
               free( league  );
               free( org     );

               return NULL;
          }
     }

     free( leagues );

     org_league_s sentinel = ORG_LEAGUE_SENTINEL;

     if ( add_to_data_list( &org_leagues, &sentinel, sizeof(org_league_s), 5 ) < 0 )
     {
          free( org );

          cleanupOrgLeagues( &org_leagues );

          return NULL;
     }

     org->leagues = org_leagues.data;

     return org;
}

void freeOrgLeagues( org_league_s *org_leagues )
{
     for ( int i = 0; org_leagues[i].league != NULL; ++i )
     {
          free_league( org_leagues[i].league );
     }

     free( org_leagues );
}

void freeOrg( org_s *org )
{
     if ( org->leagues != NULL )
     {
          freeOrgLeagues( org->leagues );
     }

     free( org );
}
