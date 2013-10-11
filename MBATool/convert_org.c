#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_list.h"
#include "file_formats.h"
#include "org.h"


static player_s *createPlayer( const int player_id, const char *first_name, const char *last_name )
{
     player_s *player = NULL;

     if ( (player = malloc( sizeof(player_s) )) == NULL ) return NULL;

     memset( player, '\0', sizeof(player_s) );

     strcpy( player->first_name,   first_name );
     strcpy( player->last_name,    last_name  );
     /**/    player->player_id   = player_id;

     return player;
}

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

static void freePlayers( player_s *players[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( players[i] != NULL ) free_player( players[i] );
     }
}

static void freeTeams( team_s *teams[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( teams[i] != NULL ) free_team( teams[i] );
     }
}

static void freeDivisions( division_s *divisions[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( divisions[i] != NULL ) free_division( divisions[i] );
     }
}

static void freeLeagues( league_s *leagues[], const int count )
{
     for ( int i = 0; i < count; ++i )
     {
          if ( leagues[i] != NULL ) free_league( leagues[i] );
     }
}

static boolean_e addPlayerToList( data_list_s *list, const int team_id, player_s *player )
{
     team_player_s team_player = { 0 };

     team_player.team_id   = team_id;
     team_player.player_id = player->player_id;
     team_player.player    = player;

     if ( add_to_data_list( list, &team_player, sizeof(team_player_s), 30 ) < 0 ) return bl_False;

     return bl_True;
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

static boolean_e addDivisionToList( data_list_s *list, const int league_id, division_s *division )
{
     league_division_s league_division = { 0 };

     league_division.league_id   = league_id;
     league_division.division_id = division->division_id;
     league_division.division    = division;

     if ( add_to_data_list( list, &league_division, sizeof(league_division_s), 5 ) < 0 ) return bl_False;

     return bl_True;
}

static boolean_e addLeagueToList( data_list_s *list, league_s *league )
{
     org_league_s org_league = { 0 };

     org_league.league = league;

     if ( add_to_data_list( list, &org_league, sizeof(org_league_s), 5 ) < 0 ) return bl_False;

     return bl_True;
}

static team_player_s *convertPlayers( const fileleagname_s *league_data, const fileparks_s *parks_data, const fileplayer_s *players_data, const int team_id )
{
     data_list_s    list                      = { 0 };
     team_player_s  sentinel                  = TEAM_PLAYER_SENTINEL;
     player_s      *players[PLAYERS_PER_TEAM] = { 0 };


     int idx = (team_id - 1) * PLAYERS_PER_TEAM;

     for ( int i = 0; i < PLAYERS_PER_TEAM; ++i )
     {
          int player_id = idx + i + 1;

          if ( (players[i] = createPlayer( player_id, players_data[idx + i].first_name, players_data[idx + i].last_name )) == NULL )
          {
               freePlayers( players, PLAYERS_PER_TEAM );

               return NULL;
          }

          if ( addPlayerToList( &list, team_id, players[i] ) != bl_True )
          {
               freePlayers( players, PLAYERS_PER_TEAM );

               return NULL;
          }
     }

     add_to_data_list( &list, &sentinel, sizeof(team_player_s), 30 );

     return list.data;
}

static division_team_s *convertTeams( const fileleagname_s *league_data, const fileparks_s *parks_data, const fileplayer_s *players_data, const int division_id )
{
     data_list_s      list                      = { 0 };
     division_team_s  sentinel                  = DIVISION_TEAM_SENTINEL;
     team_s          *teams[TEAMS_PER_DIVISION] = { 0 };


     int idx = (division_id - 1) * TEAMS_PER_DIVISION;

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          int team_id = idx + i + 1;

          if ( (teams[i] = createTeam( team_id, league_data->teams[idx + i].name, parks_data->park_names[idx + i].text, league_data->teams[idx + i].color[0] )) == NULL )
          {
               freeTeams( teams, TEAMS_PER_DIVISION );

               return NULL;
          }

          if ( (teams[i]->players = convertPlayers( league_data, parks_data, players_data, team_id )) == NULL )
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

static league_division_s *convertDivisions( const fileleagname_s *league_data, const fileparks_s *parks_data, const fileplayer_s *players_data, const int league_id )
{
     data_list_s        list                            = { 0 };
     league_division_s  sentinel                        = LEAGUE_DIVISION_SENTINEL;
     division_s        *divisions[DIVISIONS_PER_LEAGUE] = { 0 };


     int idx = (league_id - 1) * DIVISIONS_PER_LEAGUE;

     for ( int i = 0; i < DIVISIONS_PER_LEAGUE; ++i )
     {
          int division_id = idx + i + 1;

          if ( (divisions[i] = createDivision( division_id, league_data->divisions[idx + i].name )) == NULL )
          {
               freeDivisions( divisions, DIVISIONS_PER_LEAGUE );

               return NULL;
          }

          if ( (divisions[i]->teams = convertTeams( league_data, parks_data, players_data, division_id )) == NULL )
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

static org_league_s *convertLeagues( const fileleagname_s *league_data, const fileparks_s *parks_data, const fileplayer_s *players_data )
{
     data_list_s   list                   = { 0 };
     org_league_s  sentinel               = { 0 };
     league_s     *leagues[TOTAL_LEAGUES] = { 0 };


     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          int league_id = i + 1;

          if ( (leagues[i] = createLeague( league_id, league_data->leagues[i].name )) == NULL )
          {
               freeLeagues( leagues, TOTAL_LEAGUES );

               return NULL;
          }

          if ( (leagues[i]->divisions = convertDivisions( league_data, parks_data, players_data, league_id )) == NULL )
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

org_s *convertOrg( const fileleagname_s *league_data, const fileparks_s *parks_data, const fileplayer_s *players_data )
{
     org_s *org = NULL;

     if ( (org = createOrg()) == NULL ) return NULL;

     if ( (org->leagues = convertLeagues( league_data, parks_data, players_data )) == NULL )
     {
          free( org );

          return NULL;
     }

     return org;
}

static void freeOrgLeagues( org_league_s *org_leagues )
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
