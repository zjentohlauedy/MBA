#ifndef __INC_LEAGUE_H__
#define __INC_LEAGUE_H__


typedef struct
{
     int   league_id;
     char  name      [20 + 1];

} league_s;

int leagues_t_create( sqlite3 *db, const league_s *league );
int leagues_t_read(   sqlite3 *db,       league_s *league );
int leagues_t_update( sqlite3 *db, const league_s *league );
int leagues_t_delete( sqlite3 *db, const league_s *league );


#endif
