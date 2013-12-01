#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "file_formats.h"
#include "org.h"

static void freeOrgData( org_data_s *org_data )
{
     if ( org_data->league_data  != NULL ) free( org_data->league_data  );
     if ( org_data->parks_data   != NULL ) free( org_data->parks_data   );
     if ( org_data->players_data != NULL ) free( org_data->players_data );

     if ( org_data->records != NULL ) freeRecords( org_data->records );
}

static season_phase_e mapSeasonPhase( char *phase )
{
     if ( strlen( phase ) > 1 ) return sp_None;

     switch ( *phase )
     {
     case 'A': return sp_Allstar;
     case 'R': return sp_Regular;
     case 'P': return sp_Playoff;
     }

     return sp_None;
}

int main( int argc, char *argv[] )
{
     sqlite3    *db                             = NULL;
     schedule_s *schedule                       = NULL;
     org_data_s  org_data                       = { 0 };
     org_s      *org                            = NULL;
     char        filename_buf[FILENAME_MAX + 1] = { 0 };


     if ( argc < 6 )
     {
          printf( "Usage: %s <database> <ewb files dir> <schedule.csv> <season #> <phase>\n", argv[0] );
          printf( "       Where <phase> in: A    Allstar\n" );
          printf( "                         R    Regular Season\n" );
          printf( "                         P    Playoffs\n"  );

          return EXIT_SUCCESS;
     }

     if ( (org_data.season = atoi( argv[4] )) <= 0 )
     {
          printf( "Season must be a number greater than zero.\n" );

          return EXIT_FAILURE;
     }

     if ( (org_data.season_phase = mapSeasonPhase( argv[5] )) == sp_None )
     {
          printf( "Must provide a valid season phase: A - Allstar; R - Regular Season; P - Playoffs.\n" );

          return EXIT_FAILURE;
     }

     snprintf( filename_buf, sizeof(filename_buf), "%s/LeagName.Dat", argv[2] );

     if ( (org_data.league_data = readLeagueFile( filename_buf )) == NULL )
     {
          printf( "Error reading leagname file: %s\n", getFileUtilsError() );

          freeOrgData( &org_data );

          return EXIT_FAILURE;
     }

     snprintf( filename_buf, sizeof(filename_buf), "%s/parks.dat", argv[2] );

     if ( (org_data.parks_data = readParksFile( filename_buf )) == NULL )
     {
          printf( "Error reading parks file: %s\n", getFileUtilsError() );

          freeOrgData( &org_data );

          return EXIT_FAILURE;
     }

     snprintf( filename_buf, sizeof(filename_buf), "%s/Players.S", argv[2] );

     if ( (org_data.players_data = readPlayersFile( filename_buf )) == NULL )
     {
          printf( "Error reading players file: %s\n", getFileUtilsError() );

          freeOrgData( &org_data );

          return EXIT_FAILURE;
     }

     if ( (schedule = readScheduleCSV( argv[3] )) == NULL )
     {
          printf( "Error reading schedule file: %s\n", getFileUtilsError() );

          freeOrgData( &org_data );

          return EXIT_FAILURE;
     }

     if ( (org_data.records = calculateRecords( schedule, org_data.league_data, org_data.season, org_data.season_phase )) == NULL )
     {
          printf( "Error calculating records: %s\n", getCalculateRecordsError() );

          freeOrgData( &org_data );

          freeSchedule( schedule );

          return EXIT_FAILURE;
     }

     freeSchedule( schedule );

     if ( (org = convertOrg( &org_data )) == NULL )
     {
          printf( "Could not convert files into org.\n" );

          freeOrgData( &org_data );

          return EXIT_FAILURE;
     }

     sqlite3_open( argv[1], &db );
     sqlite3_exec( db, "begin", NULL, NULL, NULL );

     if ( saveOrg( db, org ) != SQLITE_OK )
     {
          printf( "Could not save org: %s.\n", sqlite3_errcode( db ) != 0 ? sqlite3_errmsg( db ) : "Unknown Error" );

          freeOrgData( &org_data );

          sqlite3_exec( db, "rollback", NULL, NULL, NULL );
          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     sqlite3_exec( db, "commit", NULL, NULL, NULL );
     sqlite3_close( db );

     freeOrgData( &org_data );

     return EXIT_SUCCESS;
}
