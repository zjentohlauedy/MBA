#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "league.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int leagues_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_s *league = (league_s *)data;

     BIND_INT( statement, 1, league->league_id );
     BIND_TXT( statement, 2, league->name      );

     return SQLITE_OK;
}


int leagues_t_create( sqlite3 *db, const league_s *league )
{
     static char query[]   = "INSERT INTO Leagues_T ( League_Id, Name )"
          /**/                              "VALUES ( ?,"       "?"  ")";

     return execute_update_old( db, query, leagues_t_create_bindings, league, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By key:
static int leagues_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const league_s *league = (league_s *)data;

     return sqlite3_bind_int( statement, 1, league->league_id );
}

static int leagues_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     league_s *league = (league_s *)data;

     cpytxt( league->name, sqlite3_column_text( statement, 0 ) );

     return SQLITE_OK;
}

int leagues_t_read( sqlite3 *db, league_s *league )
{
     static char query[] = "SELECT Name FROM Leagues_T WHERE League_Id = ?";

     return execute_query( db, query, leagues_t_read_bindings, league, leagues_t_read_retrieve, league );
}

// All:
/*
static int leagues_t_read_all_bindings( sqlite3_stmt *statement, const void *data )
{
     const league_s *league = (league_s *)data;

     return sqlite3_bind_int( statement, 1, league->league_id );
}
*/
static int leagues_t_read_all_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     league_s league = { 0 };

     /**/    league.league_id = sqlite3_column_int(  statement, 0   );
     cpytxt( league.name,       sqlite3_column_text( statement, 1 ) );

     if ( add_to_data_list( data_list, &league, sizeof(league_s), 5 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int leagues_t_read_all( sqlite3 *db, data_list_s *leagues )
{
     static char query[] = "SELECT League_Id, Name FROM Leagues_T";

     return execute_query( db, query, NULL, NULL, leagues_t_read_all_retrieve, leagues );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int leagues_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const league_s *league = (league_s *)data;

     BIND_TXT( statement, 1, league->name );

     BIND_INT( statement, 2, league->league_id );

     return SQLITE_OK;
}


int leagues_t_update( sqlite3 *db, const league_s *league )
{
     static char query[]   = "UPDATE Leagues_T "
          /**/
          /**/               "SET    Name = ? "
          /**/
          /**/               "WHERE  League_Id = ?";

     return execute_update_old( db, query, leagues_t_update_bindings, league, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int leagues_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const league_s *league = (league_s *)data;

     return sqlite3_bind_int( statement, 1, league->league_id );
}


int leagues_t_delete( sqlite3 *db, const league_s *league )
{
     return execute_update_old( db, "DELETE FROM Leagues_T WHERE League_Id = ?", leagues_t_delete_bindings, league, NULL, NULL );
}
