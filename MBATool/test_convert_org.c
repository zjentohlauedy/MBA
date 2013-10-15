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

static fileposition_e pickPosition( void )
{
     struct
     {
          fileposition_e pos;
          int            weight;

     } possibilities[] = { { fpos_Pitcher,       288 },
                           { fpos_Catcher,        54 },
                           { fpos_FirstBaseman,   54 },
                           { fpos_SecondBaseman,  54 },
                           { fpos_ThirdBaseman,   54 },
                           { fpos_ShortStop,      54 },
                           { fpos_LeftField,      54 },
                           { fpos_CenterField,    54 },
                           { fpos_RightField,     54 }
     };

     const static int total = 720;

     int x = (rand() % total) + 1;

     for ( int i = 0; i < 8; ++i )
     {
          if ( (x -= possibilities[i].weight) <= 0 ) return possibilities[i].pos;
     }

     return fpos_Pitcher;
}

static unsigned char calcChecksum( int value )
{
     unsigned char checksum = 0;

     while ( value > 0 )
     {
          checksum += value % 10;

          value /= 10;
     }

     return checksum;
}

static fileplayer_s *buildFilePlayers( void )
{
     static fileplayer_s players_data[TOTAL_PLAYERS] = { 0 };

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
          unsigned char pos       = pickPosition();
          unsigned char hand      = ((rand() % 100)) > 49 ? fh_Right : fh_Left;
          unsigned char color     = ((rand() % 100)) > 49 ? fc_Light : fc_Dark;
          unsigned char longevity = (rand() % 10) + 1;
          unsigned char position;
          int           year      = YEAR_SEASON_OFFSET + (i / 100);
          int           player_id = (rand() % 50000) + 1;

          if ( pos == fpos_Pitcher )
          {
               struct pitching_s *pitching = &(players_data[i].filestats.filepitching);

               unsigned char speed     = (rand() % 10) + 1;
               unsigned char control   = (rand() % 10) + 1;
               unsigned char fatigue   = (rand() % 10) + 1;

               position = (pos << 4) + hand;

               pitching->color[0]   = color;
               pitching->ratings[0] = (speed   << 4) + control;
               pitching->ratings[1] = (fatigue << 4) + longevity;

               acc_player_id_s *player_id_data = (acc_player_id_s *)&(pitching->action);

               int2word( player_id_data->player_id,                  player_id );
               /**/      player_id_data->checksum[0] = calcChecksum( player_id );
          }
          else
          {
               struct batting_s *batting = &(players_data[i].filestats.filebatting);

               unsigned char pos2      = pickPosition();
               unsigned char power     = (rand() % 10) + 1;
               unsigned char hit_n_run = (rand() % 10) + 1;
               unsigned char bunt      = (rand() % 10) + 1;
               unsigned char running   = (rand() % 10) + 1;
               unsigned char range     = (rand() % 10) + 1;
               unsigned char arm       = (rand() % 10) + 1;

               position = (pos << 4) + pos2;

               batting->color[0]   = color;
               batting->ratings[0] = (hand    << 4) + arm;
               batting->ratings[1] = (running << 4) + range;
               batting->ratings[2] = (power   << 4) + longevity;
               batting->ratings[3] = (bunt    << 4) + hit_n_run;

               acc_player_id_s *player_id_data = (acc_player_id_s *)&(batting->action);

               int2word( player_id_data->player_id,                  player_id );
               /**/      player_id_data->checksum[0] = calcChecksum( player_id );
          }

          sprintf( players_data[i].first_name,   "Fst%d",    i + 1 );
          sprintf( players_data[i].last_name,    "Last%d",   i + 1 );
          sprintf( players_data[i].first_phoen,  "FIR3ST%d", i + 1 );
          sprintf( players_data[i].last_phoen,   "LAH4ST%d", i + 1 );
          /**/     players_data[i].year[0]      = year;
          /**/     players_data[i].position[0]  = position;
     }

     return players_data;
}


static char *convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

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

static char *convertOrg_ShouldReturnOrgWithDivisions_GivenLeagueFileData()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     org_s *org = convertOrg( &org_data );

     fileleagname_s *league_data = org_data.league_data;
     org_league_s   *leagues     = org->leagues;

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
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     org_s *org = convertOrg( &org_data );

     fileleagname_s *league_data = org_data.league_data;
     fileparks_s    *parks_data  = org_data.parks_data;
     org_league_s   *leagues     = org->leagues;

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
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.season       = 13;

     org_s *org = convertOrg( &org_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          league_s *league = leagues[i].league;

          assertNotNull( league );

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
                         int idx     = l + (k * PLAYERS_PER_TEAM) + (j * PLAYERS_PER_DIVISION) + (i * PLAYERS_PER_LEAGUE);
                         int team_id = 1 + k + (j * TEAMS_PER_DIVISION) + (i * TEAMS_PER_LEAGUE);

                         fileplayer_s *players_data = org_data.players_data;
                         player_s     *player       = team_players[l].player;

                         assertNotNull( player );

                         int            rookie_season = players_data[idx].year[0] - YEAR_SEASON_OFFSET;
                         fileposition_e position      = nibble( players_data[idx].position[0], n_High );
                         filehand_e     hand;
                         filecolor_e    color;
                         int            player_id;

                         if ( position == fpos_Pitcher )
                         {
                              struct pitching_s *pitching = &(players_data[idx].filestats.filepitching);
                              acc_player_id_s   *id_info  = (acc_player_id_s *)&(pitching->action);

                              assertEqualsInt( pt_Pitcher, player->player_type );

                              pitcher_s *pitcher = player->details.pitching;

                              assertNotNull( pitcher );

                              int speed     = nibble(   pitching->ratings[0],          n_High );
                              int control   = nibble(   pitching->ratings[0],          n_Low  );
                              int fatigue   = nibble(   pitching->ratings[1],          n_High );
                              int longevity = nibble(   pitching->ratings[1],          n_Low  );
                              /**/color     =           pitching->color[0];
                              /**/hand      = nibble(   players_data[idx].position[0], n_Low );
                              /**/player_id = word2int( id_info->player_id                   );

                              assertEqualsInt( player_id, pitcher->player_id );
                              assertEqualsInt( speed,     pitcher->speed     );
                              assertEqualsInt( control,   pitcher->control   );
                              assertEqualsInt( longevity, pitcher->bunt      );
                              assertEqualsInt( fatigue,   pitcher->fatigue   );
                              assertEqualsInt( longevity, player->longevity  );
                         }
                         else
                         {
                              struct batting_s *batting = &(players_data[idx].filestats.filebatting);
                              acc_player_id_s  *id_info = (acc_player_id_s *)&(batting->action);

                              assertEqualsInt( pt_Batter, player->player_type );

                              batter_s *batter = player->details.batting;

                              assertNotNull( batter );

                              int pos1      = nibble( players_data[idx].position[0], n_High );
                              int pos2      = nibble( players_data[idx].position[0], n_Low  );

                              int power     = nibble(   batting->ratings[2], n_High );
                              int hit_n_run = nibble(   batting->ratings[3], n_Low  );
                              int bunt      = nibble(   batting->ratings[3], n_High );
                              int running   = nibble(   batting->ratings[1], n_High );
                              int range     = nibble(   batting->ratings[1], n_Low  );
                              int arm       = nibble(   batting->ratings[0], n_Low  );
                              int longevity = nibble(   batting->ratings[2], n_Low  );
                              /**/hand      = nibble(   batting->ratings[0], n_High );
                              /**/color     =           batting->color[0];
                              /**/player_id = word2int( id_info->player_id          );

                              switch ( pos1 )
                              {
                              case fpos_Catcher:         assertEqualsInt( pos_Catcher,     batter->primary_position ); break;
                              case fpos_FirstBaseman:    assertEqualsInt( pos_FirstBase,   batter->primary_position ); break;
                              case fpos_SecondBaseman:   assertEqualsInt( pos_SecondBase,  batter->primary_position ); break;
                              case fpos_ThirdBaseman:    assertEqualsInt( pos_ThirdBase,   batter->primary_position ); break;
                              case fpos_ShortStop:       assertEqualsInt( pos_ShortStop,   batter->primary_position ); break;
                              case fpos_LeftField:       assertEqualsInt( pos_LeftField,   batter->primary_position ); break;
                              case fpos_CenterField:     assertEqualsInt( pos_CenterField, batter->primary_position ); break;
                              case fpos_RightField:      assertEqualsInt( pos_RightField,  batter->primary_position ); break;
                              }

                              switch ( pos2 )
                              {
                              case fpos_Catcher:         assertEqualsInt( pos_Catcher,     batter->secondary_position ); break;
                              case fpos_FirstBaseman:    assertEqualsInt( pos_FirstBase,   batter->secondary_position ); break;
                              case fpos_SecondBaseman:   assertEqualsInt( pos_SecondBase,  batter->secondary_position ); break;
                              case fpos_ThirdBaseman:    assertEqualsInt( pos_ThirdBase,   batter->secondary_position ); break;
                              case fpos_ShortStop:       assertEqualsInt( pos_ShortStop,   batter->secondary_position ); break;
                              case fpos_LeftField:       assertEqualsInt( pos_LeftField,   batter->secondary_position ); break;
                              case fpos_CenterField:     assertEqualsInt( pos_CenterField, batter->secondary_position ); break;
                              case fpos_RightField:      assertEqualsInt( pos_RightField,  batter->secondary_position ); break;
                              }

                              assertEqualsInt( player_id,     batter->player_id          );
                              assertEqualsInt( power,         batter->power              );
                              assertEqualsInt( hit_n_run,     batter->hit_n_run          );
                              assertEqualsInt( bunt,          batter->bunt               );
                              assertEqualsInt( running,       batter->running            );
                              assertEqualsInt( range,         batter->range              );
                              assertEqualsInt( arm,           batter->arm                );
                              assertEqualsInt( longevity,     player->longevity          );
                         }

                         assertEqualsInt( team_id,         team_players[l].team_id );
                         assertEqualsInt( org_data.season, team_players[l].season  );

                         assertEqualsInt( player_id,                       player->player_id      );
                         assertEqualsStr( players_data[idx].first_name,    player->first_name     );
                         assertEqualsStr( players_data[idx].last_name,     player->last_name      );
                         assertEqualsStr( players_data[idx].first_phoen,   player->first_phonetic );
                         assertEqualsStr( players_data[idx].last_phoen,    player->last_phonetic  );
                         assertEqualsInt(                   rookie_season, player->rookie_season  );

                         if   ( hand == fh_Right ) assertEqualsInt( hnd_Right, player->handedness );
                         else                      assertEqualsInt( hnd_Left,  player->handedness );

                         if   ( color == fc_Light ) assertEqualsInt( st_Light, player->skin_tone );
                         else                       assertEqualsInt( st_Dark,  player->skin_tone );
                    }
               }
          }
     }

     freeOrg( org );

     return NULL;
}

static char *convertOrg_ShouldRemoveTerminatorsOnPlayerNamesAndPhoenetics_GivenPlayersFileData()
{
     org_data_s  org_data    = { 0 };
     char       *first_name  = "FULLBUFF";
     char       *last_name   = "FULLBUFFER";
     char       *first_phoen = "FULLBUFFERTO";
     char       *last_phoen  = "FULLBUFFERALSO";


     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     fileplayer_s *players_data = org_data.players_data;

     memcpy( players_data[0].first_name,  first_name,  strlen(first_name)  );
     memcpy( players_data[0].last_name,   last_name,   strlen(last_name)   );
     memcpy( players_data[0].first_phoen, first_phoen, strlen(first_phoen) );
     memcpy( players_data[0].last_phoen,  last_phoen,  strlen(last_phoen)  );

     termName( players_data[0].first_name,  sizeof(players_data[0].first_name)  );
     termName( players_data[0].last_name,   sizeof(players_data[0].last_name)   );
     termName( players_data[0].first_phoen, sizeof(players_data[0].first_phoen) );
     termName( players_data[0].last_phoen,  sizeof(players_data[0].last_phoen)  );

     org_s *org = convertOrg( &org_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     league_s *league = leagues[0].league;

     assertNotNull( league );

     league_division_s *league_divisions = league->divisions;

     assertNotNull( league_divisions );

     division_s *division = league_divisions[0].division;

     assertNotNull( division );

     division_team_s *division_teams = division->teams;

     assertNotNull( division_teams );

     team_s *team = division_teams[0].team;

     assertNotNull( team );

     team_player_s *team_players = team->players;

     assertNotNull( team_players );

     player_s *player = team_players[0].player;

     assertNotNull( player );

     assertEqualsStr( first_name,  player->first_name     );
     assertEqualsStr( last_name,   player->last_name      );
     assertEqualsStr( first_phoen, player->first_phonetic );
     assertEqualsStr( last_phoen,  player->last_phonetic  );

     freeOrg( org );

     return NULL;
}

static char *convertOrg_ShouldSkipEmptyPlayers_GivenPlayersFileData()
{
     fileplayer_s  players_data[TOTAL_PLAYERS] = { 0 };
     org_data_s    org_data                    = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = players_data;

     org_s *org = convertOrg( &org_data );

     org_league_s *leagues = org->leagues;

     assertNotNull( leagues );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          league_s *league = leagues[i].league;

          assertNotNull( league );

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
                    team_player_s  sentinel     = TEAM_PLAYER_SENTINEL;

                    assertNotNull( team_players );

                    assertEqualsInt( sentinel.team_id,   team_players->team_id   );
                    assertEqualsInt( sentinel.player_id, team_players->player_id );
               }
          }
     }

     freeOrg( org );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertOrg_ShouldReturnOrgWithLeagues_GivenLeagueFileData,                         null );
     run_test( convertOrg_ShouldReturnOrgWithDivisions_GivenLeagueFileData,                       null );
     run_test( convertOrg_ShouldReturnOrgWithTeams_GivenLeagueFileAndParksFileData,               null );
     run_test( convertOrg_ShouldReturnOrgWithPlayers_GivenPlayersFileData,                        null );
     run_test( convertOrg_ShouldRemoveTerminatorsOnPlayerNamesAndPhoenetics_GivenPlayersFileData, null );
     run_test( convertOrg_ShouldSkipEmptyPlayers_GivenPlayersFileData,                            null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
