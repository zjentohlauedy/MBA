#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "builders.h"
#include "unit_test.h"


static char *result;


static fileposition_e mapPosition( const position_e position )
{
     switch ( position )
     {
     case pos_Catcher:     return fpos_Catcher;
     case pos_FirstBase:   return fpos_FirstBaseman;
     case pos_SecondBase:  return fpos_SecondBaseman;
     case pos_ThirdBase:   return fpos_ThirdBaseman;
     case pos_ShortStop:   return fpos_ShortStop;
     case pos_LeftField:   return fpos_LeftField;
     case pos_CenterField: return fpos_CenterField;
     case pos_RightField:  return fpos_RightField;
     case pos_Infield:     return fpos_Infield;
     case pos_Outfield:    return fpos_Outfield;
     }

     return fpos_DesignatedHitter;
}

static filehand_e mapHandedness( const handedness_e handedness )
{
     switch ( handedness )
     {
     case hnd_Right:  return fh_Right;
     case hnd_Left:   return fh_Left;
     case hnd_Switch: return fh_Switch;
     }

     return hnd_Right;
}

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

                         if   ( players[i][j][k][l].player_type == pt_Pitcher ) players[i][j][k][l].details.pitching = buildPitcher( player_id );
                         else                                                   players[i][j][k][l].details.batting  = buildBatter(  player_id );

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

     fileplayer_s *players_file = generateFilePlayers( &org, 0 );

     assertNotNull( players_file );

     free( players_file );

     return NULL;
}

static char *generateFilePlayers_ShouldReturnAFilePlayersWithPlayers_GivenAnOrgObject()
{
     org_s *org    = buildOrg();
     int    season = 13;

     fileplayer_s *players_file = generateFilePlayers( org, season );

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

                         assertEquals(               team_players[l].player->player_id,   word2int( players_file[idx].acc_stats.action.id_info.player_id ) );
                         assertEquals( calcChecksum( team_players[l].player->player_id ), byte2int( players_file[idx].acc_stats.action.id_info.checksum  ) );

                         assertEquals( team_players[l].player->rookie_season, byte2int( players_file[idx].year ) - YEAR_SEASON_OFFSET );

                         int age_adjustment = (team_players[l].player->longevity + 3) - (season - team_players[l].player->rookie_season);

                         if ( team_players[l].player->player_type == pt_Pitcher )
                         {
                              pitcher_s      *pitcher  = team_players[l].player->details.pitching;
                              filepitching_s *pitching = &(players_file[idx].filestats.filepitching);

                              if ( pitcher->fatigue > 1 ) {
                                   filehand_e  handedness = mapHandedness( team_players[l].player->handedness );
                                   filecolor_e color      = byte2int( pitching->color );

                                   int speed   = (age_adjustment >= 0) ? pitcher->speed   : MAX( pitcher->speed   + age_adjustment, 1 );
                                   int control = (age_adjustment >= 0) ? pitcher->control : MAX( pitcher->control + age_adjustment, 1 );
                                   int fatigue = (age_adjustment >= 0) ? pitcher->fatigue : MAX( pitcher->fatigue + age_adjustment, 4 );

                                   assertEquals( fpos_Pitcher, nibble( players_file[idx].position[0], n_High ) );
                                   assertEquals( handedness,   nibble( players_file[idx].position[0], n_Low  ) );

                                   assertEquals(                         speed,     nibble( pitching->ratings[0], n_High ) );
                                   assertEquals(                         control,   nibble( pitching->ratings[0], n_Low  ) );
                                   assertEquals(                         fatigue,   nibble( pitching->ratings[1], n_High ) );
                                   assertEquals( team_players[l].player->longevity, nibble( pitching->ratings[1], n_Low  ) );

                                   if   ( team_players[l].player->skin_tone == st_Dark ) assertEquals( fc_Dark,  color );
                                   else                                                  assertEquals( fc_Light, color );

                                   assertNonZero( word2int( players_file[idx].real_avg ) );

                                   assertNonZero( byte2int( pitching->real_wins   ) );
                                   assertNonZero( byte2int( pitching->real_losses ) );
                                   assertNonZero( byte2int( pitching->real_games  ) );
                                   assertNonZero( byte2int( pitching->real_starts ) );
                                   assertNonZero( word2int( pitching->real_inn    ) );
                                   assertNonZero( byte2int( pitching->real_er     ) );
                                   assertNonZero( byte2int( pitching->real_fa     ) );
                                   assertNonZero( word2int( pitching->vl_ab       ) );
                                   assertNonZero( word2int( pitching->vl_hits     ) );
                                   assertNonZero( byte2int( pitching->vl_hr       ) );
                                   assertNonZero( byte2int( pitching->vl_bb       ) );
                                   assertNonZero( word2int( pitching->vl_so       ) );

                                   assertEquals( 0, byte2int( pitching->real_saves  ) );
                                   assertEquals( 0, word2int( pitching->vr_ab       ) );
                                   assertEquals( 0, word2int( pitching->vr_hits     ) );
                                   assertEquals( 0, byte2int( pitching->vr_hr       ) );
                                   assertEquals( 0, byte2int( pitching->vr_bb       ) );
                                   assertEquals( 0, word2int( pitching->vr_so       ) );
                                   assertEquals( 0, byte2int( pitching->real_hb     ) );
                                   assertEquals( 0, byte2int( pitching->real_balk   ) );
                                   assertEquals( 0, byte2int( pitching->real_ab     ) );
                                   assertEquals( 0, byte2int( pitching->real_hits   ) );
                                   assertEquals( 0, byte2int( pitching->real_hr     ) );
                                   assertEquals( 0, byte2int( pitching->real_sh     ) );
                                   assertEquals( 0, byte2int( pitching->injury_days ) );

                                   // this could be better...
                                   if      ( fatigue >= 8 ) assertNonZero(    byte2int( pitching->real_cg ) );
                                   else if ( fatigue == 4 ) assertEquals(  0, byte2int( pitching->real_cg ) );
                              }
                              else
                              {
                                   filehand_e  handedness = mapHandedness( team_players[l].player->handedness );
                                   filecolor_e color      = byte2int( pitching->color );

                                   int speed   = (age_adjustment >= 0) ? pitcher->speed   : MAX( pitcher->speed   + age_adjustment, 1 );
                                   int control = (age_adjustment >= 0) ? pitcher->control : MAX( pitcher->control + age_adjustment, 1 );
                                   int fatigue = 1;

                                   assertEquals( fpos_Pitcher, nibble( players_file[idx].position[0], n_High ) );
                                   assertEquals( handedness,   nibble( players_file[idx].position[0], n_Low  ) );

                                   assertEquals(                         speed,     nibble( pitching->ratings[0], n_High ) );
                                   assertEquals(                         control,   nibble( pitching->ratings[0], n_Low  ) );
                                   assertEquals(                         fatigue,   nibble( pitching->ratings[1], n_High ) );
                                   assertEquals( team_players[l].player->longevity, nibble( pitching->ratings[1], n_Low  ) );

                                   if   ( team_players[l].player->skin_tone == st_Dark ) assertEquals( fc_Dark,  color );
                                   else                                                  assertEquals( fc_Light, color );

                                   assertBetween( 0, 10, byte2int( pitching->real_wins   ) );
                                   assertBetween( 0, 10, byte2int( pitching->real_losses ) );

                                   assertNonZero( word2int( players_file[idx].real_avg ) );
                                   assertNonZero( byte2int( pitching->real_saves  ) );
                                   assertNonZero( byte2int( pitching->real_games  ) );
                                   assertNonZero( word2int( pitching->real_inn    ) );
                                   assertNonZero( byte2int( pitching->real_er     ) );
                                   assertNonZero( byte2int( pitching->real_fa     ) );
                                   assertNonZero( word2int( pitching->vl_ab       ) );
                                   assertNonZero( word2int( pitching->vl_hits     ) );
                                   assertNonZero( byte2int( pitching->vl_hr       ) );
                                   assertNonZero( byte2int( pitching->vl_bb       ) );
                                   assertNonZero( word2int( pitching->vl_so       ) );

                                   assertEquals( 0, byte2int( pitching->real_starts ) );
                                   assertEquals( 0, word2int( pitching->vr_ab       ) );
                                   assertEquals( 0, word2int( pitching->vr_hits     ) );
                                   assertEquals( 0, byte2int( pitching->vr_hr       ) );
                                   assertEquals( 0, byte2int( pitching->vr_bb       ) );
                                   assertEquals( 0, word2int( pitching->vr_so       ) );
                                   assertEquals( 0, byte2int( pitching->real_hb     ) );
                                   assertEquals( 0, byte2int( pitching->real_balk   ) );
                                   assertEquals( 0, byte2int( pitching->real_ab     ) );
                                   assertEquals( 0, byte2int( pitching->real_hits   ) );
                                   assertEquals( 0, byte2int( pitching->real_hr     ) );
                                   assertEquals( 0, byte2int( pitching->real_sh     ) );
                                   assertEquals( 0, byte2int( pitching->injury_days ) );
                                   assertEquals( 0, byte2int( pitching->real_cg     ) );
                              }
                         }
                         else
                         {
                              batter_s      *batter  = team_players[l].player->details.batting;
                              filebatting_s *batting = &(players_file[idx].filestats.filebatting);

                              filehand_e     handedness    = mapHandedness( team_players[l].player->handedness );
                              filecolor_e    color         = byte2int( players_file[idx].filestats.filebatting.color );
                              fileposition_e primary_pos   = mapPosition( batter->primary_position   );
                              fileposition_e secondary_pos = mapPosition( batter->secondary_position );

                              int arm       = (age_adjustment >= 0) ? batter->arm       : MAX( batter->arm       + age_adjustment, 1 );
                              int running   = (age_adjustment >= 0) ? batter->running   : MAX( batter->running   + age_adjustment, 1 );
                              int range     = (age_adjustment >= 0) ? batter->range     : MAX( batter->range     + age_adjustment, 1 );
                              int power     = (age_adjustment >= 0) ? batter->power     : MAX( batter->power     + age_adjustment, 1 );
                              int bunt      = (age_adjustment >= 0) ? batter->bunt      : MAX( batter->bunt      + age_adjustment, 1 );
                              int hit_n_run = (age_adjustment >= 0) ? batter->hit_n_run : MAX( batter->hit_n_run + age_adjustment, 1 );

                              assertEquals( primary_pos,   nibble( players_file[idx].position[0], n_High ) );
                              assertEquals( secondary_pos, nibble( players_file[idx].position[0], n_Low  ) );

                              assertEquals(                         handedness, nibble( batting->ratings[0], n_High ) );
                              assertEquals(                         arm,        nibble( batting->ratings[0], n_Low  ) );
                              assertEquals(                         running,    nibble( batting->ratings[1], n_High ) );
                              assertEquals(                         range,      nibble( batting->ratings[1], n_Low  ) );
                              assertEquals(                         power,      nibble( batting->ratings[2], n_High ) );
                              assertEquals( team_players[l].player->longevity,  nibble( batting->ratings[2], n_Low  ) );
                              assertEquals(                         bunt,       nibble( batting->ratings[3], n_High ) );
                              assertEquals(                         hit_n_run,  nibble( batting->ratings[3], n_Low  ) );

                              if   ( team_players[l].player->skin_tone == st_Dark ) assertEquals( fc_Dark,  color );
                              else                                                  assertEquals( fc_Light, color );

                              assertNonZero( word2int( players_file[idx].real_avg ) );

                              assertNonZero( byte2int( batting->real_games  ) );
                              assertNonZero( byte2int( batting->real_runs   ) );
                              assertNonZero( byte2int( batting->real_rbi    ) );

                              // can be zero but not always
                              // assertNonZero( byte2int( batting->real_err    ) );
                              assertNonZero( word2int( batting->real_po     ) );

                              // could be better...
                              if ( primary_pos <= fpos_ShortStop ) assertNonZero( word2int( batting->real_as ) );

                              assertNonZero( byte2int( batting->real_fa2    ) );

                              assertNonZero( word2int( batting->vl_ab       ) );
                              assertNonZero( word2int( batting->vl_hits     ) );
                              assertNonZero( byte2int( batting->vl_2b       ) );

                              // figure out how to test this:
                              //assertNonZero( byte2int( batting->vl_3b       ) );

                              if ( power >= 3 ) assertNonZero( byte2int( batting->vl_hr ) );

                              assertNonZero( byte2int( batting->vl_bb       ) );
                              assertNonZero( byte2int( batting->vl_so       ) );

                              assertEquals( 0, byte2int( batting->real_cs     ) );
                              assertEquals( 0, byte2int( batting->real_sh     ) );
                              assertEquals( 0, word2int( batting->vr_ab       ) );
                              assertEquals( 0, word2int( batting->vr_hits     ) );
                              assertEquals( 0, byte2int( batting->vr_2b       ) );
                              assertEquals( 0, byte2int( batting->vr_3b       ) );
                              assertEquals( 0, byte2int( batting->vr_hr       ) );
                              assertEquals( 0, byte2int( batting->vr_bb       ) );
                              assertEquals( 0, byte2int( batting->vr_so       ) );
                              assertEquals( 0, byte2int( batting->injury_days ) );

                              if      ( running >= 7 ) assertNonZero(    byte2int( batting->real_sb ) );
                              else if ( running <= 4 ) assertEquals(  0, byte2int( batting->real_sb ) );
                         }
                    }
               }
          }
     }

     free( players_file );

     return NULL;
}

static char *adjustRating_ShouldReturnSameValue_GivenAPlayerThatIsntAgeing()
{
     player_s player1 = { 0 };
     player_s player2 = { 0 };
     int      season  =  10;

     player1.rookie_season = season;
     player1.longevity     = 5;

     player2.rookie_season = 2;
     player2.longevity     = 5;

     assertEquals( 7, adjustRating( 7, season, &player1 ) );
     assertEquals( 7, adjustRating( 7, season, &player2 ) );

     return NULL;
}

static char *adjustRating_ShouldReturnIncrementallyReducedValue_GivenAPlayerThatIsAgeing()
{
     player_s player1 = { 0 };
     player_s player2 = { 0 };
     player_s player3 = { 0 };
     int      season  =  10;

     player1.rookie_season = season;
     player1.longevity     = 5;

     player2.rookie_season = 1;
     player2.longevity     = 5;

     player3.rookie_season = 1;
     player3.longevity     = 4;

     assertEquals( 7, adjustRating( 7, season, &player1 ) );
     assertEquals( 6, adjustRating( 7, season, &player2 ) );
     assertEquals( 5, adjustRating( 7, season, &player3 ) );

     return NULL;
}

static char *adjustRating_ShouldNeverReturnLessThanOne()
{
     player_s player1 = { 0 };
     player_s player2 = { 0 };
     player_s player3 = { 0 };
     int      season  =  15;

     player1.rookie_season = 1;
     player1.longevity     = 10;

     player2.rookie_season = 1;
     player2.longevity     = 1;

     player3.rookie_season = 1;
     player3.longevity     = 5;

     assertEquals( 1, adjustRating( 1, season, &player1 ) );
     assertEquals( 1, adjustRating( 1, season, &player2 ) );
     assertEquals( 1, adjustRating( 1, season, &player3 ) );

     return NULL;
}

static void run_all_tests()
{
     run_test( generateFilePlayers_ShouldReturnAFilePlayersObject_GivenAnOrgObject,      null );
     run_test( generateFilePlayers_ShouldReturnAFilePlayersWithPlayers_GivenAnOrgObject, null );

     run_test( adjustRating_ShouldReturnSameValue_GivenAPlayerThatIsntAgeing, null );
     run_test( adjustRating_ShouldReturnIncrementallyReducedValue_GivenAPlayerThatIsAgeing, null );
     run_test( adjustRating_ShouldNeverReturnLessThanOne, null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
