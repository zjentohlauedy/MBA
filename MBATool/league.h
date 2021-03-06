#ifndef __INC_LEAGUE_H__
#define __INC_LEAGUE_H__

#include <sqlite3.h>
#include "data_list.h"
#include "division.h"
#include "season.h"

#define LEAGUE_SENTINEL          { -1, { 0 }, NULL, NULL, NULL }
#define LEAGUE_DIVISION_SENTINEL { -1, -1 }
#define LEAGUE_TEAM_SENTINEL     { -1, -1 }
#define LEAGUE_STATS_SENTINEL    { -1, -1, sp_None, -1, -1, -1, -1, -1, -1, -1, -1 }
#define LEAGUE_ACCOLADE_SENTINEL { -1, -1, lacc_None }

typedef enum
{
     lacc_None              = 0,
     lacc_World_Title       = 1,
     lacc_Best_Record       = 2,
     lacc_All_Star_Champion = 3

} league_accolade_e;

typedef struct
{
     int         league_id;
     int         team_id;
     team_s     *team;

} league_team_s;

typedef struct
{
     int                league_id;
     int                season;
     league_accolade_e  accolade;

} league_accolade_s;

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

typedef struct
{
     int         league_id;
     int         division_id;
     division_s *division;

} league_division_s;

typedef struct
{
     int                league_id;
     char               name      [20 + 1];
     league_division_s *divisions;
     league_team_s     *teams;
     league_stats_s    *stats;
     league_accolade_s *accolades;

} league_s;

int leagues_t_create(   sqlite3 *db, const league_s    *league  );
int leagues_t_read(     sqlite3 *db,       league_s    *league  );
int leagues_t_read_all( sqlite3 *db,       data_list_s *leagues );
int leagues_t_update(   sqlite3 *db, const league_s    *league  );
int leagues_t_delete(   sqlite3 *db, const league_s    *league  );

int league_divisions_t_create(         sqlite3 *db,                      const league_division_s *league_division  );
int league_divisions_t_read_by_league( sqlite3 *db, const int league_id,       data_list_s       *league_divisions );
int league_divisions_t_delete(         sqlite3 *db,                      const league_division_s *league_division  );

int league_stats_t_create(         sqlite3 *db,                      const league_stats_s *league_stats );
int league_stats_t_read(           sqlite3 *db,                            league_stats_s *league_stats );
int league_stats_t_read_by_league( sqlite3 *db, const int league_id,       data_list_s    *league_stats );
int league_stats_t_update(         sqlite3 *db,                      const league_stats_s *league_stats );
int league_stats_t_delete(         sqlite3 *db,                      const league_stats_s *league_stats );

int league_accolades_t_create(         sqlite3 *db,                      const league_accolade_s *league_accolade  );
int league_accolades_t_read_by_league( sqlite3 *db, const int league_id,       data_list_s       *league_accolades );
int league_accolades_t_delete(         sqlite3 *db,                      const league_accolade_s *league_accolade  );

int league_teams_t_create(         sqlite3 *db,                      const league_team_s *league_team  );
int league_teams_t_read_by_league( sqlite3 *db, const int league_id,       data_list_s   *league_teams );
int league_teams_t_delete(         sqlite3 *db,                      const league_team_s *league_team  );

league_s *get_league(    sqlite3 *db, const int       league_id );
int       save_league(   sqlite3 *db, const league_s *league    );
int       remove_league( sqlite3 *db, const league_s *league    );

void      free_league(           league_s          *league           );
void      free_league_divisions( league_division_s *league_divisions );
void      free_league_teams(     league_team_s     *league_teams     );

#endif
