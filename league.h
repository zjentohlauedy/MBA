#ifndef __INC_LEAGUE_H__
#define __INC_LEAGUE_H__

#include "ewbfiles.h"


typedef enum
{
     pt_Pitcher,
     pt_Hitter,
     pt_None

} playertype_e;

typedef struct
{
     int   wins;
     int   losses;
     int   games;
     int   games_started;
     int   games_completed;
     int   saves;
     float innings;
     int   at_bats;
     int   hits;
     int   earned_runs;
     int   home_runs;
     int   walks;
     int   strike_outs;

} pitching_s;

typedef struct
{
     char        first_name[ 20 + 1 ];
     char        last_name [ 20 + 1 ];
     int         year;
     position_e  primary_pos;
     position_e  secondary_pos;
     int         speed;
     int         control;
     int         fatigue;
     int         bunt;
     int         longevity;
     pitching_s  potential;
     pitching_s  simulated;
     float       fielding_avg;

} pitcher_s;

typedef struct
{
     int games;
     int at_bats;
     int runs;
     int hits;
     int doubles;
     int triples;
     int home_runs;
     int rbi;
     int walks;
     int strike_outs;
     int stolen_bases;
     int errors;

} hitting_s;

typedef struct
{
     int   put_outs;
     int   assists;
     int   errors;
     float secondary_avg;

} fielding_s;

typedef struct
{
     char        first_name[ 20 + 1 ];
     char        last_name [ 20 + 1 ];
     int         year;
     position_e  primary_pos;
     position_e  secondary_pos;
     int         power;
     int         hit_n_run;
     int         bunt;
     int         running;
     int         range;
     int         arm;
     int         longevity;
     hitting_s   potential;
     hitting_s   simulated;
     fielding_s  fielding;

} batter_s;

typedef struct
{
     int          player_id;
     playertype_e type;
     int          injury_days;

     union
     {
	  pitcher_s    pitcher;
	  batter_s     batter;

     }            data;

} player_s;

typedef struct
{
     char     name[20 + 1];
     int      wins;
     int      losses;
     color_e  color;
     player_s players[PLAYERS_PER_TEAM];

} team_s;

typedef struct
{
     char   name[20 + 1];
     team_s teams[TEAMS_PER_DIVISION];

} division_s;

typedef struct
{
     char       name[20 + 1];
     division_s divisions[DIVISIONS_PER_LEAGUE];

} league_s;


league_s *convertFilesToLeagues( fileleague_s *leagueFile, const fileplayer_s *playerFile );


#endif
