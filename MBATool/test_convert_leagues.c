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
     org_data.records      = buildRecords();

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
     org_data.records      = buildRecords();

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

static void run_all_tests()
{
     run_test( convertLeagues_ShouldReturnAListOfOrgLeagues_GivenLeagueFileData,    null );
     run_test( convertLeagues_ShouldReturnLeaguesWithDivisions_GivenLeagueFileData, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
