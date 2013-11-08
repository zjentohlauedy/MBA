#include <stdio.h>
#include "file_formats.h"


static char error_message[999 + 1] = { 0 };


char *getFormatDosToAmigaError( void )
{
     return error_message;
}


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}


static void capStat( unsigned char *stat, unsigned char *overflow )
{
     if ( *stat < STAT_CAP_AMOUNT ) return;

     unsigned char x = *stat - STAT_CAP_AMOUNT;

     *overflow += x;
     *stat     -= x;
}

boolean_e formatDosToAmiga( fileplayer_s *players_file )
{
     clearErrorMessage();

     for ( int i = 0; i < TOTAL_PLAYERS; ++i )
     {
	  if ( players_file[i].last_name[0] == '\0' ) continue;

          unsigned int pos = nibble( players_file[i].position[0], n_High );

          if   ( pos != fpos_Pitcher )
          {
               acc_bat_stats_s *action = &(players_file[i].acc_stats.amiga.action.batting);

               if ( byte2int( action->acc_rbi ) >= STAT_CAP_AMOUNT ||
                    byte2int( action->acc_so  ) >= STAT_CAP_AMOUNT    )
               {
                    acc_pch_stats_s *pch = &(players_file[i].acc_stats.amiga.simulated.pitching);

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

               acc_amiga_s *acc_stats = &(players_file[i].acc_stats.amiga);

               acc_stats_u sim = acc_stats->simulated;

               acc_stats->simulated = acc_stats->action;
               acc_stats->action    = sim;

               capStat( acc_stats->simulated.batting.acc_rbi, acc_stats->action.batting.acc_rbi );
               capStat( acc_stats->simulated.batting.acc_so,  acc_stats->action.batting.acc_so  );
          }
          else
          {
               acc_bat_stats_s *act = &(players_file[i].acc_stats.amiga.action.batting);
               acc_pch_stats_s *sim = &(players_file[i].acc_stats.amiga.simulated.pitching);

               unsigned char games = act->acc_games[0];

               act->acc_games[0] = 0;

               sim->acc_starts[0] = games;
          }

          acc_player_id_s *id_info = &(players_file[i].acc_stats.amiga.action.id_info);

          int player_id = word2int( id_info->player_id );

          if ( player_id == 0  ||  calcChecksum( player_id ) != byte2int( id_info->checksum ) )
          {
               sprintf( error_message, "ERROR: Player %.*s, %.*s has a player id checksum mismatch.",
                        sizeof(players_file[i].last_name),  players_file[i].last_name,
                        sizeof(players_file[i].first_name), players_file[i].first_name );

               return bl_False;
          }
     }

     return bl_True;
}
