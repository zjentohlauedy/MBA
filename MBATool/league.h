#ifndef __INC_LEAGUE_H__
#define __INC_LEAGUE_H__

#include "data_list.h"

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

int leagues_t_create( sqlite3 *db, const league_s *league );
int leagues_t_read(   sqlite3 *db,       league_s *league );
int leagues_t_update( sqlite3 *db, const league_s *league );
int leagues_t_delete( sqlite3 *db, const league_s *league );

int league_divisions_t_create(         sqlite3 *db,                      const league_division_s *league_division  );
int league_divisions_t_read_by_league( sqlite3 *db, const int league_id,       data_list_s       *league_divisions );
int league_divisions_t_delete(         sqlite3 *db,                      const league_division_s *league_division  );


#endif
