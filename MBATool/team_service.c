#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"


static team_accolade_s *get_team_accolades( sqlite3 *db, const int team_id )
{
     static team_accolade_s sentinel = TEAM_ACCOLADE_SENTINEL;

     data_list_s list = { 0 };

     if ( team_accolades_t_read_by_team( db, team_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_accolade_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static team_batting_stats_s *get_team_batting_stats( sqlite3 *db, const int team_id )
{
     static team_batting_stats_s sentinel = TEAM_BATTING_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( team_batting_stats_t_read_by_team( db, team_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_batting_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static team_pitching_stats_s *get_team_pitching_stats( sqlite3 *db, const int team_id )
{
     static team_pitching_stats_s sentinel = TEAM_PITCHING_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( team_pitching_stats_t_read_by_team( db, team_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_pitching_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static team_stats_s *get_team_stats( sqlite3 *db, const int team_id )
{
     static team_stats_s sentinel = TEAM_STATS_SENTINEL;

     data_list_s list = { 0 };

     if ( team_stats_t_read_by_team( db, team_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_stats_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static team_player_s *get_team_players( sqlite3 *db, const int team_id )
{
     static team_player_s sentinel = TEAM_PLAYER_SENTINEL;

     data_list_s list = { 0 };

     if ( team_players_t_read_by_team( db, team_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_player_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

team_s *get_team( sqlite3 *db, const int team_id )
{
     team_s *team = NULL;

     if ( (team = malloc( sizeof(team_s) )) == NULL ) return NULL;

     memset( team, '\0', sizeof(team_s) );

     team->team_id = team_id;

     if ( teams_t_read( db, team ) != SQLITE_OK )
     {
          free( team );

          return NULL;
     }

     team->players        = get_team_players(        db, team_id );
     team->stats          = get_team_stats(          db, team_id );
     team->pitching_stats = get_team_pitching_stats( db, team_id );
     team->batting_stats  = get_team_batting_stats(  db, team_id );
     team->accolades      = get_team_accolades(      db, team_id );

     return team;
}

static int save_team_accolades( sqlite3 *db, const team_accolade_s *team_accolades )
{
     int rc;

     for ( int i = 0; team_accolades[i].team_id >= 0; ++i )
     {
          if ( (rc = team_accolades_t_create( db, &team_accolades[i] )) != SQLITE_OK )
          {
               if ( rc != SQLITE_CONSTRAINT ) return rc;
          }
     }

     return SQLITE_OK;
}

static int save_team_batting_stats( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     int rc;

     for ( int i = 0; team_batting_stats[i].team_id >= 0; ++i )
     {
          if ( (rc = team_batting_stats_t_create( db, &team_batting_stats[i] )) == SQLITE_CONSTRAINT )
          {
               if ( (rc = team_batting_stats_t_update( db, &team_batting_stats[i] )) != SQLITE_OK ) return rc;
          }
          else if ( rc != SQLITE_OK )
          {
               return rc;
          }
     }

     return SQLITE_OK;
}

static int save_team_pitching_stats( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     int rc;

     for ( int i = 0; team_pitching_stats[i].team_id >= 0; ++i )
     {
          if ( (rc = team_pitching_stats_t_create( db, &team_pitching_stats[i] )) == SQLITE_CONSTRAINT )
          {
               if ( (rc = team_pitching_stats_t_update( db, &team_pitching_stats[i] )) != SQLITE_OK ) return rc;
          }
          else if ( rc != SQLITE_OK )
          {
               return rc;
          }
     }

     return SQLITE_OK;
}

static int save_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     for ( int i = 0; team_stats[i].team_id >= 0; ++i )
     {
          if ( (rc = team_stats_t_create( db, &team_stats[i] )) == SQLITE_CONSTRAINT )
          {
               if ( (rc = team_stats_t_update( db, &team_stats[i] )) != SQLITE_OK ) return rc;
          }
          else if ( rc != SQLITE_OK )
          {
               return rc;
          }
     }

     return SQLITE_OK;
}

static int save_team_players( sqlite3 *db, const team_player_s *team_players )
{
     int rc;

     for ( int i = 0; team_players[i].team_id >= 0; ++i )
     {
          if ( (rc = team_players_t_create( db, &team_players[i] )) != SQLITE_OK )
          {
               if ( rc != SQLITE_CONSTRAINT ) return rc;
          }
     }

     return SQLITE_OK;
}

int save_team( sqlite3 *db, const team_s *team )
{
     int rc;

     if ( team->players != NULL )
     {
          if ( (rc = save_team_players( db, team->players )) != SQLITE_OK ) return rc;
     }

     if ( team->stats != NULL )
     {
          if ( (rc = save_team_stats( db, team->stats )) != SQLITE_OK ) return rc;
     }

     if ( team->pitching_stats != NULL )
     {
          if ( (rc = save_team_pitching_stats( db, team->pitching_stats )) != SQLITE_OK ) return rc;
     }

     if ( team->batting_stats != NULL )
     {
          if ( (rc = save_team_batting_stats( db, team->batting_stats )) != SQLITE_OK ) return rc;
     }

     if ( team->accolades != NULL )
     {
          if ( (rc = save_team_accolades( db, team->accolades )) != SQLITE_OK ) return rc;
     }

     if ( (rc = teams_t_create( db, team )) == SQLITE_CONSTRAINT )
     {
          rc = teams_t_update( db, team );
     }

     return rc;
}

static int remove_team_accolades( sqlite3 *db, const team_accolade_s *team_accolades )
{
     int rc;

     for ( int i = 0; team_accolades[i].team_id >= 0; ++i )
     {
          if ( (rc = team_accolades_t_delete( db, &team_accolades[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

static int remove_team_batting_stats( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     int rc;

     for ( int i = 0; team_batting_stats[i].team_id >= 0; ++i )
     {
          if ( (rc = team_batting_stats_t_delete( db, &team_batting_stats[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

static int remove_team_pitching_stats( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     int rc;

     for ( int i = 0; team_pitching_stats[i].team_id >= 0; ++i )
     {
          if ( (rc = team_pitching_stats_t_delete( db, &team_pitching_stats[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

static int remove_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     for ( int i = 0; team_stats[i].team_id >= 0; ++i )
     {
          if ( (rc = team_stats_t_delete( db, &team_stats[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

static int remove_team_players( sqlite3 *db, const team_player_s *team_players )
{
     int rc;

     for ( int i = 0; team_players[i].team_id >= 0; ++i )
     {
          if ( (rc = team_players_t_delete( db, &team_players[i] )) != SQLITE_OK ) return rc;
     }

     return SQLITE_OK;
}

int remove_team( sqlite3 *db, const team_s *team )
{
     int rc;

     if ( team->players != NULL )
     {
          if ( (rc = remove_team_players( db, team->players )) != SQLITE_OK ) return rc;
     }

     if ( team->stats != NULL )
     {
          if ( (rc = remove_team_stats( db, team->stats )) != SQLITE_OK ) return rc;
     }

     if ( team->pitching_stats != NULL )
     {
          if ( (rc = remove_team_pitching_stats( db, team->pitching_stats )) != SQLITE_OK ) return rc;
     }

     if ( team->batting_stats != NULL )
     {
          if ( (rc = remove_team_batting_stats( db, team->batting_stats )) != SQLITE_OK ) return rc;
     }

     if ( team->accolades != NULL )
     {
          if ( (rc = remove_team_accolades( db, team->accolades )) != SQLITE_OK ) return rc;
     }

     return teams_t_delete( db, team );
}

void free_team( team_s *team )
{
     if ( team->players        != NULL ) free( team->players        );
     if ( team->stats          != NULL ) free( team->stats          );
     if ( team->pitching_stats != NULL ) free( team->pitching_stats );
     if ( team->batting_stats  != NULL ) free( team->batting_stats  );
     if ( team->accolades      != NULL ) free( team->accolades      );

     free( team );
}
