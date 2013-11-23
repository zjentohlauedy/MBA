#ifndef __INC_BUILDERS_H__
#define __INC_BUILDERS_H__

#include "file_formats.h"
#include "org.h"


pitcher_stats_s *buildPitcherStats(                                     const int player_id, const int season, const season_phase_e season_phase );
void             buildIntoPitcherStats( pitcher_stats_s *pitcher_stats, const int player_id, const int season, const season_phase_e season_phase );

team_pitching_stats_s *buildTeamPitchingStats(                                                 const int team_id, const int season, const season_phase_e season_phase );
void                   buildIntoTeamPitchingStats( team_pitching_stats_s *team_pitching_stats, const int team_id, const int season, const season_phase_e season_phase );

fileleagname_s *buildFileLeagName(          void );
fileparks_s    *buildFileParks(             void );
fileplayer_s   *buildFilePlayers(           void );
fileplayer_s   *buildFilePlayersWithoutIds( void );

schedule_s *buildSchedule( const fileleagname_s *league_data );
records_s  *buildRecords(  const int season, const season_phase_e season_phase );

#endif
