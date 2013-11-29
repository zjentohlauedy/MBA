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

static boolean_e addLeagueTeamToList( data_list_s *list, const int league_id, team_s *team )
{
     league_team_s league_team = { 0 };

     league_team.league_id = league_id;
     league_team.team_id   = team->team_id;
     league_team.team      = team;

     if ( add_to_data_list( list, &league_team, sizeof(league_team_s), 10 ) < 0 ) return bl_False;

     return bl_True;
}

static boolean_e addDivisionTeamToList( data_list_s *list, const int division_id, team_s *team )
{
     division_team_s division_team = { 0 };

     division_team.division_id = division_id;
     division_team.team_id     = team->team_id;
     division_team.team        = team;

     if ( add_to_data_list( list, &division_team, sizeof(division_team_s), 10 ) < 0 ) return bl_False;

     return bl_True;
}

static team_batting_stats_s *convertBattingStats( const team_player_s *players, const int team_id )
{
     data_list_s           list              = { 0 };
     team_batting_stats_s sentinel           = TEAM_BATTING_STATS_SENTINEL;
     team_batting_stats_s team_batting_stats = { 0 };

     for ( int i = 0; players[i].team_id >= 0; ++i )
     {
          if ( players[i].player->player_type == pt_Pitcher ) continue;

          batter_stats_s *batter_stats = players[i].player->details.batting->stats;

          team_batting_stats.team_id         = players[i].team_id;
          team_batting_stats.season          = batter_stats[0].season;
          team_batting_stats.season_phase    = batter_stats[0].season_phase;
          team_batting_stats.games          += batter_stats[0].games;
          team_batting_stats.at_bats        += batter_stats[0].at_bats;
          team_batting_stats.runs           += batter_stats[0].runs;
          team_batting_stats.hits           += batter_stats[0].hits;
          team_batting_stats.doubles        += batter_stats[0].doubles;
          team_batting_stats.triples        += batter_stats[0].triples;
          team_batting_stats.home_runs      += batter_stats[0].home_runs;
          team_batting_stats.runs_batted_in += batter_stats[0].runs_batted_in;
          team_batting_stats.walks          += batter_stats[0].walks;
          team_batting_stats.strike_outs    += batter_stats[0].strike_outs;
          team_batting_stats.steals         += batter_stats[0].steals;
          team_batting_stats.errors         += batter_stats[0].errors;
     }

     if ( add_to_data_list( &list, &team_batting_stats, sizeof(team_batting_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,           sizeof(team_batting_stats_s), 5 );

     return list.data;
}

static team_pitching_stats_s *convertPitchingStats( const team_player_s *players, const int team_id )
{
     data_list_s           list                = { 0 };
     team_pitching_stats_s sentinel            = TEAM_PITCHING_STATS_SENTINEL;
     team_pitching_stats_s team_pitching_stats = { 0 };

     for ( int i = 0; players[i].team_id >= 0; ++i )
     {
          if ( players[i].player->player_type != pt_Pitcher ) continue;

          pitcher_stats_s *pitcher_stats = players[i].player->details.pitching->stats;

          team_pitching_stats.team_id          = players[i].team_id;
          team_pitching_stats.season           = pitcher_stats[0].season;
          team_pitching_stats.season_phase     = pitcher_stats[0].season_phase;
          team_pitching_stats.wins            += pitcher_stats[0].wins;
          team_pitching_stats.losses          += pitcher_stats[0].losses;
          team_pitching_stats.games           += pitcher_stats[0].games;
          team_pitching_stats.saves           += pitcher_stats[0].saves;
          team_pitching_stats.innings.innings += pitcher_stats[0].innings.innings;
          team_pitching_stats.innings.outs    += pitcher_stats[0].innings.outs;
          team_pitching_stats.hits            += pitcher_stats[0].hits;
          team_pitching_stats.earned_runs     += pitcher_stats[0].earned_runs;
          team_pitching_stats.home_runs       += pitcher_stats[0].home_runs;
          team_pitching_stats.walks           += pitcher_stats[0].walks;
          team_pitching_stats.strike_outs     += pitcher_stats[0].strike_outs;
     }

     team_pitching_stats.innings.innings += (team_pitching_stats.innings.outs / 3);
     team_pitching_stats.innings.outs     = (team_pitching_stats.innings.outs % 3);

     if ( add_to_data_list( &list, &team_pitching_stats, sizeof(team_pitching_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,            sizeof(team_pitching_stats_s), 5 );

     return list.data;
}

static team_stats_s *convertTeamStats( const team_stats_s *team_stats )
{
     data_list_s  list       = { 0 };
     team_stats_s sentinel   = TEAM_STATS_SENTINEL;

     if ( add_to_data_list( &list,  team_stats, sizeof(team_stats_s), 5 ) < 0 ) return NULL;
     /**/ add_to_data_list( &list, &sentinel,   sizeof(team_stats_s), 5 );

     return list.data;
}


static data_list_s *convertTeams( const org_data_s *org_data,
                           const int team_count,
                           const int parent_id,
                           const int parent_idx,
                           boolean_e (addTeamToList)(data_list_s *, const int, team_s *) )
{
     static data_list_s  list               = { 0 };
     /**/   team_s      *teams[TOTAL_TEAMS] = { 0 };

     list.data  = NULL;
     list.__max = 0;
     list.count = 0;

     fileleagname_s *league_data = org_data->league_data;
     fileparks_s    *parks_data  = org_data->parks_data;

     for ( int i = 0; i < team_count; ++i )
     {
          int team_idx = (parent_idx * team_count) + i;
          int team_id  = byte2int( league_data->teams[team_idx].team_id );
          int park_idx = byte2int( league_data->teams[team_idx].stadium );

          if ( (teams[i] = createTeam( team_id, league_data->teams[team_idx].name, parks_data->park_names[park_idx].text, league_data->teams[team_idx].color[0] )) == NULL )
          {
               freeTeams( teams, TOTAL_TEAMS );

               return NULL;
          }

          if ( (teams[i]->players = convertPlayers( org_data, team_id, team_idx )) == NULL )
          {
               freeTeams( teams, TOTAL_TEAMS );

               return NULL;
          }

          if ( (teams[i]->pitching_stats = convertPitchingStats( teams[i]->players, team_id )) == NULL )
          {
               freeTeams( teams, TOTAL_TEAMS );

               return NULL;
          }

          if ( (teams[i]->batting_stats = convertBattingStats( teams[i]->players, team_id )) == NULL )
          {
               freeTeams( teams, TOTAL_TEAMS );

               return NULL;
          }

          if ( (teams[i]->stats = convertTeamStats( &(org_data->records->teams[i]) )) == NULL )
          {
               freeTeams( teams, TOTAL_TEAMS );

               return NULL;
          }

          if ( addTeamToList( &list, parent_id, teams[i] ) != bl_True )
          {
               freeTeams( teams, TOTAL_TEAMS );

               return NULL;
          }
     }

     return &list;
}


division_team_s *convertDivisionTeams( const org_data_s *org_data, const int division_id, const int division_idx )
{
     division_team_s  sentinel = DIVISION_TEAM_SENTINEL;
     data_list_s     *list     = NULL;

     list = convertTeams( org_data, TEAMS_PER_DIVISION, division_id, division_idx, addDivisionTeamToList );

     add_to_data_list( list, &sentinel, sizeof(division_team_s), 10 );

     return list->data;
}

league_team_s *convertLeagueTeams( const org_data_s *org_data, const int league_id, const int league_idx )
{
     league_team_s  sentinel = LEAGUE_TEAM_SENTINEL;
     data_list_s   *list     = NULL;

     list = convertTeams( org_data, TEAMS_PER_LEAGUE, league_id, league_idx, addLeagueTeamToList );

     add_to_data_list( list, &sentinel, sizeof(league_team_s), 10 );

     return list->data;
}
