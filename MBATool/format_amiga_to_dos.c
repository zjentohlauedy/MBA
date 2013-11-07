#include <stdio.h>
#include "file_formats.h"


static void capStat( unsigned char *stat, unsigned char *overflow )
{
     if ( *stat < STAT_CAP_AMOUNT ) return;

     unsigned char x = *stat - STAT_CAP_AMOUNT;

     *overflow += x;
     *stat     -= x;
}

void formatAmigaToDos( fileplayer_s *players_file )
{
     for ( int idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
	  if ( players_file[idx].last_name[0] == '\0' ) continue;

          int pos = nibble( players_file[idx].position[0], n_High );

          if   ( pos != fpos_Pitcher )
          {
               filebatting_s *batting = &(players_file[idx].filestats.filebatting);

               capStat( batting->simulated.acc_rbi, batting->action.acc_rbi );
               capStat( batting->simulated.acc_so,  batting->action.acc_so  );

               acc_bat_stats_s act = batting->action;

               batting->action    = batting->simulated;
               batting->simulated = act;
          }
          else
          {
               filepitching_s *pitching = &(players_file[idx].filestats.filepitching);

               unsigned char games = pitching->simulated.acc_starts[0];

               pitching->simulated.acc_starts[0] = 0;

               acc_bat_stats_s *action = (acc_bat_stats_s *)&(pitching->action);

               action->acc_games[0] = games;
          }
     }
}
