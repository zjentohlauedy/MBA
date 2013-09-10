#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "league.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_accolade_s *league_accolade = (league_accolade_s *)data;

     BIND_INT( statement, 1, league_accolade->league_id );
     BIND_INT( statement, 2, league_accolade->season    );
     BIND_INT( statement, 3, league_accolade->accolade  );

     return SQLITE_OK;
}


int league_accolades_t_create( sqlite3 *db, const league_accolade_s *league_accolade )
{
     static char query[]   = "INSERT INTO League_Accolades_T ( League_Id, Season, Accolade ) "
          /**/                                       "VALUES ( ?,"       "?,"    "?"      ")";

     return execute_query( db, query, league_accolades_t_create_bindings, league_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_accolades_t_read_by_league_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *league_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *league_id );
}

static int league_accolades_t_read_by_league_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     league_accolade_s league_accolade = { 0 };

     league_accolade.league_id = sqlite3_column_int( statement, 0 );
     league_accolade.season    = sqlite3_column_int( statement, 1 );
     league_accolade.accolade  = sqlite3_column_int( statement, 2 );

     if ( add_to_data_list( data_list, &league_accolade, sizeof(league_accolade_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int league_accolades_t_read_by_league( sqlite3 *db, const int league_id, data_list_s *league_accolades )
{
     static char query[] = "SELECT League_Id, Season, Accolade FROM League_Accolades_T WHERE League_Id = ?";

     return execute_query( db, query, league_accolades_t_read_by_league_bindings, &league_id, league_accolades_t_read_by_league_retrieve, league_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int league_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_accolade_s *league_accolade = (league_accolade_s *)data;

     BIND_INT( statement, 1, league_accolade->league_id );
     BIND_INT( statement, 2, league_accolade->season    );
     BIND_INT( statement, 3, league_accolade->accolade  );

     return SQLITE_OK;
}


int league_accolades_t_delete( sqlite3 *db, const league_accolade_s *league_accolade )
{
     static char query[] = "DELETE FROM League_Accolades_T WHERE League_Id = ? AND Season = ? AND Accolade = ?";

     return execute_query( db, query, league_accolades_t_delete_bindings, league_accolade, NULL, NULL );
}
