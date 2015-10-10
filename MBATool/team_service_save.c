#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"
#include "services.h"


static int upsert_team_batting_stats( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     int rc;

     if ( (rc = team_batting_stats_t_create( db, team_batting_stats )) == SQLITE_CONSTRAINT )
     {
          return team_batting_stats_t_update( db, team_batting_stats );
     }

     return rc;
}

static int upsert_team_pitching_stats( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     int rc;

     if ( (rc = team_pitching_stats_t_create( db, team_pitching_stats )) == SQLITE_CONSTRAINT )
     {
          return team_pitching_stats_t_update( db, team_pitching_stats );
     }

     return rc;
}

static int upsert_team_versus_stats( sqlite3 *db, const team_versus_stats_s *team_versus_stats )
{
     int rc;

     if ( (rc = team_versus_stats_t_create( db, team_versus_stats )) == SQLITE_CONSTRAINT )
     {
          return team_versus_stats_t_update( db, team_versus_stats );
     }

     return rc;
}

static int upsert_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     if ( (rc = team_stats_t_create( db, team_stats )) == SQLITE_CONSTRAINT )
     {
          return team_stats_t_update( db, team_stats );
     }

     return rc;
}

static int upsert_team( sqlite3 *db, const team_s *team )
{
     int rc;

     if ( (rc = teams_t_create( db, team )) == SQLITE_CONSTRAINT )
     {
          return teams_t_update( db, team );
     }

     return rc;
}

static int save_team_accolades( sqlite3 *db, const team_accolade_s *team_accolades )
{
     int rc;

     if ( team_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; team_accolades[i].team_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( team_accolades_t_create( db, &team_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int save_team_batting_stats( sqlite3 *db, const team_batting_stats_s *team_batting_stats )
{
     int rc;

     if ( team_batting_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_batting_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_batting_stats( db, &team_batting_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_team_pitching_stats( sqlite3 *db, const team_pitching_stats_s *team_pitching_stats )
{
     int rc;

     if ( team_pitching_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_pitching_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_pitching_stats( db, &team_pitching_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_team_versus_stats( sqlite3 *db, const team_versus_stats_s *team_versus_stats )
{
     int rc;

     if ( team_versus_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_versus_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_versus_stats( db, &team_versus_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     if ( team_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_stats( db, &team_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_team_players( sqlite3 *db, const team_player_s *team_players )
{
     int rc;

     if ( team_players == NULL ) return SQLITE_OK;

     for ( int i = 0; team_players[i].team_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( team_players_t_create( db, &team_players[i] ) );
     }

     return SQLITE_OK;
}

int save_team( sqlite3 *db, const team_s *team )
{
     int rc;

     TRY( save_team_players(        db, team->players        ) );
     TRY( save_team_stats(          db, team->stats          ) );
     TRY( save_team_versus_stats(   db, team->versus_stats ) );
     TRY( save_team_pitching_stats( db, team->pitching_stats ) );
     TRY( save_team_batting_stats(  db, team->batting_stats  ) );
     TRY( save_team_accolades(      db, team->accolades      ) );

     return upsert_team( db, team );
}
