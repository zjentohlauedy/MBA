#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"
#include "division.h"
#include "league.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static char *convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData()
{
     fileleagname_s  league_data  = { 0 };
     char           *league1_name = "League1";
     char           *league2_name = "League2";

     strcpy( league_data.leagues[0].name, league1_name );
     strcpy( league_data.leagues[1].name, league2_name );

     org_s *org = convertOrg( &league_data );

     assertNotNull( org );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     assertNotNull(                 leagues[0].league            );
     assertEqualsInt( 1,            leagues[0].league->league_id );
     assertEqualsStr( league1_name, leagues[0].league->name      );

     assertNotNull(                 leagues[1].league            );
     assertEqualsInt( 2,            leagues[1].league->league_id );
     assertEqualsStr( league2_name, leagues[1].league->name      );

     assertNull( leagues[2].league );

     freeOrg( org );

     return NULL;
}

static char *convertOrg_ShouldReturnOrgWithDivisions_GivenLeagueFileData()
{
     fileleagname_s  league_data  = { 0 };
     char           *league1_name = "League1";
     char           *league2_name = "League2";
     char           *div1_name    = "Division1";
     char           *div2_name    = "Division2";
     char           *div3_name    = "Division3";
     char           *div4_name    = "Division4";

     strcpy( league_data.leagues[0].name,   league1_name );
     strcpy( league_data.leagues[1].name,   league2_name );
     strcpy( league_data.divisions[0].name, div1_name    );
     strcpy( league_data.divisions[1].name, div2_name    );
     strcpy( league_data.divisions[2].name, div3_name    );
     strcpy( league_data.divisions[3].name, div4_name    );

     org_s *org = convertOrg( &league_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     league_division_s *league1_divisions = leagues[0].league->divisions;

     assertNotNull( league1_divisions );

     assertNotNull(              league1_divisions[0].division              );
     assertEqualsInt( 1,         league1_divisions[0].division_id           );
     assertEqualsInt( 1,         league1_divisions[0].division->division_id );
     assertEqualsStr( div1_name, league1_divisions[0].division->name        );

     assertNotNull(              league1_divisions[1].division              );
     assertEqualsInt( 2,         league1_divisions[1].division_id           );
     assertEqualsInt( 2,         league1_divisions[1].division->division_id );
     assertEqualsStr( div2_name, league1_divisions[1].division->name        );

     league_division_s *league2_divisions = leagues[1].league->divisions;

     assertNotNull( league2_divisions );

     assertNotNull(              league2_divisions[0].division              );
     assertEqualsInt( 3,         league2_divisions[0].division_id           );
     assertEqualsInt( 3,         league2_divisions[0].division->division_id );
     assertEqualsStr( div3_name, league2_divisions[0].division->name        );

     assertNotNull(              league2_divisions[1].division              );
     assertEqualsInt( 4,         league2_divisions[1].division_id           );
     assertEqualsInt( 4,         league2_divisions[1].division->division_id );
     assertEqualsStr( div4_name, league2_divisions[1].division->name        );

     assertNull( leagues[2].league );

     freeOrg( org );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData,   null );
     run_test( convertOrg_ShouldReturnOrgWithDivisions_GivenLeagueFileData, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
