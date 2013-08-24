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

#endif
