#ifndef __INC_DIVISION_H__
#define __INC_DIVISION_H__

#include <sqlite3.h>
#include "data_list.h"
#include "season.h"

#define DIVISION_TEAM_SENTINEL     { -1, -1 }
#define DIVISION_STATS_SENTINEL    { -1, -1, sp_None, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
#define DIVISION_ACCOLADE_SENTINEL { -1, -1, dacc_None }

typedef enum
{
     dacc_None           = 0,
     dacc_World_Title    = 1,
     dacc_League_Title   = 2,
     dacc_Best_Record    = 3

} division_accolade_e;

typedef struct
{
     int                  division_id;
     int                  season;
     division_accolade_e  accolade;

} division_accolade_s;

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

typedef struct
{
     int   division_id;
     int   team_id;

} division_team_s;

typedef struct
{
     int                  division_id;
     char                 name        [20 + 1];
     division_team_s     *teams;
     division_stats_s    *stats;
     division_accolade_s *accolades;

} division_s;


int divisions_t_create( sqlite3 *db, const division_s *division );
int divisions_t_read(   sqlite3 *db,       division_s *division );
int divisions_t_update( sqlite3 *db, const division_s *division );
int divisions_t_delete( sqlite3 *db, const division_s *division );

int division_teams_t_create(           sqlite3 *db,                        const division_team_s *division_team  );
int division_teams_t_read_by_division( sqlite3 *db, const int division_id,       data_list_s     *division_teams );
int division_teams_t_delete(           sqlite3 *db,                        const division_team_s *division_team  );

int division_stats_t_create(           sqlite3 *db,                        const division_stats_s *division_stats );
int division_stats_t_read(             sqlite3 *db,                              division_stats_s *division_stats );
int division_stats_t_read_by_division( sqlite3 *db, const int division_id,       data_list_s      *division_stats );
int division_stats_t_update(           sqlite3 *db,                        const division_stats_s *division_stats );
int division_stats_t_delete(           sqlite3 *db,                        const division_stats_s *division_stats );

int division_accolades_t_create(           sqlite3 *db,                        const division_accolade_s *division_accolade  );
int division_accolades_t_read_by_division( sqlite3 *db, const int division_id,       data_list_s         *division_accolades );
int division_accolades_t_delete(           sqlite3 *db,                        const division_accolade_s *division_accolade  );

division_s *get_division(    sqlite3 *db, const int         division_id );
int         save_division(   sqlite3 *db, const division_s *division    );
int         remove_division( sqlite3 *db, const division_s *division    );
void        free_division(                      division_s *division    );

#endif
