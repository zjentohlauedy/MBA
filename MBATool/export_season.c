#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include "file_formats.h"
#include "org.h"

int main( const int argc, const char *argv[] )
{
     fileleagname_s *leagname_file                        = NULL;
     fileplayer_s   *players_file                         = NULL;
     sqlite3        *db                                   = NULL;
     org_s          *org                                  = NULL;
     char            leagname_filename [FILENAME_MAX + 1] = { 0 };
     char            players_filename  [FILENAME_MAX + 1] = { 0 };
     int             season                               =   0;


     if ( argc < 4 )
     {
          printf( "Usage: %s <database> <ewb files dir> <season #>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     if ( (season = atoi( argv[3] )) <= 0 )
     {
          printf( "Season must be a number greater than zero.\n" );

          return EXIT_FAILURE;
     }

     sprintf( leagname_filename, "%s/LeagName.Dat", argv[2] );
     sprintf( players_filename,  "%s/Players.S",    argv[2] );

     sqlite3_open( argv[1], &db );

     if ( (org = getOrg( db, season )) == NULL )
     {
          printf( "Unable to read organization from database for season <%d>\n", season );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     if ( (leagname_file = generateFileLeagName( org )) == NULL )
     {
          printf( "Unable to generate league file from organaization data.\n" );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     srand( time( NULL ) );

     if ( (players_file = generateFilePlayers( org, season )) == NULL )
     {
          printf( "Unable to generate players file from organization data.\n" );

          free( leagname_file );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     sqlite3_close( db );

     if ( ! writeLeagueFile( leagname_filename, leagname_file ) )
     {
          printf( "Cannot write leagname file <%s>: %s\n", leagname_filename, getFileUtilsError() );

          free( leagname_file );
          free( players_file );

          return EXIT_FAILURE;
     }

     if ( ! writePlayersFile( players_filename, players_file ) )
     {
          printf( "Cannot write players file <%s>: %s\n", players_filename, getFileUtilsError() );

          free( leagname_file );
          free( players_file );

          return EXIT_FAILURE;
     }

     free( leagname_file );
     free( players_file );

     return EXIT_SUCCESS;
}
