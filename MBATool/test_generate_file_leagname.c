#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "unit_test.h"


static char *result;


static org_s *buildOrg( void )
{
     static org_s             org                                                                                    = { 0 };
     static league_s          leagues                  [TOTAL_LEAGUES    ]                                           = { 0 };
     static org_league_s      org_leagues              [TOTAL_LEAGUES + 1]                                           = { 0 };
     static org_league_s      org_league_sentinel                                                                    = ORG_LEAGUE_SENTINEL;
     static division_s        divisions                [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE    ]                     = { 0 };
     static league_division_s league_divisions         [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE + 1]                     = { 0 };
     static league_division_s league_division_sentinel                                                               = LEAGUE_DIVISION_SENTINEL;
     static team_s            teams                    [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE][TEAMS_PER_DIVISION    ] = { 0 };
     static division_team_s   division_teams           [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE][TEAMS_PER_DIVISION + 1] = { 0 };
     static division_team_s   division_team_sentinel                                                                 = DIVISION_TEAM_SENTINEL;

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          int league_id = i + 1;

          buildIntoLeague( &leagues[i], league_id );

          for ( int j = 0; j < DIVISIONS_PER_LEAGUE; ++j )
          {
               int division_id = (i * DIVISIONS_PER_LEAGUE) + j + 1;

               buildIntoDivision( &divisions[i][j], division_id );
               buildIntoLeagueDivision( &league_divisions[i][j], league_id, division_id );

               for ( int k = 0; k < TEAMS_PER_DIVISION; ++k )
               {
                    int team_id = (i * TEAMS_PER_LEAGUE) + (j * TEAMS_PER_DIVISION) + k + 1;

                    buildIntoTeam( &teams[i][j][k], team_id );
                    buildIntoDivisionTeam( &division_teams[i][j][k], division_id, team_id );

                    division_teams[i][j][k].team = &teams[i][j][k];
               }

               division_teams[i][j][TEAMS_PER_DIVISION] = division_team_sentinel;

               divisions[i][j].teams = division_teams[i][j];

               league_divisions[i][j].division = &divisions[i][j];
          }

          league_divisions[i][DIVISIONS_PER_LEAGUE] = league_division_sentinel;

          leagues[i].divisions = league_divisions[i];

          org_leagues[i].league = &leagues[i];
     }

     org_leagues[TOTAL_LEAGUES] = org_league_sentinel;

     org.leagues = org_leagues;

     return &org;
}


static char *generateFileLeagName_ShouldReturnAFileLeagNameObject_GivenAnOrgObject()
{
     org_s org = { 0 };

     fileleagname_s *league_file = generateFileLeagName( &org );

     assertNotNull( league_file );

     free( league_file );

     return NULL;
}

static char *generateFileLeagName_ShouldReturnAFileLeagNameWithLeagues_GivenAnOrgObject()
{
     org_s *org = buildOrg();

     fileleagname_s *league_file = generateFileLeagName( org );

     assertNotNull( league_file );

     assertEqualsStr( org->leagues[0].league->name, league_file->leagues[0].name );
     assertEqualsStr( org->leagues[1].league->name, league_file->leagues[1].name );

     free( league_file );

     return NULL;
}

static char *generateFileLeagName_ShouldReturnAFileLeagNameWithDivisions_GivenAnOrgObject()
{
     org_s *org = buildOrg();

     fileleagname_s *league_file = generateFileLeagName( org );

     assertNotNull( league_file );

     league_division_s *league_divisions = NULL;

     league_divisions = org->leagues[0].league->divisions;

     assertEqualsStr( league_divisions[0].division->name, league_file->divisions[0].name );
     assertEqualsStr( league_divisions[1].division->name, league_file->divisions[1].name );

     league_divisions = org->leagues[1].league->divisions;

     assertEqualsStr( league_divisions[0].division->name, league_file->divisions[2].name );
     assertEqualsStr( league_divisions[1].division->name, league_file->divisions[3].name );

     free( league_file );

     return NULL;
}

static char *generateFileLeagName_ShouldReturnAFileLeagNameWithTeams_GivenAnOrgObject()
{
     org_s *org = buildOrg();

     fileleagname_s *league_file = generateFileLeagName( org );

     assertNotNull( league_file );

     division_team_s *division_teams = NULL;

     division_teams = org->leagues[0].league->divisions[0].division->teams;

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertEqualsStr( division_teams[i].team->name,                    league_file->teams[i].name      );
          assertEqualsInt( division_teams[i].team->team_id,       byte2int( league_file->teams[i].team_id ) );
          assertEqualsInt( division_teams[i].team->primary_color, byte2int( league_file->teams[i].color   ) );
          assertEqualsInt( i,                                     byte2int( league_file->teams[i].stadium ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i].wins    ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i].losses  ) );
     }

     division_teams = org->leagues[0].league->divisions[1].division->teams;

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertEqualsStr( division_teams[i].team->name,                    league_file->teams[i + 8].name      );
          assertEqualsInt( division_teams[i].team->team_id,       byte2int( league_file->teams[i + 8].team_id ) );
          assertEqualsInt( division_teams[i].team->primary_color, byte2int( league_file->teams[i + 8].color   ) );
          assertEqualsInt( i + 8,                                 byte2int( league_file->teams[i + 8].stadium ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i + 8].wins    ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i + 8].losses  ) );
     }

     division_teams = org->leagues[1].league->divisions[0].division->teams;

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertEqualsStr( division_teams[i].team->name,                    league_file->teams[i + 16].name      );
          assertEqualsInt( division_teams[i].team->team_id,       byte2int( league_file->teams[i + 16].team_id ) );
          assertEqualsInt( division_teams[i].team->primary_color, byte2int( league_file->teams[i + 16].color   ) );
          assertEqualsInt( i + 16,                                byte2int( league_file->teams[i + 16].stadium ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i + 16].wins    ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i + 16].losses  ) );
     }

     division_teams = org->leagues[1].league->divisions[1].division->teams;

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertEqualsStr( division_teams[i].team->name,                    league_file->teams[i + 24].name      );
          assertEqualsInt( division_teams[i].team->team_id,       byte2int( league_file->teams[i + 24].team_id ) );
          assertEqualsInt( division_teams[i].team->primary_color, byte2int( league_file->teams[i + 24].color   ) );
          assertEqualsInt( i + 24,                                byte2int( league_file->teams[i + 24].stadium ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i + 24].wins    ) );
          assertEqualsInt( 0,                                     byte2int( league_file->teams[i + 24].losses  ) );
     }

     free( league_file );

     return NULL;
}

static void run_all_tests()
{
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameObject_GivenAnOrgObject,        null );
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameWithLeagues_GivenAnOrgObject,   null );
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameWithDivisions_GivenAnOrgObject, null );
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameWithTeams_GivenAnOrgObject,     null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
