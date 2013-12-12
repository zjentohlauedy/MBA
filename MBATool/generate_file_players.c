#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "org.h"


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

fileplayer_s *generateFilePlayers( const org_s *org, const int season )
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

                         int age_adjustment = (team->players[l].player->longevity + 3) - (season - team->players[l].player->rookie_season);

                         if ( team->players[l].player->player_type == pt_Pitcher )
                         {
                              pitcher_s      *pitcher  = team->players[l].player->details.pitching;
                              filepitching_s *pitching = &(players_file[player_idx].filestats.filepitching);

                              filehand_e hand = (team->players[l].player->handedness == hnd_Right) ? fh_Right : fh_Left;

                              int speed   = (age_adjustment >= 0) ? pitcher->speed   : MIN( pitcher->speed   - age_adjustment, 1 );
                              int control = (age_adjustment >= 0) ? pitcher->control : MIN( pitcher->control - age_adjustment, 1 );
                              int fatigue = (age_adjustment >= 0) ? pitcher->fatigue : MIN( pitcher->fatigue - age_adjustment, 1 );

                              players_file[player_idx].position[0] = (fpos_Pitcher << 4) + hand;

                              pitching->ratings[0] = (speed   << 4) +                          control;
                              pitching->ratings[1] = (fatigue << 4) + team->players[l].player->longevity;

                              if   ( team->players[l].player->skin_tone == st_Dark ) int2byte( pitching->color, fc_Dark  );
                              else                                                   int2byte( pitching->color, fc_Light );
                         }
                         else
                         {
                              batter_s      *batter  = team->players[l].player->details.batting;
                              filebatting_s *batting = &(players_file[player_idx].filestats.filebatting);

                              filehand_e     hand      = (team->players[l].player->handedness == hnd_Right) ? fh_Right : fh_Left;
                              fileposition_e primary   = mapPosition( batter->primary_position   );
                              fileposition_e secondary = mapPosition( batter->secondary_position );

                              int arm       = (age_adjustment >= 0) ? batter->arm       : MIN( batter->arm       - age_adjustment, 1 );
                              int running   = (age_adjustment >= 0) ? batter->running   : MIN( batter->running   - age_adjustment, 1 );
                              int range     = (age_adjustment >= 0) ? batter->range     : MIN( batter->range     - age_adjustment, 1 );
                              int power     = (age_adjustment >= 0) ? batter->power     : MIN( batter->power     - age_adjustment, 1 );
                              int bunt      = (age_adjustment >= 0) ? batter->bunt      : MIN( batter->bunt      - age_adjustment, 1 );
                              int hit_n_run = (age_adjustment >= 0) ? batter->hit_n_run : MIN( batter->hit_n_run - age_adjustment, 1 );

                              players_file[player_idx].position[0] = (primary << 4) + secondary;

                              batting->ratings[0] = (hand    << 4) +                          arm;
                              batting->ratings[1] = (running << 4) +                          range;
                              batting->ratings[2] = (power   << 4) + team->players[l].player->longevity;
                              batting->ratings[3] = (bunt    << 4) +                          hit_n_run;

                              if   ( team->players[l].player->skin_tone == st_Dark ) int2byte( batting->color, fc_Dark  );
                              else                                                   int2byte( batting->color, fc_Light );
                         }
                    }
               }
          }
     }

     return players_file;
}
