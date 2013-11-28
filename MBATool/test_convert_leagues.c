#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static char *convertLeagues_ShouldReturnAListOfOrgLeagues_GivenLeagueFileData()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( 1, sp_Regular );

     fileleagname_s *league_data = org_data.league_data;
     org_league_s   *org_leagues = convertLeagues( &org_data );

     assertNotNull( org_leagues );

     assertNotNull(                                 org_leagues[0].league            );
     assertEqualsInt( 1,                            org_leagues[0].league->league_id );
     assertEqualsStr( league_data->leagues[0].name, org_leagues[0].league->name      );

     assertNotNull(                                 org_leagues[1].league            );
     assertEqualsInt( 2,                            org_leagues[1].league->league_id );
     assertEqualsStr( league_data->leagues[1].name, org_leagues[1].league->name      );

     assertNull( org_leagues[2].league );

     freeOrgLeagues( org_leagues );

     return NULL;
}

static char *convertLeagues_ShouldReturnLeaguesWithDivisions_GivenLeagueFileData()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( 1, sp_Regular );

     fileleagname_s *league_data = org_data.league_data;
     org_league_s   *org_leagues = convertLeagues( &org_data );

     assertNotNull( org_leagues );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          assertNotNull( org_leagues[i].league );

          league_division_s *league_divisions = org_leagues[i].league->divisions;

          assertNotNull( league_divisions );

          for ( int j = 0; j < DIVISIONS_PER_LEAGUE; ++j )
          {
               int idx = (i * DIVISIONS_PER_LEAGUE) + j;
               int id  = (i * DIVISIONS_PER_LEAGUE) + j + 1;

               assertNotNull(                                     league_divisions[j].division              );
               assertEqualsInt( i + 1,                            league_divisions[j].league_id             );
               assertEqualsInt( id,                               league_divisions[j].division_id           );
               assertEqualsInt( id,                               league_divisions[j].division->division_id );
               assertEqualsStr( league_data->divisions[idx].name, league_divisions[j].division->name        );
          }
     }

     freeOrgLeagues( org_leagues );

     return NULL;
}

static char *convertLeagues_ShouldReturnLeaguesWithStats_GivenLeagueFileDataAndRecords()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( 1, sp_Regular );
     org_data.season       = 1;
     org_data.season_phase = sp_Regular;

     records_s *records = org_data.records;

     org_league_s   *org_leagues = convertLeagues( &org_data );

     assertNotNull( org_leagues );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          assertNotNull( org_leagues[i].league );

          league_stats_s  sentinel     = LEAGUE_STATS_SENTINEL;
          league_stats_s *league_stats = org_leagues[i].league->stats;

          assertNotNull( league_stats );

          assertEquals( org_leagues[i].league->league_id, league_stats[0].league_id     );
          assertEquals( org_data.season,                  league_stats[0].season        );
          assertEquals( org_data.season_phase,            league_stats[0].season_phase  );
          assertEquals( records->leagues[i].wins,         league_stats[0].wins          );
          assertEquals( records->leagues[i].losses,       league_stats[0].losses        );
          assertEquals( records->leagues[i].home_wins,    league_stats[0].home_wins     );
          assertEquals( records->leagues[i].home_losses,  league_stats[0].home_losses   );
          assertEquals( records->leagues[i].road_wins,    league_stats[0].road_wins     );
          assertEquals( records->leagues[i].road_losses,  league_stats[0].road_losses   );
          assertEquals( records->leagues[i].runs_scored,  league_stats[0].runs_scored   );
          assertEquals( records->leagues[i].runs_allowed, league_stats[0].runs_allowed  );

          assertEquals( sentinel.league_id,    league_stats[1].league_id     );
          assertEquals( sentinel.season,       league_stats[1].season        );
          assertEquals( sentinel.season_phase, league_stats[1].season_phase  );
          assertEquals( sentinel.wins,         league_stats[1].wins          );
          assertEquals( sentinel.losses,       league_stats[1].losses        );
          assertEquals( sentinel.home_wins,    league_stats[1].home_wins     );
          assertEquals( sentinel.home_losses,  league_stats[1].home_losses   );
          assertEquals( sentinel.road_wins,    league_stats[1].road_wins     );
          assertEquals( sentinel.road_losses,  league_stats[1].road_losses   );
          assertEquals( sentinel.runs_scored,  league_stats[1].runs_scored   );
          assertEquals( sentinel.runs_allowed, league_stats[1].runs_allowed  );
     }

     freeOrgLeagues( org_leagues );

     return NULL;
}

static char *convertLeagues_ShouldReturnLeaguesWithTeams_GivenLeagueFileAndAllstarPhase()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( 1, sp_Allstar );
     org_data.season       = 1;
     org_data.season_phase = sp_Allstar;

     fileleagname_s *league_data = org_data.league_data;
     org_league_s   *org_leagues = convertLeagues( &org_data );

     assertNotNull( org_leagues );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          assertNotNull( org_leagues[i].league );

          league_team_s *league_teams = org_leagues[i].league->teams;

          assertNotNull( league_teams );

          for ( int j = 0; j < TEAMS_PER_LEAGUE; ++j )
          {
               int idx = (i * TEAMS_PER_LEAGUE) + j;
               int id  = (i * TEAMS_PER_LEAGUE) + j + 1;

               assertNotNull(                                 league_teams[j].team          );
               assertEqualsInt( i + 1,                        league_teams[j].league_id     );
               assertEqualsInt( id,                           league_teams[j].team_id       );
               assertEqualsInt( id,                           league_teams[j].team->team_id );
               assertEqualsStr( league_data->teams[idx].name, league_teams[j].team->name    );
          }
     }

     freeOrgLeagues( org_leagues );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertLeagues_ShouldReturnAListOfOrgLeagues_GivenLeagueFileData,           null );
     run_test( convertLeagues_ShouldReturnLeaguesWithDivisions_GivenLeagueFileData,        null );
     run_test( convertLeagues_ShouldReturnLeaguesWithStats_GivenLeagueFileDataAndRecords,  null );
     run_test( convertLeagues_ShouldReturnLeaguesWithTeams_GivenLeagueFileAndAllstarPhase, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
