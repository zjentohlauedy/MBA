#ifndef __INC_DIVISION_H__
#define __INC_DIVISION_H__


typedef struct
{
     int   division_id;
     char  name        [20 + 1];

} division_s;


int divisions_t_create( sqlite3 *db, const division_s *division );
int divisions_t_read(   sqlite3 *db,       division_s *division );
int divisions_t_update( sqlite3 *db, const division_s *division );
int divisions_t_delete( sqlite3 *db, const division_s *division );


#endif
