#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "org.h"

fileplayer_s *generateFilePlayers( org_s *org )
{
     fileplayer_s *players_file = NULL;

     if ( (players_file = malloc( sizeof(fileplayer_s) * TOTAL_PLAYERS )) == NULL ) return NULL;

     if ( org->leagues == NULL ) return players_file;

     for ( int i = 0; org->leagues[i].league != NULL; ++i )
     {
          league_s *league = org->leagues[i].league;

          for ( int j = 0; league->divisions[j].division != NULL; ++j )
          {
               division_s *division = league->divisions[j].division;

               int div_idx = (i * DIVISIONS_PER_LEAGUE) + j;

               for ( int k = 0; division->teams[k].team != NULL; ++k )
               {
                    team_s *team = division->teams[k].team;

                    int team_idx = (i * TEAMS_PER_LEAGUE) + (j * TEAMS_PER_DIVISION) + k;

                    for ( int l = 0; team->players[l].player != NULL; ++l )
                    {
                         int player_idx = (i * PLAYERS_PER_LEAGUE) + (j * PLAYERS_PER_DIVISION) + (k * PLAYERS_PER_TEAM) + l;

                         int2word( players_file[player_idx].acc_stats.action.id_info.player_id, team->players[l].player->player_id );

                         memcpy( players_file[player_idx].first_name,  team->players[l].player->first_name,      sizeof(players_file[player_idx].first_name)  );
                         memcpy( players_file[player_idx].last_name,   team->players[l].player->last_name,       sizeof(players_file[player_idx].last_name)   );
                         memcpy( players_file[player_idx].first_phoen, team->players[l].player->first_phoenetic, sizeof(players_file[player_idx].first_phoen) );
                         memcpy( players_file[player_idx].last_phoen,  team->players[l].player->last_phoenetic,  sizeof(players_file[player_idx].last_phoen)  );

                         termName( players_file[player_idx].first_name,  sizeof(players_file[player_idx].first_name)  );
                         termName( players_file[player_idx].last_name,   sizeof(players_file[player_idx].last_name)   );
                         termName( players_file[player_idx].first_phoen, sizeof(players_file[player_idx].first_phoen) );
                         termName( players_file[player_idx].last_phoen,  sizeof(players_file[player_idx].last_phoen)  );

                         int2byte( players_file[player_idx].year, team->players[l].player->rookie_season + YEAR_SEASON_OFFSET );

                         if ( team->players[l].player->player_type == pt_Pitcher )
                         {
                              filepitching_s *pitching = &(players_file[player_idx].filestats.filepitching);

                              if   ( team->players[l].player->handedness == hnd_Right ) players_file[player_idx].position[0] = fh_Right;
                              else                                                      players_file[player_idx].position[0] = fh_Left;

                              if   ( team->players[l].player->skin_tone == st_Dark ) int2byte( pitching->color, fc_Dark  );
                              else                                                   int2byte( pitching->color, fc_Light );
                         }
                         else
                         {
                              filebatting_s *batting = &(players_file[player_idx].filestats.filebatting);

                              if   ( team->players[l].player->handedness == hnd_Right ) batting->ratings[0] = fh_Right << 4;
                              else                                                      batting->ratings[0] = fh_Left  << 4;

                              if   ( team->players[l].player->skin_tone == st_Dark ) int2byte( batting->color, fc_Dark  );
                              else                                                   int2byte( batting->color, fc_Light );
                         }
                    }
               }
          }
     }

     return players_file;
}
