#ifndef __INC_COMPARES_H__
#define __INC_COMPARES_H__

#include "player.h"
#include "team.h"


#define comparePitcherStats( A, B ) do { char *ret; if ( (ret = _comparePitcherStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_comparePitcherStats( pitcher_stats_s *expected, pitcher_stats_s *actual );

#define compareTeamPitchingStats( A, B ) do { char *ret; if ( (ret = _compareTeamPitchingStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char *_compareTeamPitchingStats( team_pitching_stats_s *expected, team_pitching_stats_s *actual );


#endif
