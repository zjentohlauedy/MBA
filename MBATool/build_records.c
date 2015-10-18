#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "file_formats.h"

records_s *buildRecords( const fileleagname_s *league_data, const int season, const season_phase_e season_phase )
{
     static records_s           records;
     static league_stats_s      leagues   [TOTAL_LEAGUES];
     static division_stats_s    divisions [TOTAL_DIVISIONS];
     static team_stats_s        teams     [TOTAL_TEAMS];
     static team_versus_stats_s versus    [TOTAL_TEAMS][TOTAL_TEAMS];


     memset( leagues,   '\0', sizeof(leagues)   );
     memset( divisions, '\0', sizeof(divisions) );
     memset( teams,     '\0', sizeof(teams)     );
     memset( versus,    '\0', sizeof(versus)    );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          leagues[i].league_id    = i + 1;
          leagues[i].season       = season;
          leagues[i].season_phase = season_phase;
          leagues[i].wins         = rand() % 5000;
          leagues[i].losses       = rand() % 5000;
          leagues[i].home_wins    = rand() % 5000;
          leagues[i].home_losses  = rand() % 5000;
          leagues[i].road_wins    = rand() % 5000;
          leagues[i].road_losses  = rand() % 5000;
          leagues[i].runs_scored  = rand() % 5000;
          leagues[i].runs_allowed = rand() % 5000;
     }

     for ( int i = 0; i < TOTAL_DIVISIONS; ++i )
     {
          divisions[i].division_id   = i + 1;
          divisions[i].season        = season;
          divisions[i].season_phase  = season_phase;
          divisions[i].wins          = rand() % 5000;
          divisions[i].losses        = rand() % 5000;
          divisions[i].home_wins     = rand() % 5000;
          divisions[i].home_losses   = rand() % 5000;
          divisions[i].road_wins     = rand() % 5000;
          divisions[i].road_losses   = rand() % 5000;
          divisions[i].league_wins   = rand() % 5000;
          divisions[i].league_losses = rand() % 5000;
          divisions[i].runs_scored   = rand() % 5000;
          divisions[i].runs_allowed  = rand() % 5000;
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          teams[i].team_id         = byte2int( league_data->teams[i].team_id );
          teams[i].season          = season;
          teams[i].season_phase    = season_phase;
          teams[i].wins            = rand() % 5000;
          teams[i].losses          = rand() % 5000;
          teams[i].home_wins       = rand() % 5000;
          teams[i].home_losses     = rand() % 5000;
          teams[i].road_wins       = rand() % 5000;
          teams[i].road_losses     = rand() % 5000;
          teams[i].division_wins   = rand() % 5000;
          teams[i].division_losses = rand() % 5000;
          teams[i].league_wins     = rand() % 5000;
          teams[i].league_losses   = rand() % 5000;
          teams[i].runs_scored     = rand() % 5000;
          teams[i].runs_allowed    = rand() % 5000;
     }

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          for ( int j = 0; j < TOTAL_TEAMS; ++j )
          {
               versus[i][j].team_id      = byte2int( league_data->teams[i].team_id );
               versus[i][j].season       = season;
               versus[i][j].season_phase = season_phase;
               versus[i][j].opponent     = byte2int( league_data->teams[j].team_id );
               versus[i][j].wins         = 1 + rand() % 5;
               versus[i][j].losses       = 1 + rand() % 5;
               versus[i][j].runs_scored  = 1 + rand() % 25;
               versus[i][j].runs_allowed = 1 + rand() % 25;
          }
     }

     records.leagues   = leagues;
     records.divisions = divisions;
     records.teams     = teams;

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          records.versus[i] = versus[i];
     }

     return &records;
}
