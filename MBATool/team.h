#ifndef __INC_TEAM_H__
#define __INC_TEAM_H__

#include "data_list.h"

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

typedef struct
{
     int      team_id;
     char     name            [20 + 1];
     char     location        [20 + 1];
     color_e  primary_color;
     color_e  secondary_color;

} team_s;

typedef struct
{
     int team_id;
     int season;
     int player_id;

} team_player_s;


int teams_t_create( sqlite3 *db, const team_s *team );
int teams_t_read(   sqlite3 *db,       team_s *team );
int teams_t_update( sqlite3 *db, const team_s *team );
int teams_t_delete( sqlite3 *db, const team_s *team );

int team_players_t_create(       sqlite3 *db,                    const team_player_s *team_player  );
int team_players_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s   *team_players );
int team_players_t_delete(       sqlite3 *db,                    const team_player_s *team_player  );


#endif
