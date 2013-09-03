#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "division.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_accolade_s *division_accolade = (division_accolade_s *)data;

     BIND_INT( statement, 1, division_accolade->division_id );
     BIND_INT( statement, 2, division_accolade->season      );
     BIND_INT( statement, 3, division_accolade->accolade    );

     return SQLITE_OK;
}


int division_accolades_t_create( sqlite3 *db, const division_accolade_s *division_accolade )
{
     static char query[]   = "INSERT INTO Division_Accolades_T ( Division_Id, Season, Accolade ) "
          /**/                                         "VALUES ( ?,"         "?,"    "?"      ")";

     return execute_query( db, query, division_accolades_t_create_bindings, division_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_accolades_t_read_by_division_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *division_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *division_id );
}

static int division_accolades_t_read_by_division_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     division_accolade_s division_accolade = { 0 };

     division_accolade.division_id = sqlite3_column_int( statement, 0 );
     division_accolade.season      = sqlite3_column_int( statement, 1 );
     division_accolade.accolade    = sqlite3_column_int( statement, 2 );

     if ( add_to_data_list( data_list, &division_accolade, sizeof(division_accolade), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int division_accolades_t_read_by_division( sqlite3 *db, const int division_id, data_list_s *division_accolades )
{
     static char query[] = "SELECT Division_Id, Season, Accolade FROM Division_Accolades_T WHERE Division_Id = ?";

     return execute_query( db, query, division_accolades_t_read_by_division_bindings, &division_id, division_accolades_t_read_by_division_retrieve, division_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int division_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const division_accolade_s *division_accolade = (division_accolade_s *)data;

     BIND_INT( statement, 1, division_accolade->division_id );
     BIND_INT( statement, 2, division_accolade->season      );
     BIND_INT( statement, 3, division_accolade->accolade    );

     return SQLITE_OK;
}


int division_accolades_t_delete( sqlite3 *db, const division_accolade_s *division_accolade )
{
     static char query[] = "DELETE FROM Division_Accolades_T WHERE Division_Id = ? AND Season = ? AND Accolade = ?";

     return execute_query( db, query, division_accolades_t_delete_bindings, division_accolade, NULL, NULL );
}
