#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

records_s *buildRecords( const int season, const season_phase_e season_phase )
{
     static records_s        records;
     static league_stats_s   leagues   [TOTAL_LEAGUES];
     static division_stats_s divisions [TOTAL_DIVISIONS];
     static team_stats_s     teams     [TOTAL_TEAMS];


     memset( leagues,   '\0', sizeof(leagues)   );
     memset( divisions, '\0', sizeof(divisions) );
     memset( teams,     '\0', sizeof(teams)     );

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
          teams[i].team_id         = i + 1;
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

     records.leagues   = leagues;
     records.divisions = divisions;
     records.teams     = teams;

     return &records;
}
