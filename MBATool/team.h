#ifndef __INC_TEAM_H__
#define __INC_TEAM_H__

#include <sqlite3.h>
#include "player.h"
#include "season.h"
#include "data_list.h"

#define TEAM_PLAYER_SENTINEL         { -1, -1, -1, NULL }
#define TEAM_STATS_SENTINEL          { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
#define TEAM_PITCHING_STATS_SENTINEL { -1, -1, sp_None, -1, -1, -1, -1, { -1, -1 }, -1, -1, -1, -1, -1 }
#define TEAM_BATTING_STATS_SENTINEL  { -1, -1, sp_None, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
#define TEAM_ACCOLADE_SENTINEL       { -1, -1, tacc_None }

typedef enum
{
     cl_None   = 0,
     cl_LtBlue = 1,
     cl_Blue   = 2,
     cl_DkRed  = 3,
     cl_Red    = 4,
     cl_Gold   = 5,
     cl_Black  = 6,
     cl_Brown  = 7,
     cl_Green  = 8

} color_e;

typedef enum
{
     tacc_None           = 0,
     tacc_World_Title    = 1,
     tacc_League_Title   = 2,
     tacc_Division_Title = 3,
     tacc_Best_Record    = 4

} team_accolade_e;

typedef struct
{
     int              team_id;
     int              season;
     team_accolade_e  accolade;

} team_accolade_s;

typedef struct
{
     int            team_id;
     int            season;
     season_phase_e season_phase;
     int            games;
     int            at_bats;
     int            runs;
     int            hits;
     int            doubles;
     int            triples;
     int            home_runs;
     int            runs_batted_in;
     int            walks;
     int            strike_outs;
     int            steals;
     int            errors;

} team_batting_stats_s;

typedef struct
{
     int            team_id;
     int            season;
     season_phase_e season_phase;
     int            wins;
     int            losses;
     int            games;
     int            saves;
     innings_s      innings;
     int            hits;
     int            earned_runs;
     int            home_runs;
     int            walks;
     int            strike_outs;

} team_pitching_stats_s;

typedef struct
{
     int            team_id;
     int            season;
     season_phase_e season_phase;
     int            wins;
     int            losses;
     int            home_wins;
     int            home_losses;
     int            road_wins;
     int            road_losses;
     int            division_wins;
     int            division_losses;
     int            league_wins;
     int            league_losses;
     int            runs_scored;
     int            runs_allowed;

} team_stats_s;

typedef struct
{
     int       team_id;
     int       season;
     int       player_id;
     player_s *player;

} team_player_s;

typedef struct
{
     int                    team_id;
     char                   name            [20 + 1];
     char                   location        [20 + 1];
     color_e                primary_color;
     color_e                secondary_color;
     team_player_s         *players;
     team_stats_s          *stats;
     team_pitching_stats_s *pitching_stats;
     team_batting_stats_s  *batting_stats;
     team_accolade_s       *accolades;

} team_s;


int teams_t_create( sqlite3 *db, const team_s *team );
int teams_t_read(   sqlite3 *db,       team_s *team );
int teams_t_update( sqlite3 *db, const team_s *team );
int teams_t_delete( sqlite3 *db, const team_s *team );

int team_players_t_create(                  sqlite3 *db,                                   const team_player_s *team_player  );
int team_players_t_read_by_team(            sqlite3 *db, const int            team_id,           data_list_s   *team_players );
int team_players_t_read_by_team_and_season( sqlite3 *db, const team_player_s *team_player,       data_list_s   *team_players );
int team_players_t_delete(                  sqlite3 *db,                                   const team_player_s *team_player  );

int team_stats_t_create(       sqlite3 *db,                    const team_stats_s *team_stats );
int team_stats_t_read(         sqlite3 *db,                          team_stats_s *team_stats );
int team_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s  *team_stats );
int team_stats_t_update(       sqlite3 *db,                    const team_stats_s *team_stats );
int team_stats_t_delete(       sqlite3 *db,                    const team_stats_s *team_stats );

int team_pitching_stats_t_create(       sqlite3 *db,                    const team_pitching_stats_s *team_pitching_stats );
int team_pitching_stats_t_read(         sqlite3 *db,                          team_pitching_stats_s *team_pitching_stats );
int team_pitching_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s           *team_pitching_stats );
int team_pitching_stats_t_update(       sqlite3 *db,                    const team_pitching_stats_s *team_pitching_stats );
int team_pitching_stats_t_delete(       sqlite3 *db,                    const team_pitching_stats_s *team_pitching_stats );

int team_batting_stats_t_create(       sqlite3 *db,                    const team_batting_stats_s *team_batting_stats );
int team_batting_stats_t_read(         sqlite3 *db,                          team_batting_stats_s *team_batting_stats );
int team_batting_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s          *team_batting_stats );
int team_batting_stats_t_update(       sqlite3 *db,                    const team_batting_stats_s *team_batting_stats );
int team_batting_stats_t_delete(       sqlite3 *db,                    const team_batting_stats_s *team_batting_stats );

int team_accolades_t_create(       sqlite3 *db,                    const team_accolade_s *team_accolade  );
int team_accolades_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s     *team_accolades );
int team_accolades_t_delete(       sqlite3 *db,                    const team_accolade_s *team_accolade  );

team_s *get_team(            sqlite3 *db, const int     team_id                   );
team_s *get_team_for_season( sqlite3 *db, const int     team_id, const int season );
int     save_team(           sqlite3 *db, const team_s *team                      );
int     remove_team(         sqlite3 *db, const team_s *team                      );

void free_team(         team_s        *team         );
void free_team_players( team_player_s *team_players );

#endif
