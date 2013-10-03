#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"


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
