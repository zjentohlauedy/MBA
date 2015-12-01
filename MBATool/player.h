#ifndef __INC_PLAYER_H__
#define __INC_PLAYER_H__

#include <sqlite3.h>
#include "season.h"
#include "data_list.h"

#define PLAYER_ACCOLADE_SENTINEL  { -1, -1, acc_None }
#define PITCHER_STATS_SENTINEL    { -1, -1, sp_None, -1, -1, -1, -1, { -1, -1 }, -1, -1, -1, -1, -1 }
#define PITCHER_ACCOLADE_SENTINEL { -1, -1, pacc_None }
#define BATTER_STATS_SENTINEL     { -1, -1, sp_None, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
#define BATTER_ACCOLADE_SENTINEL  { -1, -1, bacc_None }

typedef enum
{
     acc_None             = 0,
     acc_All_Star         = 1,
     acc_World_Title      = 2,
     acc_League_Title     = 3,
     acc_Division_Title   = 4,
     acc_Best_Record      = 5,
     acc_World_Series_MVP = 6

} accolade_e;

typedef enum
{
     bacc_None                        =  0,
     bacc_Batter_of_the_Year          =  1,
     bacc_Global_Batter_of_the_Year   =  2,
     bacc_World_Batter_of_the_Year    =  3,
     bacc_Atlantic_Batter_of_the_Year =  4,
     bacc_North_Batter_of_the_Year    =  5,
     bacc_Pacific_Batter_of_the_Year  =  6,
     bacc_South_Batter_of_the_Year    =  7,
     bacc_Best_Batting_Average        =  8,
     bacc_Most_Doubles                =  9,
     bacc_Most_Triples                = 10,
     bacc_Most_Home_Runs              = 11,
     bacc_Most_Runs                   = 12,
     bacc_Most_Runs_Batted_In         = 13,
     bacc_Most_Stolen_Bases           = 14,
     bacc_Most_Walks                  = 15,
     bacc_Best_Slugging_Average       = 16,
     bacc_Best_On_Base_Percentage     = 17,
     bacc_Best_Strike_Out_Average     = 18,
     bacc_Best_Runs_per_Game          = 19

} batting_accolade_e;

typedef enum
{
     pacc_None                         =  0,
     pacc_Pitcher_of_the_Year          =  1,
     pacc_Global_Pitcher_of_the_Year   =  2,
     pacc_World_Pitcher_of_the_Year    =  3,
     pacc_Atlantic_Pitcher_of_the_Year =  4,
     pacc_North_Pitcher_of_the_Year    =  5,
     pacc_Pacific_Pitcher_of_the_Year  =  6,
     pacc_South_Pitcher_of_the_Year    =  7,
     pacc_Most_Wins                    =  8,
     pacc_Best_Win_Loss_Ratio          =  9,
     pacc_Best_Win_Percentage          = 10,
     pacc_Best_Earned_Run_Average      = 11,
     pacc_Best_vs_Batting_Average      = 12,
     pacc_Most_Saves                   = 13,
     pacc_Most_Strike_Outs             = 14,
     pacc_Most_Innings_per_Game        = 15,
     pacc_Best_Walks_Hits_Per_Inning   = 16,
     pacc_Most_Strike_Outs_per_Nine    = 17,
     pacc_Best_Home_Runs_per_Nine      = 18,
     pacc_Best_Efficiency              = 19

} pitching_accolade_e;

typedef enum
{
     pos_None        =  0,
     pos_Catcher     =  1,
     pos_FirstBase   =  2,
     pos_SecondBase  =  3,
     pos_ThirdBase   =  4,
     pos_ShortStop   =  5,
     pos_LeftField   =  6,
     pos_CenterField =  7,
     pos_RightField  =  8,
     pos_Infield     =  9,
     pos_Outfield    = 10

} position_e;


typedef enum
{
     hnd_None   = 0,
     hnd_Right  = 1,
     hnd_Left   = 2,
     hnd_Switch = 3

} handedness_e;

typedef enum
{
     pt_None    = 0,
     pt_Pitcher = 1,
     pt_Batter  = 2

} playertype_e;

typedef enum
{
     st_None  = 0,
     st_Light = 1,
     st_Dark  = 2

} skin_tone_e;

typedef struct
{
     int innings;
     int outs;

} innings_s;

typedef struct
{
     int                player_id;
     int                season;
     batting_accolade_e accolade;

} batter_accolade_s;

typedef struct
{
     int            player_id;
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

} batter_stats_s;

typedef struct
{
     int                player_id;
     int                power;
     int                hit_n_run;
     int                bunt;
     int                running;
     int                range;
     int                arm;
     position_e         primary_position;
     position_e         secondary_position;
     batter_stats_s    *stats;
     batter_accolade_s *accolades;

} batter_s;

typedef struct
{
     int                 player_id;
     int                 season;
     pitching_accolade_e accolade;

} pitcher_accolade_s;

typedef struct
{
     int            player_id;
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

} pitcher_stats_s;

typedef struct
{
     int                 player_id;
     int                 speed;
     int                 control;
     int                 bunt;
     int                 fatigue;
     pitcher_stats_s    *stats;
     pitcher_accolade_s *accolades;

} pitcher_s;

typedef struct
{
     int                player_id;
     int                season;
     accolade_e         accolade;

} player_accolade_s;

typedef struct
{
     int                player_id;
     char               first_name      [ 20 + 1 ];
     char               last_name       [ 20 + 1 ];
     char               first_phoenetic [ 20 + 1 ];
     char               last_phoenetic  [ 20 + 1 ];
     skin_tone_e        skin_tone;
     handedness_e       handedness;
     playertype_e       player_type;
     int                rookie_season;
     int                longevity;
     player_accolade_s *accolades;
     union {
          pitcher_s *pitching;
          batter_s  *batting;
     }                  details;

} player_s;


int players_t_create(            sqlite3 *db, const player_s *player );
int players_t_read(              sqlite3 *db,       player_s *player );
int players_t_update(            sqlite3 *db, const player_s *player );
int players_t_update_phoenetics( sqlite3 *db, const player_s *player );
int players_t_delete(            sqlite3 *db, const player_s *player );

int player_accolades_t_create(         sqlite3 *db,                      const player_accolade_s *player_accolade  );
int player_accolades_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s       *player_accolades );
int player_accolades_t_delete(         sqlite3 *db,                      const player_accolade_s *player_accolade  );

int pitchers_t_create( sqlite3 *db, const pitcher_s *pitcher );
int pitchers_t_read(   sqlite3 *db,       pitcher_s *pitcher );
int pitchers_t_update( sqlite3 *db, const pitcher_s *pitcher );
int pitchers_t_delete( sqlite3 *db, const pitcher_s *pitcher );

int pitcher_stats_t_create(         sqlite3 *db,                      const pitcher_stats_s *pitcher_stats );
int pitcher_stats_t_read(           sqlite3 *db,                            pitcher_stats_s *pitcher_stats );
int pitcher_stats_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s     *pitcher_stats );
int pitcher_stats_t_update(         sqlite3 *db,                      const pitcher_stats_s *pitcher_stats );
int pitcher_stats_t_delete(         sqlite3 *db,                      const pitcher_stats_s *pitcher_stats );

int pitcher_accolades_t_create(         sqlite3 *db,                      const pitcher_accolade_s *pitcher_accolade  );
int pitcher_accolades_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s        *pitcher_accolades );
int pitcher_accolades_t_delete(         sqlite3 *db,                      const pitcher_accolade_s *pitcher_accolade  );

int batters_t_create( sqlite3 *db, const batter_s *batter );
int batters_t_read(   sqlite3 *db,       batter_s *batter );
int batters_t_update( sqlite3 *db, const batter_s *batter );
int batters_t_delete( sqlite3 *db, const batter_s *batter );

int batter_stats_t_create(         sqlite3 *db,                      const batter_stats_s *batter_stats );
int batter_stats_t_read(           sqlite3 *db,                            batter_stats_s *batter_stats );
int batter_stats_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s    *batter_stats );
int batter_stats_t_update(         sqlite3 *db,                      const batter_stats_s *batter_stats );
int batter_stats_t_delete(         sqlite3 *db,                      const batter_stats_s *batter_stats );

int batter_accolades_t_create(         sqlite3 *db,                      const batter_accolade_s *batter_accolade  );
int batter_accolades_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s       *batter_accolades );
int batter_accolades_t_delete(         sqlite3 *db,                      const batter_accolade_s *batter_accolade  );

player_s *get_player(    sqlite3 *db, const int       player_id );
int       save_player(   sqlite3 *db, const player_s *player    );
int       remove_player( sqlite3 *db, const player_s *player    );
void      free_player(                      player_s *player    );


#endif
