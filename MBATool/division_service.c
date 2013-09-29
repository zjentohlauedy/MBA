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

static int save_division_accolades( sqlite3 *db, const division_accolade_s *division_accolades )
{
     int rc;

     for ( int i = 0; division_accolades[i].division_id >= 0; ++i )
     {
          if ( (rc = division_accolades_t_create( db, &division_accolades[i] )) != SQLITE_OK )
          {
               if ( rc != SQLITE_CONSTRAINT ) return rc;
          }
     }

     return SQLITE_OK;
}

static int save_division_stats( sqlite3 *db, const division_stats_s *division_stats )
{
     int rc;

     for ( int i = 0; division_stats[i].division_id >= 0; ++i )
     {
          if ( (rc = division_stats_t_create( db, &division_stats[i] )) == SQLITE_CONSTRAINT )
          {
               if ( (rc = division_stats_t_update( db, &division_stats[i] )) != SQLITE_OK ) return rc;
          }
          else if ( rc != SQLITE_OK )
          {
               return rc;
          }
     }

     return SQLITE_OK;
}

static int save_division_teams( sqlite3 *db, const division_team_s *division_teams )
{
     int rc;

     for ( int i = 0; division_teams[i].division_id >= 0; ++i )
     {
          if ( (rc = division_teams_t_create( db, &division_teams[i] )) != SQLITE_OK )
          {
               if ( rc != SQLITE_CONSTRAINT ) return rc;
          }
     }

     return SQLITE_OK;
}

int save_division( sqlite3 *db, const division_s *division )
{
     int rc;

     if ( division->teams != NULL )
     {
          if ( (rc = save_division_teams( db, division->teams )) != SQLITE_OK ) return rc;
     }

     if ( division->stats != NULL )
     {
          if ( (rc = save_division_stats( db, division->stats )) != SQLITE_OK ) return rc;
     }

     if ( division->accolades != NULL )
     {
          if ( (rc = save_division_accolades( db, division->accolades )) != SQLITE_OK ) return rc;
     }

     if ( (rc = divisions_t_create( db, division )) == SQLITE_CONSTRAINT )
     {
          return divisions_t_update( db, division );
     }

     return rc;
}

static int remove_division_accolades( sqlite3 *db, const division_accolade_s *division_accolades )
{
     int rc;

     for ( int i = 0; division_accolades[i].division_id >= 0; ++i )
     {
          if ( (rc = division_accolades_t_delete( db, &division_accolades[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

static int remove_division_stats( sqlite3 *db, const division_stats_s *division_stats )
{
     int rc;

     for ( int i = 0; division_stats[i].division_id >= 0; ++i )
     {
          if ( (rc = division_stats_t_delete( db, &division_stats[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

static int remove_division_teams( sqlite3 *db, const division_team_s *division_teams )
{
     int rc;

     for ( int i = 0; division_teams[i].division_id >= 0; ++i )
     {
          if ( (rc = division_teams_t_delete( db, &division_teams[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

int remove_division( sqlite3 *db, const division_s *division )
{
     int rc;

     if ( division->teams != NULL )
     {
          if ( (rc = remove_division_teams( db, division->teams )) != SQLITE_OK ) return rc;
     }

     if ( division->stats != NULL )
     {
          if ( (rc = remove_division_stats( db, division->stats )) != SQLITE_OK ) return rc;
     }

     if ( division->accolades != NULL )
     {
          if ( (rc = remove_division_accolades( db, division->accolades )) != SQLITE_OK ) return rc;
     }

     return divisions_t_delete( db, division );
}

void free_division( division_s *division )
{
     if ( division->teams     != NULL ) free( division->teams     );
     if ( division->stats     != NULL ) free( division->stats     );
     if ( division->accolades != NULL ) free( division->accolades );

     free( division );
}
