#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static char *convertTeams_ShouldReturnAListOfDivisionTeamRecords_GivenALeagueDataFileParksDataFileAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     fileleagname_s  *league_data    = org_data.league_data;
     fileparks_s     *parks_data     = org_data.parks_data;

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          int division_id = i + 1;

          division_team_s *division_teams = convertTeams( &org_data, division_id );

          assertNotNull( division_teams );

          for ( int j = 0; j < TEAMS_PER_DIVISION; ++j )
          {
               int idx = (i * TEAMS_PER_DIVISION) + j;
               int id  = (i * TEAMS_PER_DIVISION) + j + 1;

               assertNotNull(                                     division_teams[j].team                  );
               assertEqualsInt( division_id,                      division_teams[j].division_id           );
               assertEqualsInt( id,                               division_teams[j].team_id               );
               assertEqualsInt( id,                               division_teams[j].team->team_id         );
               assertEqualsStr( league_data->teams[idx].name,     division_teams[j].team->name            );
               assertEqualsStr( parks_data->park_names[idx].text, division_teams[j].team->location        );
               assertEqualsInt( league_data->teams[idx].color[0], division_teams[j].team->primary_color   );
               assertEqualsInt( league_data->teams[idx].color[0], division_teams[j].team->secondary_color );
          }

          assertNull( division_teams[TEAMS_PER_DIVISION].team );

          free_division_teams( division_teams );
     }

     return NULL;
}

static char *convertTeams_ShouldReturnTeamsWithPlayers_GivenOrgDataAndDivisionId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.season       = 3;

     fileleagname_s  *league_data    = org_data.league_data;
     fileparks_s     *parks_data     = org_data.parks_data;
     fileplayer_s    *players_data   = org_data.players_data;

     division_team_s *division_teams = convertTeams( &org_data, 1 );

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
               assertEqualsInt( i + 1,                           team_players[j].team_id                );
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

static void run_all_tests()
{
     run_test( convertTeams_ShouldReturnAListOfDivisionTeamRecords_GivenALeagueDataFileParksDataFileAndDivisionId, null );
     run_test( convertTeams_ShouldReturnTeamsWithPlayers_GivenOrgDataAndDivisionId,                                null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
