#ifndef __INC_PLAYER_H__
#define __INC_PLAYER_H__

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

int   insert_player(    sqlite3 *db, const player_s *player );
int   get_player(       sqlite3 *db,       player_s *player );
int   update_player(    sqlite3 *db, const player_s *player );
int   delete_player(    sqlite3 *db, const player_s *player );

int   insert_player_accolade(    sqlite3 *db, const player_accolade_s *player_accolade );
int   get_player_accolade(       sqlite3 *db,       player_accolade_s *player_accolade );
int   update_player_accolade(    sqlite3 *db, const player_accolade_s *player_accolade );
int   delete_player_accolade(    sqlite3 *db, const player_accolade_s *player_accolade );

#endif
