#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewbfiles.h"


static int teamExcluded( const char *team, const char *excluded_teams[], const int num_excluded )
{
     for ( int i = 0; i < num_excluded; ++i )
     {
          if ( strncmp( team, excluded_teams[i], 12 ) == 0 ) return 1;
     }

     return 0;
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playerFile;
     fileleague_s *leagueFile;


     if ( argc < 4 )
     {
	  printf( "Usage: %s <leaguefile> <input playerfile> <output playerfile> [exclude team]...\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     // Load LeagName.Dat
     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file." );

          return EXIT_FAILURE;
     }

     // Load Players.S
     if ( (playerFile = readPlayersFile( argv[2] )) == NULL )
     {
          printf( "Cannot load players file." );

          free( leagueFile );

          return EXIT_FAILURE;
     }

     const char **excluded_teams = &(argv[4]);
     const int    num_excluded   = argc - 4;

     for ( int idx = 0; idx < TOTAL_PLAYERS; ++idx )
     {
	  if ( playerFile[idx].last_name[0] == '\0' ) continue;

          int   team = idx / PLAYERS_PER_TEAM;
          char *teamName = leagueFile->teams[team].name;

          if ( teamExcluded( teamName, excluded_teams, num_excluded ) ) continue;

          unsigned int pos = playerFile[idx].position[0] / 0x10;

          if   ( pos != pos_Pitcher )
          {
               struct batting_s *batting = &(playerFile[idx].filestats.filebatting);

               batting->injury_days[0] = (batting->injury_days[0] > 0) ? batting->injury_days[0] - 1 : 0;
          }
          else
          {
               struct pitching_s *pitching = &(playerFile[idx].filestats.filepitching);

               pitching->injury_days[0] = (pitching->injury_days[0] > 0) ? pitching->injury_days[0] - 1 : 0;
          }
     }

     // Write Players.S
     if ( ! writePlayersFile( argv[3], playerFile ) )
     {
          printf( "Cannot save player changes.\n" );

          free( leagueFile );
          free( playerFile );

          return EXIT_FAILURE;
     }

     free( leagueFile );
     free( playerFile );

     return EXIT_SUCCESS;
}
