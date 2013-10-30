#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static char *convertDivisions_ShouldReturnAListOfLeagueDivisions_GivenALeagueFileDataAndLeagueId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     fileleagname_s    *league_data      = org_data.league_data;
     league_division_s *league_divisions = convertDivisions( &org_data, 1 );

     assertNotNull( league_divisions );

     assertNotNull(                                   league_divisions[0].division              );
     assertEqualsInt( 1,                              league_divisions[0].league_id             );
     assertEqualsInt( 1,                              league_divisions[0].division_id           );
     assertEqualsInt( 1,                              league_divisions[0].division->division_id );
     assertEqualsStr( league_data->divisions[0].name, league_divisions[0].division->name        );

     assertNotNull(                                   league_divisions[1].division              );
     assertEqualsInt( 1,                              league_divisions[1].league_id             );
     assertEqualsInt( 2,                              league_divisions[1].division_id           );
     assertEqualsInt( 2,                              league_divisions[1].division->division_id );
     assertEqualsStr( league_data->divisions[1].name, league_divisions[1].division->name        );

     assertNull( league_divisions[2].division );

     free_league_divisions( league_divisions );

     league_divisions = convertDivisions( &org_data, 2 );

     assertNotNull( league_divisions );

     assertNotNull(                                   league_divisions[0].division              );
     assertEqualsInt( 2,                              league_divisions[0].league_id             );
     assertEqualsInt( 3,                              league_divisions[0].division_id           );
     assertEqualsInt( 3,                              league_divisions[0].division->division_id );
     assertEqualsStr( league_data->divisions[2].name, league_divisions[0].division->name        );

     assertNotNull(                                   league_divisions[1].division              );
     assertEqualsInt( 2,                              league_divisions[1].league_id             );
     assertEqualsInt( 4,                              league_divisions[1].division_id           );
     assertEqualsInt( 4,                              league_divisions[1].division->division_id );
     assertEqualsStr( league_data->divisions[3].name, league_divisions[1].division->name        );

     assertNull( league_divisions[2].division );

     free_league_divisions( league_divisions );

     return NULL;
}

static char *convertDivisions_ShouldReturnDivisionsWithTeams_GivenALeagueFileDataParksDataAndLeagueId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     fileleagname_s    *league_data      = org_data.league_data;
     fileparks_s       *parks_data       = org_data.parks_data;
     league_division_s *league_divisions = convertDivisions( &org_data, 1 );

     for ( int i = 0; i < DIVISIONS_PER_LEAGUE; ++i )
     {
          assertNotNull( league_divisions[i].division );

          division_team_s *division_teams = league_divisions[i].division->teams;

          assertNotNull( division_teams );

          for ( int j = 0; j < TEAMS_PER_DIVISION; ++j )
          {
               int idx = (i * TEAMS_PER_DIVISION) + j;
               int id  = (i * TEAMS_PER_DIVISION) + j + 1;

               assertNotNull(                                     division_teams[j].team                  );
               assertEqualsInt( i + 1,                            division_teams[j].division_id           );
               assertEqualsInt( id,                               division_teams[j].team_id               );
               assertEqualsInt( id,                               division_teams[j].team->team_id         );
               assertEqualsStr( league_data->teams[idx].name,     division_teams[j].team->name            );
               assertEqualsStr( parks_data->park_names[idx].text, division_teams[j].team->location        );
               assertEqualsInt( league_data->teams[idx].color[0], division_teams[j].team->primary_color   );
               assertEqualsInt( league_data->teams[idx].color[0], division_teams[j].team->secondary_color );
          }
     }

     free_league_divisions( league_divisions );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertDivisions_ShouldReturnAListOfLeagueDivisions_GivenALeagueFileDataAndLeagueId,      null );
     run_test( convertDivisions_ShouldReturnDivisionsWithTeams_GivenALeagueFileDataParksDataAndLeagueId, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
