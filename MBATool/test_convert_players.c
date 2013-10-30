#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"
#include "org.h"
#include "unit_test.h"


static char *result;


static char *convertPlayers_ShouldReturnAListOfTeamPlayers_GivenPlayersFileDataAndTeamId()
{
     org_data_s org_data = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();
     org_data.season       = 13;

     fileplayer_s  *players_data = org_data.players_data;

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          int team_id = i + 1;

          team_player_s *team_players = convertPlayers( &org_data, team_id );

          assertNotNull( team_players );

          for ( int j = 0; j < PLAYERS_PER_TEAM; ++j )
          {
               int idx = (i * PLAYERS_PER_TEAM) + j;
               int id  = (i * PLAYERS_PER_TEAM) + j + 1;

               player_s *player = team_players[j].player;

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

               assertEqualsInt( team_id,         team_players[j].team_id );
               assertEqualsInt( org_data.season, team_players[j].season  );

               assertEqualsInt(                   player_id,     player->player_id      );
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

          assertNull( team_players[PLAYERS_PER_TEAM].player );

          free_team_players( team_players );
     }

     return NULL;
}

static char *convertPlayers_ShouldRemoveTerminatorsOnPlayerNamesAndPhoenetics_GivenPlayersFileData()
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

     team_player_s *team_players = convertPlayers( &org_data, 1 );

     assertNotNull( team_players );

     player_s *player = team_players[0].player;

     assertNotNull( player );

     assertEqualsStr( first_name,  player->first_name     );
     assertEqualsStr( last_name,   player->last_name      );
     assertEqualsStr( first_phoen, player->first_phonetic );
     assertEqualsStr( last_phoen,  player->last_phonetic  );

     free_team_players( team_players );

     return NULL;
}

static char *convertPlayers_ShouldSkipEmptyPlayers_GivenPlayersFileData()
{
     fileplayer_s  players_data[TOTAL_PLAYERS] = { 0 };
     org_data_s    org_data                    = { 0 };

     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = players_data;

     team_player_s *team_players = convertPlayers( &org_data, 1 );
     team_player_s  sentinel     = TEAM_PLAYER_SENTINEL;

     assertNotNull( team_players );

     assertEqualsInt( sentinel.team_id,   team_players->team_id   );
     assertEqualsInt( sentinel.player_id, team_players->player_id );

     free_team_players( team_players );

     return NULL;
}

static char *convertPlayers_ShouldFailIfAPlayerHasAIdChecksumMismatch_GivenPlayersFileData()
{
     org_data_s org_data = { 0 };


     org_data.league_data  = buildFileLeagName();
     org_data.parks_data   = buildFileParks();
     org_data.players_data = buildFilePlayers();

     fileplayer_s *players_data = org_data.players_data;

     if ( nibble( players_data[0].position[0], n_High ) == fpos_Pitcher )
     {
          struct pitching_s *pitching = &(players_data[0].filestats.filepitching);

          acc_player_id_s *player_id_data = (acc_player_id_s *)&(pitching->action);

          player_id_data->checksum[0]++;
     }
     else
     {
          struct batting_s *batting = &(players_data[0].filestats.filebatting);

          acc_player_id_s *player_id_data = (acc_player_id_s *)&(batting->action);

          player_id_data->checksum[0]++;
     }

     assertNull( convertPlayers( &org_data, 1 ) );

     return NULL;
}

static void run_all_tests()
{
     run_test( convertPlayers_ShouldReturnAListOfTeamPlayers_GivenPlayersFileDataAndTeamId,           null );
     run_test( convertPlayers_ShouldRemoveTerminatorsOnPlayerNamesAndPhoenetics_GivenPlayersFileData, null );
     run_test( convertPlayers_ShouldSkipEmptyPlayers_GivenPlayersFileData,                            null );
     run_test( convertPlayers_ShouldFailIfAPlayerHasAIdChecksumMismatch_GivenPlayersFileData,         null );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
