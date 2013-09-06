#ifndef __INC_LEAGUE_H__
#define __INC_LEAGUE_H__

#include "data_list.h"
#include "season.h"

typedef struct
{
     int   league_id;
     char  name      [20 + 1];

} league_s;

typedef struct
{
     int  league_id;
     int  division_id;

} league_division_s;

typedef struct
{
     int            league_id;
     int            season;
     season_phase_e season_phase;
     int            wins;
     int            losses;
     int            home_wins;
     int            home_losses;
     int            road_wins;
     int            road_losses;
     int            runs_scored;
     int            runs_allowed;

} league_stats_s;

int leagues_t_create( sqlite3 *db, const league_s *league );
int leagues_t_read(   sqlite3 *db,       league_s *league );
int leagues_t_update( sqlite3 *db, const league_s *league );
int leagues_t_delete( sqlite3 *db, const league_s *league );

int league_divisions_t_create(         sqlite3 *db,                      const league_division_s *league_division  );
int league_divisions_t_read_by_league( sqlite3 *db, const int league_id,       data_list_s       *league_divisions );
int league_divisions_t_delete(         sqlite3 *db,                      const league_division_s *league_division  );

int league_stats_t_create( sqlite3 *db, const league_stats_s *league_stats );
int league_stats_t_read(   sqlite3 *db,       league_stats_s *league_stats );
int league_stats_t_update( sqlite3 *db, const league_stats_s *league_stats );
int league_stats_t_delete( sqlite3 *db, const league_stats_s *league_stats );


#endif
