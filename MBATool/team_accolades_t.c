#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_accolade_s *team_accolade = (team_accolade_s *)data;

     BIND_INT( statement, 1, team_accolade->team_id  );
     BIND_INT( statement, 2, team_accolade->season   );
     BIND_INT( statement, 3, team_accolade->accolade );

     return SQLITE_OK;
}


int team_accolades_t_create( sqlite3 *db, const team_accolade_s *team_accolade )
{
     static char query[]   = "INSERT INTO Team_Accolades_T ( Team_Id, Season, Accolade ) "
          /**/                                     "VALUES ( ?,"     "?,"    "?"      ")";

     return execute_update_old( db, query, team_accolades_t_create_bindings, team_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_accolades_t_read_by_team_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_accolades_t_read_by_team_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_accolade_s team_accolade = { 0 };

     team_accolade.team_id  = sqlite3_column_int( statement, 0 );
     team_accolade.season   = sqlite3_column_int( statement, 1 );
     team_accolade.accolade = sqlite3_column_int( statement, 2 );

     if ( add_to_data_list( data_list, &team_accolade, sizeof(team_accolade_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_accolades_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_accolades )
{
     static char query[] = "SELECT Team_Id, Season, Accolade FROM Team_Accolades_T WHERE Team_Id = ?";

     return execute_query( db, query, team_accolades_t_read_by_team_bindings, &team_id, team_accolades_t_read_by_team_retrieve, team_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_accolade_s *team_accolade = (team_accolade_s *)data;

     BIND_INT( statement, 1, team_accolade->team_id  );
     BIND_INT( statement, 2, team_accolade->season   );
     BIND_INT( statement, 3, team_accolade->accolade );

     return SQLITE_OK;
}


int team_accolades_t_delete( sqlite3 *db, const team_accolade_s *team_accolade )
{
     static char query[] = "DELETE FROM Team_Accolades_T WHERE Team_Id = ? AND Season = ? AND Accolade = ?";

     return execute_update_old( db, query, team_accolades_t_delete_bindings, team_accolade, NULL, NULL );
}
