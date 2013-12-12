#ifndef __INC_BUILDERS_H__
#define __INC_BUILDERS_H__

#include "file_formats.h"
#include "org.h"

league_s *buildLeague(                       const int league_id );
void      buildIntoLeague( league_s *league, const int league_id );

league_team_s *buildLeagueTeam(                                 const int league_id, const int team_id );
void           buildIntoLeagueTeam( league_team_s *league_team, const int league_id, const int team_id );

league_division_s *buildLeagueDivision(                                         const int league_id, const int division_id );
void               buildIntoLeagueDivision( league_division_s *league_division, const int league_id, const int division_id );

division_s *buildDivision(                           const int division_id );
void        buildIntoDivision( division_s *division, const int division_id );

division_team_s *buildDivisionTeam(                                     const int division_id, const int team_id );
void             buildIntoDivisionTeam( division_team_s *division_team, const int division_id, const int team_id );

team_s *buildTeam(                   const int team_id );
void    buildIntoTeam( team_s *team, const int team_id );

team_player_s *buildTeamPlayer(                                 const int team_id, const int season, const int player_id );
void           buildIntoTeamPlayer( team_player_s *team_player, const int team_id, const int season, const int player_id );

team_pitching_stats_s *buildTeamPitchingStats(                                                 const int team_id, const int season, const season_phase_e season_phase );
void                   buildIntoTeamPitchingStats( team_pitching_stats_s *team_pitching_stats, const int team_id, const int season, const season_phase_e season_phase );

player_s *buildPlayer(                       const int player_id );
void      buildIntoPlayer( player_s *player, const int player_id );

pitcher_s *buildPitcher(                         const int player_id );
void       buildIntoPitcher( pitcher_s *pitcher, const int player_id );

pitcher_stats_s *buildPitcherStats(                                     const int player_id, const int season, const season_phase_e season_phase );
void             buildIntoPitcherStats( pitcher_stats_s *pitcher_stats, const int player_id, const int season, const season_phase_e season_phase );

batter_s *buildBatter(                       const int player_id );
void      buildIntoBatter( batter_s *batter, const int player_id );

fileleagname_s *buildFileLeagName(          void );
fileparks_s    *buildFileParks(             void );
fileplayer_s   *buildFilePlayers(           void );
fileplayer_s   *buildFilePlayersWithoutIds( void );

schedule_s *buildSchedule( const fileleagname_s *league_data );
records_s *buildRecords( const fileleagname_s *league_data, const int season, const season_phase_e season_phase );

#endif
