#ifndef __INC_DIVISION_H__
#define __INC_DIVISION_H__

#include "data_list.h"
#include "season.h"

typedef struct
{
     int   division_id;
     char  name        [20 + 1];

} division_s;

typedef struct
{
     int   division_id;
     int   team_id;

} division_team_s;

typedef struct
{
     int            division_id;
     int            season;
     season_phase_e season_phase;
     int            wins;
     int            losses;
     int            home_wins;
     int            home_losses;
     int            road_wins;
     int            road_losses;
     int            league_wins;
     int            league_losses;
     int            runs_scored;
     int            runs_allowed;

} division_stats_s;

int divisions_t_create( sqlite3 *db, const division_s *division );
int divisions_t_read(   sqlite3 *db,       division_s *division );
int divisions_t_update( sqlite3 *db, const division_s *division );
int divisions_t_delete( sqlite3 *db, const division_s *division );

int division_teams_t_create(           sqlite3 *db,                        const division_team_s *division_team  );
int division_teams_t_read_by_division( sqlite3 *db, const int division_id,       data_list_s     *division_teams );
int division_teams_t_delete(           sqlite3 *db,                        const division_team_s *division_team  );

int division_stats_t_create( sqlite3 *db, const division_stats_s *division_stats );
int division_stats_t_read(   sqlite3 *db,       division_stats_s *division_stats );
int division_stats_t_update( sqlite3 *db, const division_stats_s *division_stats );
int division_stats_t_delete( sqlite3 *db, const division_stats_s *division_stats );


#endif
