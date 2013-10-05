#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"
#include "services.h"


static int remove_team_accolades( sqlite3 *db, const team_accolade_s *team_accolades )
{
     int rc;

     if ( team_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; team_accolades[i].team_id >= 0; ++i )
     {
          TRY( team_accolades_t_delete( db, &team_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int remove_team_batting_stats( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     int rc;

     if ( team_batting_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_batting_stats[i].team_id >= 0; ++i )
     {
          TRY( team_batting_stats_t_delete( db, &team_batting_stats[i] ) );
     }

     return SQLITE_OK;
}

static int remove_team_pitching_stats( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     int rc;

     if ( team_pitching_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_pitching_stats[i].team_id >= 0; ++i )
     {
          TRY( team_pitching_stats_t_delete( db, &team_pitching_stats[i] ) );
     }

     return SQLITE_OK;
}

static int remove_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     if ( team_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_stats[i].team_id >= 0; ++i )
     {
          TRY( team_stats_t_delete( db, &team_stats[i] ) );
     }

     return SQLITE_OK;
}

static int remove_team_players( sqlite3 *db, const team_player_s *team_players )
{
     int rc;

     if ( team_players == NULL ) return SQLITE_OK;

     for ( int i = 0; team_players[i].team_id >= 0; ++i )
     {
          TRY( team_players_t_delete( db, &team_players[i] ) );
     }

     return SQLITE_OK;
}

int remove_team( sqlite3 *db, const team_s *team )
{
     int rc;

     TRY( remove_team_players(        db, team->players        ) );
     TRY( remove_team_stats(          db, team->stats          ) );
     TRY( remove_team_pitching_stats( db, team->pitching_stats ) );
     TRY( remove_team_batting_stats(  db, team->batting_stats  ) );
     TRY( remove_team_accolades(      db, team->accolades      ) );

     return teams_t_delete( db, team );
}
