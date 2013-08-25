#ifndef __INC_PLAYER_H__
#define __INC_PLAYER_H__

#include "data_list.h"

typedef enum
{
     acc_None           = 0,
     acc_All_Star       = 1,
     acc_World_Title    = 2,
     acc_League_Title   = 3,
     acc_Division_Title = 4,
     acc_Best_Record    = 5

} accolade_e;

typedef enum
{
     pacc_None                         =  0,
     pacc_Pitcher_of_the_Year          =  1,
     pacc_Global_Pitcher_of_the_Year   =  2,
     pacc_World_Pitcher_of_the_Year    =  3,
     pacc_Atlantic_Pitcher_of_the_Year =  4,
     pacc_North_Pitcher_of_the_Year    =  5,
     pacc_South_Pitcher_of_the_Year    =  6,
     pacc_Pacific_Pitcher_of_the_Year  =  7,
     pacc_Most_Wins                    =  8,
     pacc_Best_Win_Percentage          =  9,
     pacc_Best_Earned_Run_Average      = 10,
     pacc_Best_vs_Batting_Average      = 11,
     pacc_Most_Saves                   = 12,
     pacc_Most_Strike_Outs             = 13,
     pacc_Most_Innings_per_Game        = 14,
     pacc_Best_Hits_per_Nine           = 15,
     pacc_Best_Walks_per_Nine          = 16,
     pacc_Most_Strike_Outs_per_Nine    = 17,
     pacc_Best_Home_Runs_per_Nine      = 18,
     pacc_Best_Efficiency              = 19

} pitching_accolade_e;

typedef enum
{
     hnd_None  = 0,
     hnd_Right = 1,
     hnd_Left  = 2,

} handedness_e;

typedef enum
{
     pt_None    = 0,
     pt_Pitcher = 1,
     pt_Hitter  = 2

} playertype_e;

typedef enum
{
     sp_None    = 0,
     sp_Regular = 1,
     sp_Playoff = 2,
     sp_Allstar = 3

} season_phase_e;

typedef enum
{
     st_None  = 0,
     st_Light = 1,
     st_Dark  = 2,

} skin_tone_e;

typedef struct
{
     int           player_id;
     char          first_name     [ 20 + 1 ];
     char          last_name      [ 20 + 1 ];
     char          first_phonetic [ 20 + 1 ];
     char          last_phonetic  [ 20 + 1 ];
     skin_tone_e   skin_tone;
     handedness_e  handedness;
     playertype_e  player_type;
     int           rookie_season;
     int           longevity;

} player_s;

typedef struct
{
     int           player_id;
     int           season;
     accolade_e    accolade;

} player_accolade_s;

typedef struct
{
     int player_id;
     int speed;
     int control;
     int bunt;
     int fatigue;

} pitcher_s;

typedef struct
{
     int            player_id;
     int            season;
     season_phase_e season_phase;
     int            wins;
     int            losses;
     int            games;
     int            saves;
     float          innings;
     int            hits;
     int            earned_runs;
     int            home_runs;
     int            walks;
     int            strike_outs;

} pitcher_stats_s;

typedef struct
{
     int                 player_id;
     int                 season;
     pitching_accolade_e accolade;

} pitcher_accolade_s;

int players_t_create( sqlite3 *db, const player_s *player );
int players_t_read(   sqlite3 *db,       player_s *player );
int players_t_update( sqlite3 *db, const player_s *player );
int players_t_delete( sqlite3 *db, const player_s *player );

int player_accolades_t_create(         sqlite3 *db,                      const player_accolade_s *player_accolade  );
int player_accolades_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s       *player_accolades );
int player_accolades_t_delete(         sqlite3 *db,                      const player_accolade_s *player_accolade  );

int pitchers_t_create( sqlite3 *db, const pitcher_s *pitcher );
int pitchers_t_read(   sqlite3 *db,       pitcher_s *pitcher );
int pitchers_t_update( sqlite3 *db, const pitcher_s *pitcher );
int pitchers_t_delete( sqlite3 *db, const pitcher_s *pitcher );

int pitcher_stats_t_create( sqlite3 *db, const pitcher_stats_s *pitcher_stats );
int pitcher_stats_t_read(   sqlite3 *db,       pitcher_stats_s *pitcher_stats );
int pitcher_stats_t_update( sqlite3 *db, const pitcher_stats_s *pitcher_stats );
int pitcher_stats_t_delete( sqlite3 *db, const pitcher_stats_s *pitcher_stats );

int pitcher_accolades_t_create(         sqlite3 *db,                      const pitcher_accolade_s *pitcher_accolade  );
int pitcher_accolades_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s        *pitcher_accolades );
int pitcher_accolades_t_delete(         sqlite3 *db,                      const pitcher_accolade_s *pitcher_accolade  );

#endif
