#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"

division_accolade_s *get_division_accolades( sqlite3 *db, const int division_id )
{
     static division_accolade_s sentinel = DIVISION_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( division_accolades_t_read_by_division( db, division_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(division_accolade_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

division_stats_s *get_division_stats( sqlite3 *db, const int division_id )
{
     static division_stats_s sentinel = DIVISION_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( division_stats_t_read_by_division( db, division_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(division_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

division_team_s *get_division_teams( sqlite3 *db, const int division_id )
{
     static division_team_s sentinel = DIVISION_TEAM_SENTINEL;

     data_list_s list = { 0 };

     if ( division_teams_t_read_by_division( db, division_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(division_team_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

division_s *get_division( sqlite3 *db, const int division_id )
{
     division_s *division = NULL;
     int         rc;

     if ( (division = malloc( sizeof(division_s) )) == NULL ) return NULL;

     memset( division, '\0', sizeof(division_s) );

     division->division_id = division_id;

     if ( divisions_t_read( db, division ) != SQLITE_OK )
     {
          free( division );

          return NULL;
     }

     division->teams     = get_division_teams(     db, division_id );
     division->stats     = get_division_stats(     db, division_id );
     division->accolades = get_division_accolades( db, division_id );

     return division;
}

int save_division( sqlite3 *db, const division_s *division )
{
     return divisions_t_create( db, division );
}

void free_division( division_s *division )
{
     if ( division->teams     != NULL ) free( division->teams     );
     if ( division->stats     != NULL ) free( division->stats     );
     if ( division->accolades != NULL ) free( division->accolades );

     free( division );
}
