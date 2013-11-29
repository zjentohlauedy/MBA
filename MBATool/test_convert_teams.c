#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "compares.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static team_batting_stats_s *calcBattingStats( fileplayer_s *players_data, int team_id, int team_idx )
{
     static team_batting_stats_s team_batting_stats = { 0 };

     memset( &team_batting_stats, '\0', sizeof(team_batting_stats_s) );

     team_batting_stats.team_id = team_id;

     int start = team_idx * PLAYERS_PER_TEAM;

     for ( int i = 0; i < PLAYERS_PER_TEAM; ++i )
     {
          fileplayer_s *player = &players_data[start + i];

          if (         player->last_name[0]          == '\0'         ) continue;
          if ( nibble( player->position[0], n_High ) == fpos_Pitcher ) continue;

          acc_bat_stats_s *batting_stats = &(player->acc_stats.simulated.batting);
          acc_bat_stats_s *overflow      = &(player->acc_stats.action.batting);

          int rbi = byte2int( batting_stats->acc_rbi ) + byte2int( overflow->acc_rbi );
          int so  = byte2int( batting_stats->acc_so  ) + byte2int( overflow->acc_so  );

          team_batting_stats.games          += byte2int( batting_stats->acc_games );
          team_batting_stats.at_bats        += word2int( batting_stats->acc_ab    );
          team_batting_stats.runs           += byte2int( batting_stats->acc_runs  );
          team_batting_stats.hits           += word2int( batting_stats->acc_hits  );
          team_batting_stats.doubles        += byte2int( batting_stats->acc_2b    );
          team_batting_stats.triples        += byte2int( batting_stats->acc_3b    );
          team_batting_stats.home_runs      += byte2int( batting_stats->acc_hr    );
          team_batting_stats.runs_batted_in +=                          rbi;
          team_batting_stats.walks          += byte2int( batting_stats->acc_bb    );
          team_batting_stats.strike_outs    +=                          so;
          team_batting_stats.steals         += byte2int( batting_stats->acc_sb    );
          team_batting_stats.errors         += byte2int( batting_stats->acc_err   );
     }

     return &team_batting_stats;
}

static team_pitching_stats_s *calcPitchingStats( fileplayer_s *players_data, int team_id, int team_idx )
{
     static team_pitching_stats_s team_pitching_stats = { 0 };

     memset( &team_pitching_stats, '\0', sizeof(team_pitching_stats_s) );

     team_pitching_stats.team_id = team_id;

     int start = team_idx * PLAYERS_PER_TEAM;

     for ( int i = 0; i < PLAYERS_PER_TEAM; ++i )
     {
          fileplayer_s *player = &players_data[start + i];

          if (         player->last_name[0]          == '\0'         ) continue;
          if ( nibble( player->position[0], n_High ) != fpos_Pitcher ) continue;

          acc_pch_stats_s *pitching_stats = &(player->acc_stats.simulated.pitching);

          team_pitching_stats.wins            += byte2int( pitching_stats->acc_wins    );
          team_pitching_stats.losses          += byte2int( pitching_stats->acc_losses  );
          team_pitching_stats.games           += byte2int( pitching_stats->acc_starts  );
          team_pitching_stats.saves           += byte2int( pitching_stats->acc_saves   );
          team_pitching_stats.innings.innings += word2int( pitching_stats->acc_innings ) / 10;
          team_pitching_stats.innings.outs    += word2int( pitching_stats->acc_innings ) % 10;
          team_pitching_stats.hits            += word2int( pitching_stats->acc_hits    );
          team_pitching_stats.earned_runs     += word2int( pitching_stats->acc_er      );
          team_pitching_stats.home_runs       += byte2int( pitching_stats->acc_hr      );
          team_pitching_stats.walks           += byte2int( pitching_stats->acc_bb      );
          team_pitching_stats.strike_outs     += word2int( pitching_stats->acc_so      );
     }

     team_pitching_stats.innings.innings += (team_pitching_stats.innings.outs / 3);
     team_pitching_stats.innings.outs     = (team_pitching_stats.innings.outs % 3);

     return &team_pitching_stats;
}


static char *convertDivisionTeams_ShouldReturnAListOfDivisionTeamRecords_GivenOrgDataAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 1, sp_Regular );

     fileleagname_s  *league_data    = org_data.league_data;
     fileparks_s     *parks_data     = org_data.parks_data;

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          int division_id = i + 1;

          division_team_s *division_teams = convertDivisionTeams( &org_data, division_id, i );

          assertNotNull( division_teams );

          for ( int j = 0; j < TEAMS_PER_DIVISION; ++j )
          {
               int idx      = (i * TEAMS_PER_DIVISION) + j;
               int park_idx = byte2int( league_data->teams[idx].stadium );

               assertNotNull(                                                    division_teams[j].team                  );
               assertEqualsInt(           division_id,                           division_teams[j].division_id           );
               assertEqualsInt( byte2int( league_data->teams[idx].team_id ),     division_teams[j].team_id               );
               assertEqualsInt( byte2int( league_data->teams[idx].team_id ),     division_teams[j].team->team_id         );
               assertEqualsStr(           league_data->teams[idx].name,          division_teams[j].team->name            );
               assertEqualsStr(           parks_data->park_names[park_idx].text, division_teams[j].team->location        );
               assertEqualsInt( byte2int( league_data->teams[idx].color ),       division_teams[j].team->primary_color   );
               assertEqualsInt( byte2int( league_data->teams[idx].color ),       division_teams[j].team->secondary_color );
          }

          assertNull( division_teams[TEAMS_PER_DIVISION].team );

          free_division_teams( division_teams );
     }

     return NULL;
}

static char *convertDivisionTeams_ShouldReturnTeamsWithPlayers_GivenOrgDataAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 3, sp_Regular );
     org_data.season       = 3;

     fileleagname_s  *league_data    = org_data.league_data;
     fileparks_s     *parks_data     = org_data.parks_data;
     fileplayer_s    *players_data   = org_data.players_data;

     division_team_s *division_teams = convertDivisionTeams( &org_data, 1, 0 );

     assertNotNull( division_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull( division_teams[i].team );

          team_player_s *team_players = division_teams[i].team->players;

          assertNotNull( team_players );

          for ( int j = 0; j < PLAYERS_PER_TEAM; ++j )
          {
               int idx = (i * PLAYERS_PER_TEAM) + j;
               int id  = (i * PLAYERS_PER_TEAM) + j + 1;

               assertNotNull(                                    team_players[j].player                 );
               assertEqualsInt( division_teams[i].team_id,       team_players[j].team_id                );
               assertEqualsInt( org_data.season,                 team_players[j].season                 );
               assertEqualsStr( players_data[idx].first_name,    team_players[j].player->first_name     );
               assertEqualsStr( players_data[idx].last_name,     team_players[j].player->last_name      );
               assertEqualsStr( players_data[idx].first_phoen,   team_players[j].player->first_phonetic );
               assertEqualsStr( players_data[idx].last_phoen,    team_players[j].player->last_phonetic  );
          }
     }

     free_division_teams( division_teams );

     return NULL;
}

static char *convertDivisionTeams_ShouldReturnTeamsWithPitchingStats_GivenOrgDataAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 3, sp_Playoff );
     org_data.season       = 3;
     org_data.season_phase = sp_Playoff;

     fileleagname_s  *league_data    = org_data.league_data;
     fileparks_s     *parks_data     = org_data.parks_data;
     fileplayer_s    *players_data   = org_data.players_data;

     division_team_s *division_teams = convertDivisionTeams( &org_data, 1, 0 );

     assertNotNull( division_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull( division_teams[i].team );

          team_pitching_stats_s *team_pitching_stats = division_teams[i].team->pitching_stats;

          assertNotNull( team_pitching_stats );

          team_pitching_stats_s  sentinel = TEAM_PITCHING_STATS_SENTINEL;
          team_pitching_stats_s *expected = calcPitchingStats( players_data, division_teams[i].team_id, i );

          expected->season       = org_data.season;
          expected->season_phase = org_data.season_phase;

          compareTeamPitchingStats(  expected, &team_pitching_stats[0] );
          compareTeamPitchingStats( &sentinel, &team_pitching_stats[1] );
     }

     free_division_teams( division_teams );

     return NULL;
}

static char *convertDivisionTeams_ShouldReturnTeamsWithBattingStats_GivenOrgDataAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 3, sp_Playoff );
     org_data.season       = 3;
     org_data.season_phase = sp_Playoff;

     fileleagname_s  *league_data    = org_data.league_data;
     fileparks_s     *parks_data     = org_data.parks_data;
     fileplayer_s    *players_data   = org_data.players_data;

     division_team_s *division_teams = convertDivisionTeams( &org_data, 1, 0 );

     assertNotNull( division_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull( division_teams[i].team );

          team_batting_stats_s *team_batting_stats = division_teams[i].team->batting_stats;

          assertNotNull( team_batting_stats );

          team_batting_stats_s  sentinel = TEAM_BATTING_STATS_SENTINEL;
          team_batting_stats_s *expected = calcBattingStats( players_data, division_teams[i].team_id, i );

          expected->season       = org_data.season;
          expected->season_phase = org_data.season_phase;

          compareTeamBattingStats(  expected, &team_batting_stats[0] );
          compareTeamBattingStats( &sentinel, &team_batting_stats[1] );
     }

     free_division_teams( division_teams );

     return NULL;
}

static char *convertDivisionTeams_ShouldReturnTeamsWithStats_GivenOrgDataAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 3, sp_Playoff );
     org_data.season       = 3;
     org_data.season_phase = sp_Playoff;

     division_team_s *division_teams = convertDivisionTeams( &org_data, 1, 0 );

     assertNotNull( division_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull( division_teams[i].team );

          team_stats_s *team_stats = division_teams[i].team->stats;

          assertNotNull( team_stats );

          team_stats_s *expected = &(org_data.records->teams[i]);

          assertEqualsInt( division_teams[i].team->team_id, team_stats[0].team_id         );
          assertEqualsInt( org_data.season,                 team_stats[0].season          );
          assertEqualsInt( org_data.season_phase,           team_stats[0].season_phase    );
          assertEqualsInt( expected->wins,                  team_stats[0].wins            );
          assertEqualsInt( expected->losses,                team_stats[0].losses          );
          assertEqualsInt( expected->home_wins,             team_stats[0].home_wins       );
          assertEqualsInt( expected->home_losses,           team_stats[0].home_losses     );
          assertEqualsInt( expected->road_wins,             team_stats[0].road_wins       );
          assertEqualsInt( expected->road_losses,           team_stats[0].road_losses     );
          assertEqualsInt( expected->division_wins,         team_stats[0].division_wins   );
          assertEqualsInt( expected->division_losses,       team_stats[0].division_losses );
          assertEqualsInt( expected->league_wins,           team_stats[0].league_wins     );
          assertEqualsInt( expected->league_losses,         team_stats[0].league_losses   );
          assertEqualsInt( expected->runs_scored,           team_stats[0].runs_scored     );
          assertEqualsInt( expected->runs_allowed,          team_stats[0].runs_allowed    );

          team_stats_s sentinel = TEAM_STATS_SENTINEL;

          assertEqualsInt( sentinel.team_id,         team_stats[1].team_id         );
          assertEqualsInt( sentinel.season,          team_stats[1].season          );
          assertEqualsInt( sentinel.season_phase,    team_stats[1].season_phase    );
          assertEqualsInt( sentinel.wins,            team_stats[1].wins            );
          assertEqualsInt( sentinel.losses,          team_stats[1].losses          );
          assertEqualsInt( sentinel.home_wins,       team_stats[1].home_wins       );
          assertEqualsInt( sentinel.home_losses,     team_stats[1].home_losses     );
          assertEqualsInt( sentinel.road_wins,       team_stats[1].road_wins       );
          assertEqualsInt( sentinel.road_losses,     team_stats[1].road_losses     );
          assertEqualsInt( sentinel.division_wins,   team_stats[1].division_wins   );
          assertEqualsInt( sentinel.division_losses, team_stats[1].division_losses );
          assertEqualsInt( sentinel.league_wins,     team_stats[1].league_wins     );
          assertEqualsInt( sentinel.league_losses,   team_stats[1].league_losses   );
          assertEqualsInt( sentinel.runs_scored,     team_stats[1].runs_scored     );
          assertEqualsInt( sentinel.runs_allowed,    team_stats[1].runs_allowed    );
     }

     free_division_teams( division_teams );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertDivisionTeams_ShouldReturnAListOfDivisionTeamRecords_GivenOrgDataAndDivisionId, null );
     run_test( convertDivisionTeams_ShouldReturnTeamsWithPlayers_GivenOrgDataAndDivisionId,           null );
     run_test( convertDivisionTeams_ShouldReturnTeamsWithPitchingStats_GivenOrgDataAndDivisionId,     null );
     run_test( convertDivisionTeams_ShouldReturnTeamsWithBattingStats_GivenOrgDataAndDivisionId,      null );
     run_test( convertDivisionTeams_ShouldReturnTeamsWithStats_GivenOrgDataAndDivisionId,             null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
