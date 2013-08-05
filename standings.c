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


int main( int argc, char *argv[] )
{
     fileleague_s *leagueFile;
     league_s      leagues[TOTAL_LEAGUES];

     if ( argc < 2 )
     {
	  printf( "Usage: %s <league file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file." );

          return EXIT_FAILURE;
     }

     strncpy( leagues[0].name, leagueFile->league1, 12 );
     strncpy( leagues[1].name, leagueFile->league2, 12 );

     strncpy( leagues[0].divisions[0].name, leagueFile->division1, 12 );
     strncpy( leagues[0].divisions[1].name, leagueFile->division2, 12 );
     strncpy( leagues[1].divisions[0].name, leagueFile->division3, 12 );
     strncpy( leagues[1].divisions[1].name, leagueFile->division4, 12 );


     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          for ( int j = 0; j < DIVISIONS_PER_LEAGUE; ++j )
          {
               for ( int k = 0; k < TEAMS_PER_DIVISION; ++k )
               {
                    int x = k + (j * TEAMS_PER_DIVISION) + (i * TEAMS_PER_LEAGUE);

                    fileteam_s *team = &(leagueFile->teams[x]);

                    strncpy( leagues[i].divisions[j].teams[k].name,        team->name,      12 );
                    /**/     leagues[i].divisions[j].teams[k].wins     =   team->wins[0];
                    /**/     leagues[i].divisions[j].teams[k].losses   =   team->losses[0];
               }

               qsort( leagues[i].divisions[j].teams, TEAMS_PER_DIVISION, sizeof(team_s), compareTeams );
          }
     }

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          printf( "%s League                                      ", leagues[i].name );
     }

     printf( "\n" );
     printf( "-                                                 -\n" );

     for ( int i = 0; i < TOTAL_LEAGUES; ++i )
     {
          for ( int j = 0; j < DIVISIONS_PER_LEAGUE; ++j )
          {
               printf( "%-12s   W   L     ", leagues[i].divisions[j].name );
          }
     }

     printf( "\n" );
     printf( "-              -   -     -              -   -     -              -   -     -              -   -\n" );

     for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
     {
          for ( int j = 0; j < TOTAL_LEAGUES; ++j )
          {
               for ( int k = 0; k < DIVISIONS_PER_LEAGUE; ++k )
               {
                    printf( "%-12s %3d %3d     ",
                            leagues[j].divisions[k].teams[i].name,
                            leagues[j].divisions[k].teams[i].wins,
                            leagues[j].divisions[k].teams[i].losses );
               }
          }

          printf( "\n" );
     }

     free( leagueFile );

     return EXIT_SUCCESS;
}
