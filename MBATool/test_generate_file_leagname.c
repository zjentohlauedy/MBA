#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "unit_test.h"


static char *result;


static org_s *buildOrg( void )
{
     static org_s             org                        = { 0 };
     static org_league_s      org_leagues[3]             = { 0 };
     static org_league_s      org_league_sentinel        = ORG_LEAGUE_SENTINEL;
     static league_s          league1                    = { 0 };
     static league_s          league2                    = { 0 };
     static league_division_s league_divisions1[3]       = { 0 };
     static league_division_s league_divisions2[3]       = { 0 };
     static league_division_s league_division_sentinel   = LEAGUE_DIVISION_SENTINEL;
     static division_s        division1                  = { 0 };
     static division_s        division2                  = { 0 };
     static division_s        division3                  = { 0 };
     static division_s        division4                  = { 0 };
     static division_team_s   division_teams1[9]         = { 0 };
     static division_team_s   division_teams2[9]         = { 0 };
     static division_team_s   division_teams3[9]         = { 0 };
     static division_team_s   division_teams4[9]         = { 0 };
     static division_team_s   division_team_sentinel     = DIVISION_TEAM_SENTINEL;
     static team_s            team01                     = { 0 };
     static team_s            team02                     = { 0 };
     static team_s            team03                     = { 0 };
     static team_s            team04                     = { 0 };
     static team_s            team05                     = { 0 };
     static team_s            team06                     = { 0 };
     static team_s            team07                     = { 0 };
     static team_s            team08                     = { 0 };
     static team_s            team09                     = { 0 };
     static team_s            team10                     = { 0 };
     static team_s            team11                     = { 0 };
     static team_s            team12                     = { 0 };
     static team_s            team13                     = { 0 };
     static team_s            team14                     = { 0 };
     static team_s            team15                     = { 0 };
     static team_s            team16                     = { 0 };
     static team_s            team17                     = { 0 };
     static team_s            team18                     = { 0 };
     static team_s            team19                     = { 0 };
     static team_s            team20                     = { 0 };
     static team_s            team21                     = { 0 };
     static team_s            team22                     = { 0 };
     static team_s            team23                     = { 0 };
     static team_s            team24                     = { 0 };
     static team_s            team25                     = { 0 };
     static team_s            team26                     = { 0 };
     static team_s            team27                     = { 0 };
     static team_s            team28                     = { 0 };
     static team_s            team29                     = { 0 };
     static team_s            team30                     = { 0 };
     static team_s            team31                     = { 0 };
     static team_s            team32                     = { 0 };

     buildIntoLeague( &league1, 1 );
     buildIntoLeague( &league2, 2 );
     buildIntoLeagueDivision( &league_divisions1[0], 1, 1 );
     buildIntoLeagueDivision( &league_divisions1[1], 1, 2 );
     buildIntoLeagueDivision( &league_divisions2[0], 2, 3 );
     buildIntoLeagueDivision( &league_divisions2[1], 2, 4 );
     buildIntoDivision( &division1, 1 );
     buildIntoDivision( &division2, 2 );
     buildIntoDivision( &division3, 3 );
     buildIntoDivision( &division4, 4 );
     buildIntoDivisionTeam( &division_teams1[0], 1,  1 );
     buildIntoDivisionTeam( &division_teams1[1], 1,  2 );
     buildIntoDivisionTeam( &division_teams1[2], 1,  3 );
     buildIntoDivisionTeam( &division_teams1[3], 1,  4 );
     buildIntoDivisionTeam( &division_teams1[4], 1,  5 );
     buildIntoDivisionTeam( &division_teams1[5], 1,  6 );
     buildIntoDivisionTeam( &division_teams1[6], 1,  7 );
     buildIntoDivisionTeam( &division_teams1[7], 1,  8 );
     buildIntoDivisionTeam( &division_teams2[0], 2,  9 );
     buildIntoDivisionTeam( &division_teams2[1], 2, 10 );
     buildIntoDivisionTeam( &division_teams2[2], 2, 11 );
     buildIntoDivisionTeam( &division_teams2[3], 2, 12 );
     buildIntoDivisionTeam( &division_teams2[4], 2, 13 );
     buildIntoDivisionTeam( &division_teams2[5], 2, 14 );
     buildIntoDivisionTeam( &division_teams2[6], 2, 15 );
     buildIntoDivisionTeam( &division_teams2[7], 2, 16 );
     buildIntoDivisionTeam( &division_teams3[0], 3, 17 );
     buildIntoDivisionTeam( &division_teams3[1], 3, 18 );
     buildIntoDivisionTeam( &division_teams3[2], 3, 19 );
     buildIntoDivisionTeam( &division_teams3[3], 3, 20 );
     buildIntoDivisionTeam( &division_teams3[4], 3, 21 );
     buildIntoDivisionTeam( &division_teams3[5], 3, 22 );
     buildIntoDivisionTeam( &division_teams3[6], 3, 23 );
     buildIntoDivisionTeam( &division_teams3[7], 3, 24 );
     buildIntoDivisionTeam( &division_teams4[0], 4, 25 );
     buildIntoDivisionTeam( &division_teams4[1], 4, 26 );
     buildIntoDivisionTeam( &division_teams4[2], 4, 27 );
     buildIntoDivisionTeam( &division_teams4[3], 4, 28 );
     buildIntoDivisionTeam( &division_teams4[4], 4, 29 );
     buildIntoDivisionTeam( &division_teams4[5], 4, 30 );
     buildIntoDivisionTeam( &division_teams4[6], 4, 31 );
     buildIntoDivisionTeam( &division_teams4[7], 4, 32 );
     buildIntoTeam( &team01,  1 );
     buildIntoTeam( &team02,  2 );
     buildIntoTeam( &team03,  3 );
     buildIntoTeam( &team04,  4 );
     buildIntoTeam( &team05,  5 );
     buildIntoTeam( &team06,  6 );
     buildIntoTeam( &team07,  7 );
     buildIntoTeam( &team08,  8 );
     buildIntoTeam( &team09,  9 );
     buildIntoTeam( &team10, 10 );
     buildIntoTeam( &team11, 11 );
     buildIntoTeam( &team12, 12 );
     buildIntoTeam( &team13, 13 );
     buildIntoTeam( &team14, 14 );
     buildIntoTeam( &team15, 15 );
     buildIntoTeam( &team16, 16 );
     buildIntoTeam( &team17, 17 );
     buildIntoTeam( &team18, 18 );
     buildIntoTeam( &team19, 19 );
     buildIntoTeam( &team20, 20 );
     buildIntoTeam( &team21, 21 );
     buildIntoTeam( &team22, 22 );
     buildIntoTeam( &team23, 23 );
     buildIntoTeam( &team24, 24 );
     buildIntoTeam( &team25, 25 );
     buildIntoTeam( &team26, 26 );
     buildIntoTeam( &team27, 27 );
     buildIntoTeam( &team28, 28 );
     buildIntoTeam( &team29, 29 );
     buildIntoTeam( &team30, 30 );
     buildIntoTeam( &team31, 31 );
     buildIntoTeam( &team32, 32 );

     division_teams1[0].team = &team01;
     division_teams1[1].team = &team02;
     division_teams1[2].team = &team03;
     division_teams1[3].team = &team04;
     division_teams1[4].team = &team05;
     division_teams1[5].team = &team06;
     division_teams1[6].team = &team07;
     division_teams1[7].team = &team08;
     division_teams1[8]      = division_team_sentinel;

     division_teams2[0].team = &team09;
     division_teams2[1].team = &team10;
     division_teams2[2].team = &team11;
     division_teams2[3].team = &team12;
     division_teams2[4].team = &team13;
     division_teams2[5].team = &team14;
     division_teams2[6].team = &team15;
     division_teams2[7].team = &team16;
     division_teams2[8]      = division_team_sentinel;

     division_teams3[0].team = &team17;
     division_teams3[1].team = &team18;
     division_teams3[2].team = &team19;
     division_teams3[3].team = &team20;
     division_teams3[4].team = &team21;
     division_teams3[5].team = &team22;
     division_teams3[6].team = &team23;
     division_teams3[7].team = &team24;
     division_teams3[8]      = division_team_sentinel;

     division_teams4[0].team = &team25;
     division_teams4[1].team = &team26;
     division_teams4[2].team = &team27;
     division_teams4[3].team = &team28;
     division_teams4[4].team = &team29;
     division_teams4[5].team = &team30;
     division_teams4[6].team = &team31;
     division_teams4[7].team = &team32;
     division_teams4[8]      = division_team_sentinel;

     division1.teams = division_teams1;
     division2.teams = division_teams2;
     division3.teams = division_teams3;
     division4.teams = division_teams4;

     league_divisions1[0].division = &division1;
     league_divisions1[1].division = &division2;
     league_divisions1[2]          =  league_division_sentinel;

     league_divisions2[0].division = &division3;
     league_divisions2[1].division = &division4;
     league_divisions2[2]          =  league_division_sentinel;

     league1.divisions = league_divisions1;
     league2.divisions = league_divisions2;

     org_leagues[0].league = &league1;
     org_leagues[1].league = &league2;
     org_leagues[2]        =  org_league_sentinel;

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
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameObject_GivenAnOrgObject, null );
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameWithLeagues_GivenAnOrgObject, null );
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameWithDivisions_GivenAnOrgObject, null );
     run_test( generateFileLeagName_ShouldReturnAFileLeagNameWithTeams_GivenAnOrgObject, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
