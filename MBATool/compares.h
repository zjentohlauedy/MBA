#ifndef __INC_COMPARES_H__
#define __INC_COMPARES_H__

#include "file_formats.h"
#include "player.h"
#include "team.h"


#define comparePitcherStats( A, B ) do { char *ret; if ( (ret = _comparePitcherStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_comparePitcherStats( pitcher_stats_s *expected, pitcher_stats_s *actual );

#define compareTeamPitchingStats( A, B ) do { char *ret; if ( (ret = _compareTeamPitchingStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_compareTeamPitchingStats( team_pitching_stats_s *expected, team_pitching_stats_s *actual );

#define compareTeamVersusStats( A, B ) do { char *ret; if ( (ret = _compareTeamVersusStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_compareTeamVersusStats( team_versus_stats_s *expected, team_versus_stats_s *actual );

#define compareBattingStats( A, B ) do { char *ret; if ( (ret = _compareBattingStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_compareBattingStats( acc_bat_stats_s *expected, acc_bat_stats_s *actual );

#define compareTeamBattingStats( A, B ) do { char *ret; if ( (ret = _compareTeamBattingStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_compareTeamBattingStats( team_batting_stats_s *expected, team_batting_stats_s *actual );

#endif
