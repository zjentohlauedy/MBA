#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static fileleagname_s *buildFileLeagName( void )
{
     static fileleagname_s league_data = { 0 };

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          sprintf( league_data.leagues[i].name, "League%d", i + 1 );
     }

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          sprintf( league_data.divisions[i].name, "Division%d", i + 1 );
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          sprintf( league_data.teams[i].name,       "Team%d", i + 1 );
          /**/     league_data.teams[i].color [0] = i + 1;
          /**/     league_data.teams[i].wins  [0] = i + 1;
          /**/     league_data.teams[i].losses[0] = i + 1;
     }

     return &league_data;
}

static fileparks_s *buildFileParks( void )
{
     static fileparks_s parks_data = { 0 };

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          sprintf( parks_data.park_names[i].text, "Park%d", i + 1 );
     }

     return &parks_data;
}

static fileplayer_s *buildFilePlayers( void )
{
     static fileplayer_s players_data[TOTAL_PLAYERS] = { 0 };

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          sprintf( players_data[i].first_name, "First%d", i + 1 );
          sprintf( players_data[i].last_name,  "Last%d",  i + 1 );
     }

     return players_data;
}


static char *convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData()
{
     fileleagname_s *league_data  = buildFileLeagName();
     fileparks_s    *parks_data   = buildFileParks();
     fileplayer_s   *players_data = buildFilePlayers();

     org_s *org = convertOrg( league_data, parks_data, players_data );

     assertNotNull( org );

     org_league_s *leagues = org->leagues;

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

static char *convertOrg_ShouldReturnOrgWithDivisions_GivenLeagueFileData()
{
     fileleagname_s *league_data  = buildFileLeagName();
     fileparks_s    *parks_data   = buildFileParks();
     fileplayer_s   *players_data = buildFilePlayers();

     org_s *org = convertOrg( league_data, parks_data, players_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     league_division_s *league1_divisions = leagues[0].league->divisions;

     assertNotNull( league1_divisions );

     assertNotNull(                                   league1_divisions[0].division              );
     assertEqualsInt( 1,                              league1_divisions[0].league_id             );
     assertEqualsInt( 1,                              league1_divisions[0].division_id           );
     assertEqualsInt( 1,                              league1_divisions[0].division->division_id );
     assertEqualsStr( league_data->divisions[0].name, league1_divisions[0].division->name        );

     assertNotNull(                                   league1_divisions[1].division              );
     assertEqualsInt( 1,                              league1_divisions[1].league_id             );
     assertEqualsInt( 2,                              league1_divisions[1].division_id           );
     assertEqualsInt( 2,                              league1_divisions[1].division->division_id );
     assertEqualsStr( league_data->divisions[1].name, league1_divisions[1].division->name        );

     // check for sentinel

     league_division_s *league2_divisions = leagues[1].league->divisions;

     assertNotNull( league2_divisions );

     assertNotNull(                                   league2_divisions[0].division              );
     assertEqualsInt( 2,                              league2_divisions[0].league_id             );
     assertEqualsInt( 3,                              league2_divisions[0].division_id           );
     assertEqualsInt( 3,                              league2_divisions[0].division->division_id );
     assertEqualsStr( league_data->divisions[2].name, league2_divisions[0].division->name        );

     assertNotNull(                                   league2_divisions[1].division              );
     assertEqualsInt( 2,                              league2_divisions[1].league_id             );
     assertEqualsInt( 4,                              league2_divisions[1].division_id           );
     assertEqualsInt( 4,                              league2_divisions[1].division->division_id );
     assertEqualsStr( league_data->divisions[3].name, league2_divisions[1].division->name        );

     freeOrg( org );

     return NULL;
}

static char *convertOrg_ShouldReturnOrgWithTeams_GivenLeagueFileAndParksFileData()
{
     fileleagname_s *league_data  = buildFileLeagName();
     fileparks_s    *parks_data   = buildFileParks();
     fileplayer_s   *players_data = buildFilePlayers();

     org_s *org = convertOrg( league_data, parks_data, players_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     league_division_s *league1_divisions = leagues[0].league->divisions;

     assertNotNull( league1_divisions );

     division_s *division1 = league1_divisions[0].division;

     assertNotNull( division1 );

     division_team_s *division1_teams = division1->teams;

     assertNotNull( division1_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull(                                   division1_teams[i].team                  );
          assertEqualsInt( 1,                              division1_teams[i].division_id           );
          assertEqualsInt( i + 1,                          division1_teams[i].team_id               );
          assertEqualsInt( i + 1,                          division1_teams[i].team->team_id         );
          assertEqualsStr( league_data->teams[i].name,     division1_teams[i].team->name            );
          assertEqualsStr( parks_data->park_names[i].text, division1_teams[i].team->location        );
          assertEqualsInt( league_data->teams[i].color[0], division1_teams[i].team->primary_color   );
          assertEqualsInt( league_data->teams[i].color[0], division1_teams[i].team->secondary_color );
     }

     division_s *division2 = league1_divisions[1].division;

     assertNotNull( division2 );

     division_team_s *division2_teams = division2->teams;

     assertNotNull( division2_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull(                                       division2_teams[i].team                  );
          assertEqualsInt( 2,                                  division2_teams[i].division_id           );
          assertEqualsInt( i + 9,                              division2_teams[i].team_id               );
          assertEqualsInt( i + 9,                              division2_teams[i].team->team_id         );
          assertEqualsStr( league_data->teams[i + 8].name,     division2_teams[i].team->name            );
          assertEqualsStr( parks_data->park_names[i + 8].text, division2_teams[i].team->location        );
          assertEqualsInt( league_data->teams[i + 8].color[0], division2_teams[i].team->primary_color   );
          assertEqualsInt( league_data->teams[i + 8].color[0], division2_teams[i].team->secondary_color );
     }

     league_division_s *league2_divisions = leagues[1].league->divisions;

     assertNotNull( league2_divisions );

     division_s *division3 = league2_divisions[0].division;

     assertNotNull( division3 );

     division_team_s *division3_teams = division3->teams;

     assertNotNull( division3_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull(                                        division3_teams[i].team                  );
          assertEqualsInt( 3,                                   division3_teams[i].division_id           );
          assertEqualsInt( i + 17,                              division3_teams[i].team_id               );
          assertEqualsInt( i + 17,                              division3_teams[i].team->team_id         );
          assertEqualsStr( league_data->teams[i + 16].name,     division3_teams[i].team->name            );
          assertEqualsStr( parks_data->park_names[i + 16].text, division3_teams[i].team->location        );
          assertEqualsInt( league_data->teams[i + 16].color[0], division3_teams[i].team->primary_color   );
          assertEqualsInt( league_data->teams[i + 16].color[0], division3_teams[i].team->secondary_color );
     }

     division_s *division4 = league2_divisions[1].division;

     assertNotNull( division4 );

     division_team_s *division4_teams = division4->teams;

     assertNotNull( division4_teams );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          assertNotNull(                                        division4_teams[i].team                  );
          assertEqualsInt( 4,                                   division4_teams[i].division_id           );
          assertEqualsInt( i + 25,                              division4_teams[i].team_id               );
          assertEqualsInt( i + 25,                              division4_teams[i].team->team_id         );
          assertEqualsStr( league_data->teams[i + 24].name,     division4_teams[i].team->name            );
          assertEqualsStr( parks_data->park_names[i + 24].text, division4_teams[i].team->location        );
          assertEqualsInt( league_data->teams[i + 24].color[0], division4_teams[i].team->primary_color   );
          assertEqualsInt( league_data->teams[i + 24].color[0], division4_teams[i].team->secondary_color );
     }

     freeOrg( org );

     return NULL;
}

static char *convertOrg_ShouldReturnOrgWithPlayers_GivenPlayersFileData()
{
     fileleagname_s *league_data  = buildFileLeagName();
     fileparks_s    *parks_data   = buildFileParks();
     fileplayer_s   *players_data = buildFilePlayers();

     org_s *org = convertOrg( league_data, parks_data, players_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          league_s *league = leagues[i].league;

          assertNotNull( leagues[i].league );

          league_division_s *league_divisions = league->divisions;

          assertNotNull( league_divisions );

          for ( int j = 0; j < DIVISIONS_PER_LEAGUE; ++j )
          {
               division_s *division = league_divisions[j].division;

               assertNotNull( division );

               division_team_s *division_teams = division->teams;

               assertNotNull( division_teams );

               for ( int k = 0; k < TEAMS_PER_DIVISION; ++k )
               {
                    team_s *team = division_teams[k].team;

                    assertNotNull( team );

                    team_player_s *team_players = team->players;

                    assertNotNull( team_players );

                    for ( int l = 0; l < PLAYERS_PER_TEAM; ++l )
                    {
                         int idx = l + (k * PLAYERS_PER_TEAM) + (j * PLAYERS_PER_DIVISION) + (i * PLAYERS_PER_LEAGUE);

                         player_s *player = team_players[l].player;

                         assertNotNull( player );

                         assertEqualsInt( idx + 1,                      player->player_id  );
                         assertEqualsStr( players_data[idx].first_name, player->first_name );
                         assertEqualsStr( players_data[idx].last_name,  player->last_name  );
                    }
               }
          }
     }

     freeOrg( org );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData,           null );
     run_test( convertOrg_ShouldReturnOrgWithDivisions_GivenLeagueFileData,         null );
     run_test( convertOrg_ShouldReturnOrgWithTeams_GivenLeagueFileAndParksFileData, null );
     run_test( convertOrg_ShouldReturnOrgWithPlayers_GivenPlayersFileData,          null );

     // TODO:
     //   add the rest of the player data fields, including player id in fileplayers data
     //   deal with the player name terminators (and maybe phoens too)
     //   add support for seasons
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
