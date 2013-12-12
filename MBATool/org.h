#ifndef __INC_ORG_H__
#define __INC_ORG_H__

#include "file_formats.h"
#include "league.h"


#define ORG_LEAGUE_SENTINEL { 0 }


typedef struct
{
     league_stats_s   *leagues;
     division_stats_s *divisions;
     team_stats_s     *teams;

} records_s;

typedef struct
{
     league_s *league;

} org_league_s;

typedef struct
{
     org_league_s *leagues;

} org_s;

typedef struct
{
     fileleagname_s *league_data;
     fileparks_s    *parks_data;
     fileplayer_s   *players_data;
     records_s      *records;
     int             season;
     season_phase_e  season_phase;

} org_data_s;

org_s *getOrg(  sqlite3 *db, const int    season );
int    saveOrg( sqlite3 *db, const org_s *org    );

org_s             *convertOrg(           const org_data_s *org_data                                                );
org_league_s      *convertLeagues(       const org_data_s *org_data                                                );
league_division_s *convertDivisions(     const org_data_s *org_data, const int league_id,   const int league_idx   );
division_team_s   *convertDivisionTeams( const org_data_s *org_data, const int division_id, const int division_idx );
league_team_s     *convertLeagueTeams(   const org_data_s *org_data, const int league_id,   const int league_idx   );
team_player_s     *convertPlayers(       const org_data_s *org_data, const int team_id,     const int team_idx     );
records_s         *calculateRecords(     const schedule_s *schedule, const fileleagname_s *league_file, const int season, const season_phase_e season_phase );

char *getCalculateRecordsError( void );

fileleagname_s *generateFileLeagName( const org_s *org                   );
fileplayer_s   *generateFilePlayers(  const org_s *org, const int season );

void freeOrg(        org_s        *org         );
void freeOrgLeagues( org_league_s *org_leagues );
void freeRecords(    records_s    *records     );

#endif
