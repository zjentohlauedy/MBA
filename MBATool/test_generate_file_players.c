#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "unit_test.h"


static char *result;


static org_s *buildOrg( void )
{
     static org_s             org                                                                                                      = { 0 };
     static league_s          leagues                  [TOTAL_LEAGUES    ]                                                             = { 0 };
     static org_league_s      org_leagues              [TOTAL_LEAGUES + 1]                                                             = { 0 };
     static org_league_s      org_league_sentinel                                                                                      = ORG_LEAGUE_SENTINEL;
     static division_s        divisions                [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE    ]                                       = { 0 };
     static league_division_s league_divisions         [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE + 1]                                       = { 0 };
     static league_division_s league_division_sentinel                                                                                 = LEAGUE_DIVISION_SENTINEL;
     static team_s            teams                    [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE][TEAMS_PER_DIVISION    ]                   = { 0 };
     static division_team_s   division_teams           [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE][TEAMS_PER_DIVISION + 1]                   = { 0 };
     static division_team_s   division_team_sentinel                                                                                   = DIVISION_TEAM_SENTINEL;
     static player_s          players                  [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE][TEAMS_PER_DIVISION][PLAYERS_PER_TEAM    ] = { 0 };
     static team_player_s     team_players             [TOTAL_LEAGUES][DIVISIONS_PER_LEAGUE][TEAMS_PER_DIVISION][PLAYERS_PER_TEAM + 1] = { 0 };
     static team_player_s     team_player_sentinel                                                                                     = TEAM_PLAYER_SENTINEL;

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

                    for ( int l = 0; l < PLAYERS_PER_TEAM; ++l )
                    {
                         int player_id = (i * PLAYERS_PER_LEAGUE) + (j * PLAYERS_PER_DIVISION) + (k * PLAYERS_PER_TEAM) + l + 1;

                         buildIntoPlayer( &players[i][j][k][l], player_id );
                         buildIntoTeamPlayer( &team_players[i][j][k][l], team_id, 0, player_id );

                         team_players[i][j][k][l].player = &players[i][j][k][l];
                    }

                    team_players[i][j][k][PLAYERS_PER_TEAM] = team_player_sentinel;

                    teams[i][j][k].players = team_players[i][j][k];

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


static char *generateFilePlayers_ShouldReturnAFilePlayersObject_GivenAnOrgObject()
{
     org_s org = { 0 };

     fileplayer_s *players_file = generateFilePlayers( &org );

     assertNotNull( players_file );

     free( players_file );

     return NULL;
}

static char *generateFilePlayers_ShouldReturnAFilePlayersWithPlayers_GivenAnOrgObject()
{
     org_s *org = buildOrg();

     fileplayer_s *players_file = generateFilePlayers( org );

     assertNotNull( players_file );

     for ( int i = 0; org->leagues[i].league != NULL; ++i )
     {
          for ( int j = 0; org->leagues[i].league->divisions[j].division != NULL; ++j )
          {
               for ( int k = 0; org->leagues[i].league->divisions[j].division->teams[k].team != NULL; ++k )
               {
                    team_player_s *team_players = org->leagues[i].league->divisions[j].division->teams[k].team->players;

                    for ( int l = 0; team_players[l].player != NULL; ++l )
                    {
                         int idx = (i * PLAYERS_PER_LEAGUE) + (j * PLAYERS_PER_DIVISION) + (k * PLAYERS_PER_TEAM) + l;

                         char first_name  [sizeof(players_file[idx].first_name) ] = { 0 };
                         char last_name   [sizeof(players_file[idx].last_name)  ] = { 0 };
                         char first_phoen [sizeof(players_file[idx].first_phoen)] = { 0 };
                         char last_phoen  [sizeof(players_file[idx].last_phoen) ] = { 0 };

                         memcpy( first_name,  team_players[l].player->first_name,      sizeof(first_name)  );
                         memcpy( last_name,   team_players[l].player->last_name,       sizeof(last_name)   );
                         memcpy( first_phoen, team_players[l].player->first_phoenetic, sizeof(first_phoen) );
                         memcpy( last_phoen,  team_players[l].player->last_phoenetic,  sizeof(last_phoen)  );

                         termName( first_name,  sizeof(first_name)  );
                         termName( last_name,   sizeof(last_name)   );
                         termName( first_phoen, sizeof(first_phoen) );
                         termName( last_phoen,  sizeof(last_phoen)  );

                         assertEqualsBfr( first_name,  players_file[idx].first_name,  sizeof(first_name)  );
                         assertEqualsBfr( last_name,   players_file[idx].last_name,   sizeof(last_name)   );
                         assertEqualsBfr( first_phoen, players_file[idx].first_phoen, sizeof(first_phoen) );
                         assertEqualsBfr( last_phoen,  players_file[idx].last_phoen,  sizeof(last_phoen)  );

                         assertEquals( team_players[l].player->player_id,     word2int( players_file[idx].acc_stats.action.id_info.player_id ) );
                         assertEquals( team_players[l].player->rookie_season, byte2int( players_file[idx].year ) - YEAR_SEASON_OFFSET );

                         if ( team_players[l].player->player_type == pt_Pitcher )
                         {
                              filehand_e  handedness = nibble( players_file[idx].position[0], n_Low );
                              filecolor_e color      = byte2int( players_file[idx].filestats.filepitching.color );

                              if   ( team_players[l].player->handedness == hnd_Right ) assertEquals( fh_Right, handedness );
                              else                                                     assertEquals( fh_Left,  handedness );

                              if   ( team_players[l].player->skin_tone == st_Dark ) assertEquals( fc_Dark,  color );
                              else                                                  assertEquals( fc_Light, color );
                         }
                         else
                         {
                              filehand_e  handedness = nibble( players_file[idx].filestats.filebatting.ratings[0], n_High );
                              filecolor_e color      = byte2int( players_file[idx].filestats.filebatting.color );

                              if   ( team_players[l].player->handedness == hnd_Right ) assertEquals( fh_Right, handedness );
                              else                                                     assertEquals( fh_Left,  handedness );

                              if   ( team_players[l].player->skin_tone == st_Dark ) assertEquals( fc_Dark,  color );
                              else                                                  assertEquals( fc_Light, color );
                         }
                    }
               }
          }
     }

     free( players_file );

     return NULL;
}

static void run_all_tests()
{
     run_test( generateFilePlayers_ShouldReturnAFilePlayersObject_GivenAnOrgObject,      null );
     run_test( generateFilePlayers_ShouldReturnAFilePlayersWithPlayers_GivenAnOrgObject, null );
     // longevity ratings modification
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
