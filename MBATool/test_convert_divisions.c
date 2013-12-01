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
     org_data.records      = buildRecords( org_data.league_data, 1, sp_Regular );

     fileleagname_s    *league_data      = org_data.league_data;
     league_division_s *league_divisions = convertDivisions( &org_data, 1, 0 );

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

     league_divisions = convertDivisions( &org_data, 2, 1 );

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
     org_data.records      = buildRecords( org_data.league_data, 1, sp_Regular );

     fileleagname_s    *league_data      = org_data.league_data;
     fileparks_s       *parks_data       = org_data.parks_data;
     league_division_s *league_divisions = convertDivisions( &org_data, 1, 0 );

     for ( int i = 0; i < DIVISIONS_PER_LEAGUE; ++i )
     {
          assertNotNull( league_divisions[i].division );

          division_team_s *division_teams = league_divisions[i].division->teams;

          assertNotNull( division_teams );

          for ( int j = 0; j < TEAMS_PER_DIVISION; ++j )
          {
               int idx      = (i * TEAMS_PER_DIVISION) + j;
               int park_idx = byte2int( league_data->teams[idx].stadium );

               assertNotNull(                                                     division_teams[j].team                  );
               assertEqualsInt(           i + 1,                                  division_teams[j].division_id           );
               assertEqualsInt( byte2int( league_data->teams[idx].team_id ),      division_teams[j].team_id               );
               assertEqualsInt( byte2int( league_data->teams[idx].team_id ),      division_teams[j].team->team_id         );
               assertEqualsStr(           league_data->teams[idx].name,           division_teams[j].team->name            );
               assertEqualsStr(           parks_data->park_names[park_idx].text,  division_teams[j].team->location        );
               assertEqualsInt( byte2int( league_data->teams[idx].color ),        division_teams[j].team->primary_color   );
               assertEqualsInt( byte2int( league_data->teams[idx].color ),        division_teams[j].team->secondary_color );
          }
     }

     free_league_divisions( league_divisions );

     return NULL;
}

static char *convertDivisions_ShouldReturnDivisionsWithStats_GivenOrgData()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 7, sp_Regular );
     org_data.season       = 7;
     org_data.season_phase = sp_Regular;

     records_s *records = org_data.records;

     league_division_s *league_divisions = convertDivisions( &org_data, 1, 0 );

     for ( int i = 0; i < DIVISIONS_PER_LEAGUE; ++i )
     {
          assertNotNull( league_divisions[i].division );

          division_stats_s  sentinel       = DIVISION_STATS_SENTINEL;
          division_stats_s *division_stats = league_divisions[i].division->stats;

          assertNotNull( division_stats );

          assertEquals( league_divisions[i].division->division_id, division_stats[0].division_id   );
          assertEquals( org_data.season,                           division_stats[0].season        );
          assertEquals( org_data.season_phase,                     division_stats[0].season_phase  );
          assertEquals( records->divisions[i].wins,                division_stats[0].wins          );
          assertEquals( records->divisions[i].losses,              division_stats[0].losses        );
          assertEquals( records->divisions[i].home_wins,           division_stats[0].home_wins     );
          assertEquals( records->divisions[i].home_losses,         division_stats[0].home_losses   );
          assertEquals( records->divisions[i].road_wins,           division_stats[0].road_wins     );
          assertEquals( records->divisions[i].road_losses,         division_stats[0].road_losses   );
          assertEquals( records->divisions[i].league_wins,         division_stats[0].league_wins   );
          assertEquals( records->divisions[i].league_losses,       division_stats[0].league_losses );
          assertEquals( records->divisions[i].runs_scored,         division_stats[0].runs_scored   );
          assertEquals( records->divisions[i].runs_allowed,        division_stats[0].runs_allowed  );

          assertEquals( sentinel.division_id,   division_stats[1].division_id   );
          assertEquals( sentinel.season,        division_stats[1].season        );
          assertEquals( sentinel.season_phase,  division_stats[1].season_phase  );
          assertEquals( sentinel.wins,          division_stats[1].wins          );
          assertEquals( sentinel.losses,        division_stats[1].losses        );
          assertEquals( sentinel.home_wins,     division_stats[1].home_wins     );
          assertEquals( sentinel.home_losses,   division_stats[1].home_losses   );
          assertEquals( sentinel.road_wins,     division_stats[1].road_wins     );
          assertEquals( sentinel.road_losses,   division_stats[1].road_losses   );
          assertEquals( sentinel.league_wins,   division_stats[1].league_wins   );
          assertEquals( sentinel.league_losses, division_stats[1].league_losses );
          assertEquals( sentinel.runs_scored,   division_stats[1].runs_scored   );
          assertEquals( sentinel.runs_allowed,  division_stats[1].runs_allowed  );
     }

     free_league_divisions( league_divisions );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertDivisions_ShouldReturnAListOfLeagueDivisions_GivenALeagueFileDataAndLeagueId,      null );
     run_test( convertDivisions_ShouldReturnDivisionsWithTeams_GivenALeagueFileDataParksDataAndLeagueId, null );
     run_test( convertDivisions_ShouldReturnDivisionsWithStats_GivenOrgData,                             null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
