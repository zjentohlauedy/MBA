#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewbfiles.h"


typedef struct
{
     char name[12 + 1];
     int  wins;
     int  losses;
} team_s;

typedef struct
{
     char   name[12 + 1];
     team_s teams[TEAMS_PER_DIVISION];
} division_s;

typedef struct
{
     char       name[12 + 1];
     division_s divisions[DIVISIONS_PER_LEAGUE];
} league_s;


int compareTeams(const void *t1, const void *t2)
{
     team_s *team1 = (team_s *)t1;
     team_s *team2 = (team_s *)t2;

     if ( team1->wins != team2->wins ) return -(team1->wins - team2->wins);

     if ( team1->losses != team2->losses ) return team1->losses - team2->losses;

     return strcmp( team1->name, team2->name );
}


int main( const int argc, const char *argv[] )
{
     fileleague_s *leagueFile;
     league_s      leagues[TOTAL_LEAGUES];
     int           games;

     if ( argc < 3 )
     {
	  printf( "Usage: %s <league file> <# of games>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file." );

          return EXIT_FAILURE;
     }

     games = atoi( argv[2] );

     printf( "Checking for teams without %d games...\n", games );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          for ( int j = 0; j < DIVISIONS_PER_LEAGUE; ++j )
          {
               for ( int k = 0; k < TEAMS_PER_DIVISION; ++k )
               {
                    int x = k + (j * TEAMS_PER_DIVISION) + (i * TEAMS_PER_LEAGUE);

                    fileteam_s *team = &(leagueFile->teams[x]);

                    int g = team->wins[0] + team->losses[0];

                    if ( g != games )
                    {
                         printf( "%-12s %d games\n", team->name, g );
                    }
               }
          }
     }

     free( leagueFile );

     return EXIT_SUCCESS;
}
