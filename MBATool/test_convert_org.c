#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static char *convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.records      = buildRecords( org_data.league_data, 1, sp_Regular );

     org_s *org = convertOrg( &org_data );

     assertNotNull( org );

     fileleagname_s *league_data = org_data.league_data;
     org_league_s   *leagues     = org->leagues;

     assertNotNull( leagues );

     assertNotNull(                                 leagues[0].league            );
     assertEqualsInt( 1,                            leagues[0].league->league_id );
     assertEqualsStr( league_data->leagues[0].name, leagues[0].league->name      );

     assertNotNull(                                 leagues[1].league            );
     assertEqualsInt( 2,                            leagues[1].league->league_id );
     assertEqualsStr( league_data->leagues[1].name, leagues[1].league->name      );

     assertNull( leagues[2].league );

     freeOrg( org );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData,                         null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
