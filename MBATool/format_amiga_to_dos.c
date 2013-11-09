#include <stdio.h>
#include "file_formats.h"


void formatAmigaToDos( fileplayer_s *players_file )
{
     for ( int idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
	  if ( players_file[idx].last_name[0] == '\0' ) continue;

          int pos = nibble( players_file[idx].position[0], n_High );

          acc_stats_s *acc_stats = &(players_file[idx].acc_stats);

          if   ( pos != fpos_Pitcher )
          {
               capStat( acc_stats->simulated.batting.acc_rbi, acc_stats->action.batting.acc_rbi );
               capStat( acc_stats->simulated.batting.acc_so,  acc_stats->action.batting.acc_so  );

               acc_stats_u act = acc_stats->action;

               acc_stats->action    = acc_stats->simulated;
               acc_stats->simulated = act;
          }
          else
          {
               unsigned char games = byte2int( acc_stats->simulated.pitching.acc_starts );

               int2byte( acc_stats->simulated.pitching.acc_starts, 0     );
               int2byte( acc_stats->action.   batting. acc_games,  games );
          }
     }
}
