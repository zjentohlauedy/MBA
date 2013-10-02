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

int save_league_accolades( sqlite3 *db, const league_accolade_s *league_accolades )
{
     int rc;

     for ( int i = 0; league_accolades[i].league_id >= 0; ++i )
     {
          if ( (rc = league_accolades_t_create( db, &league_accolades[i] )) != SQLITE_OK )
          {
               if ( rc != SQLITE_CONSTRAINT ) return rc;
          }
     }

     return SQLITE_OK;
}

int save_league_stats( sqlite3 *db, const league_stats_s *league_stats )
{
     int rc;

     for ( int i = 0; league_stats[i].league_id >= 0; ++i )
     {
          if ( (rc = league_stats_t_create( db, &league_stats[i] )) == SQLITE_CONSTRAINT )
          {
               if ( (rc = league_stats_t_update( db, &league_stats[i] )) != SQLITE_OK ) return rc;
          }
          else if ( rc != SQLITE_OK )
          {
               return rc;
          }
     }

     return SQLITE_OK;
}

int save_league_divisions( sqlite3 *db, const league_division_s *league_divisions )
{
     int rc;

     for ( int i = 0; league_divisions[i].league_id >= 0; ++i )
     {
          if ( (rc = league_divisions_t_create( db, &league_divisions[i] )) != SQLITE_OK )
          {
               if ( rc != SQLITE_CONSTRAINT ) return rc;
          }
     }

     return SQLITE_OK;
}

int save_league( sqlite3 *db, const league_s *league )
{
     int rc;

     if ( league->divisions != NULL )
     {
          if ( (rc = save_league_divisions( db, league->divisions )) != SQLITE_OK ) return rc;
     }

     if ( league->stats != NULL )
     {
          if ( (rc = save_league_stats( db, league->stats )) != SQLITE_OK ) return rc;
     }

     if ( league->accolades != NULL )
     {
          if ( (rc = save_league_accolades( db, league->accolades )) != SQLITE_OK ) return rc;
     }

     if ( (rc = leagues_t_create( db, league )) == SQLITE_CONSTRAINT )
     {
          return leagues_t_update( db, league );
     }

     return rc;
}

int remove_league_accolades( sqlite3 *db, const league_accolade_s *league_accolades )
{
     int rc;

     for ( int i = 0; league_accolades[i].league_id >= 0; ++i )
     {
          if ( (rc = league_accolades_t_delete( db, &league_accolades[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

int remove_league_stats( sqlite3 *db, const league_stats_s *league_stats )
{
     int rc;

     for ( int i = 0; league_stats[i].league_id >= 0; ++i )
     {
          if ( (rc = league_stats_t_delete( db, &league_stats[i] )) == SQLITE_CONSTRAINT ) return rc;
     }

     return SQLITE_OK;
}

int remove_league_divisions( sqlite3 *db, const league_division_s *league_divisions )
{
     int rc;

     for ( int i = 0; league_divisions[i].league_id >= 0; ++i )
     {
          if ( (rc = league_divisions_t_delete( db, &league_divisions[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

int remove_league( sqlite3 *db, const league_s *league )
{
     int rc;

     if ( league->divisions != NULL )
     {
          if ( (rc = remove_league_divisions( db, league->divisions )) != SQLITE_OK ) return rc;
     }

     if ( league->stats != NULL )
     {
          if ( (rc = remove_league_stats( db, league->stats )) != SQLITE_OK ) return rc;
     }

     if ( league->accolades != NULL )
     {
          if ( (rc = remove_league_accolades( db, league->accolades )) != SQLITE_OK ) return rc;
     }

     return leagues_t_delete( db, league );
}

void free_league( league_s *league )
{
     if ( league->divisions != NULL ) free( league->divisions );
     if ( league->stats     != NULL ) free( league->stats     );
     if ( league->accolades != NULL ) free( league->accolades );

     free( league );
}
