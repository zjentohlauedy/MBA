#include <stdio.h>
#include "file_formats.h"


static char error_message[999 + 1];


char *getFormatDosToAmigaError( void )
{
     return error_message;
}


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


boolean_e formatDosToAmiga( fileplayer_s *players_file )
{
     clearErrorMessage();

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
	  if ( players_file[i].last_name[0] == '\0' ) continue;

          unsigned int pos = nibble( players_file[i].position[0], n_High );

          acc_player_id_s *id_info = NULL;

          if   ( pos != fpos_Pitcher )
          {
               filebatting_s *batting = &(players_file[i].filestats.filebatting);

               if ( byte2int( batting->action.acc_rbi ) >= STAT_CAP_AMOUNT ||
                    byte2int( batting->action.acc_so  ) >= STAT_CAP_AMOUNT    )
               {
                    acc_pch_stats_s *pch = (acc_pch_stats_s *)&(batting->simulated);

                    unsigned int act_inn  = word2int( pch->acc_innings );
                    unsigned int act_hits = word2int( pch->acc_hits    );

                    if ( act_inn > 0  ||  act_hits > 0 )
                    {
                         sprintf( error_message, "ERROR: Player %.*s, %.*s requires overflow and has pitching stats.",
                                  sizeof(players_file[i].last_name),  players_file[i].last_name,
                                  sizeof(players_file[i].first_name), players_file[i].first_name );

                         return bl_False;
                    }
               }

               acc_bat_stats_s sim = batting->simulated;

               batting->simulated = batting->action;
               batting->action    = sim;

               // If RBI are over STAT_CAP_AMOUNT, move extra from sim to act
               if ( batting->simulated.acc_rbi[0] >= STAT_CAP_AMOUNT )
               {
                    unsigned char x = batting->simulated.acc_rbi[0] - STAT_CAP_AMOUNT;

                    batting->action.acc_rbi[0]     = sim.acc_rbi[0];

                    batting->simulated.acc_rbi[0] -= x;
                    batting->action.acc_rbi[0]    += x;
               }

               // Same thing with strike outs
               if ( batting->simulated.acc_so[0] >= STAT_CAP_AMOUNT )
               {
                    unsigned char x = batting->simulated.acc_so[0] - STAT_CAP_AMOUNT;

                    batting->action.acc_so[0]     = sim.acc_so[0];

                    batting->simulated.acc_so[0] -= x;
                    batting->action.acc_so[0]  += x;
               }

               id_info = (acc_player_id_s *)&(batting->action);
          }
          else
          {
               acc_bat_stats_s *act = (acc_bat_stats_s *)&(players_file[i].filestats.filebatting.action);
               acc_pch_stats_s *sim =                    &(players_file[i].filestats.filepitching.simulated);

               unsigned char games = act->acc_games[0];

               act->acc_games[0] = 0;

               sim->acc_starts[0] = games;

               id_info = (acc_player_id_s *)&(players_file[i].filestats.filepitching.action);
          }

          int player_id = word2int( id_info->player_id );

          if ( player_id == 0  ||  calcChecksum( player_id ) != id_info->checksum[0] )
          {
               sprintf( error_message, "ERROR: Player %.*s, %.*s has a player id checksum mismatch.",
                        sizeof(players_file[i].last_name),  players_file[i].last_name,
                        sizeof(players_file[i].first_name), players_file[i].first_name );

               return bl_False;
          }

     }

     return bl_True;
}
