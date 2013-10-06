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

static team_s *get_team_details( sqlite3 *db, const int team_id )
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

     return team;
}

team_s *get_team( sqlite3 *db, const int team_id )
{
     team_s *team = NULL;

     if ( (team = get_team_details( db, team_id )) == NULL ) return NULL;

     team->players        = get_team_players(        db, team_id );
     team->stats          = get_team_stats(          db, team_id );
     team->pitching_stats = get_team_pitching_stats( db, team_id );
     team->batting_stats  = get_team_batting_stats(  db, team_id );
     team->accolades      = get_team_accolades(      db, team_id );

     return team;
}
