#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"


league_accolade_s *get_league_accolades( sqlite3 *db, const int league_id )
{
     static league_accolade_s sentinel = LEAGUE_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( league_accolades_t_read_by_league( db, league_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(league_accolade_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

league_stats_s *get_league_stats( sqlite3 *db, const int league_id )
{
     static league_stats_s sentinel = LEAGUE_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( league_stats_t_read_by_league( db, league_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(league_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

league_division_s *get_league_divisions( sqlite3 *db, const int league_id )
{
     static league_division_s sentinel = LEAGUE_DIVISION_SENTINEL;

     data_list_s list = { 0 };

     if ( league_divisions_t_read_by_league( db, league_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(league_division_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

league_s *get_league( sqlite3 *db, const int league_id )
{
     league_s *league = NULL;

     if ( (league = malloc( sizeof(league_s) )) == NULL ) return NULL;

     memset( league, '\0', sizeof(league_s) );

     league->league_id = league_id;

     if ( leagues_t_read( db, league ) != SQLITE_OK )
     {
          free( league );

          return NULL;
     }

     league->divisions = get_league_divisions( db, league_id );
     league->stats     = get_league_stats(     db, league_id );
     league->accolades = get_league_accolades( db, league_id );

     return league;
}

void free_league( league_s *league )
{
     if ( league->divisions != NULL ) free( league->divisions );
     if ( league->stats     != NULL ) free( league->stats     );
     if ( league->accolades != NULL ) free( league->accolades );

     free( league );
}
