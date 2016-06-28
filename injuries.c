#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ewbfiles.h"
#include "league.h"


#define MAX_REST 4


static boolean_e  print_pitcher_rest;


static void printHitter( const player_s *player )
{
     char name_buf[ 42 + 1 ];

     const batter_s *b = &(player->data.batter);

     snprintf( name_buf, sizeof(name_buf), "%s, %s", b->last_name, b->first_name );

     printf( "%-2s %-20s ",
             positionName( b->primary_pos ),
             /**/          name_buf       );

     printf( "%2d Games", player->injury_days );

     printf( "\n" );
}


static void printPitcher( const player_s *player )
{
     char name_buf[ 42 + 1 ];

     const pitcher_s *p = &(player->data.pitcher);

     snprintf( name_buf, sizeof(name_buf), "%s, %s", p->last_name, p->first_name );

     printf( "%-2s %-20s ",
             positionName( p->primary_pos ),
             /**/          name_buf       );

     if ( print_pitcher_rest == bl_True )
     {
          printf( "%2d Games", player->injury_days );
     }
     else
     {
          printf( "%2d Games", player->injury_days - MAX_REST );
     }

     printf( "\n" );
}


static void printPlayer( const player_s *player )
{
     if      ( player->type == pt_Pitcher ) printPitcher( player );
     else if ( player->type == pt_Hitter  ) printHitter(  player );
}


static void printInjuries( const league_s *leagues )
{
     boolean_e  printTeam = bl_False;
     int  leag;
     int  div;
     int  team;
     int  plyr;

     for ( leag = 0; leag < TOTAL_LEAGUES; ++leag )
     {
	  for ( div = 0; div < DIVISIONS_PER_LEAGUE; ++div )
	  {
	       for ( team = 0; team < TEAMS_PER_DIVISION; ++team )
	       {
                    printTeam = bl_True;

		    for ( plyr = 0; plyr < PLAYERS_PER_TEAM; ++plyr )
		    {
			 const player_s *pl = &leagues[leag].divisions[div].teams[team].players[plyr];

                         if ( pl->injury_days <= 0 ) continue;

                         if ( print_pitcher_rest == bl_False   &&
                              pl->type           == pt_Pitcher &&
                              pl->injury_days    <= MAX_REST      ) continue;

                         if ( printTeam )
                         {
                              printf( "%s:\n", leagues[leag].divisions[div].teams[team].name );

                              printTeam = bl_False;
                         }

                         printPlayer( pl );
		    }

                    if ( ! printTeam ) printf( "\n" );
	       }
	  }
     }
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playerFile;
     fileleague_s *leagueFile;
     league_s     *mba;


     if ( argc < 3 )
     {
	  printf( "Usage: %s <leaguefile> <playerfile>  [options]\n", argv[0] );
          printf( "       Where options in: -p Print pitcher rest injuries.\n" );

	  return EXIT_SUCCESS;
     }

     if ( argc == 4  &&  strcmp( argv[3], "-p" ) == 0 )
     {
          print_pitcher_rest = bl_True;
     }
     else
     {
          print_pitcher_rest = bl_False;
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

          return EXIT_FAILURE;
     }

     // Convert from file format to native format
     if ( (mba = convertFilesToLeagues( leagueFile, playerFile )) == NULL )
     {
          printf( "Cannot convert files to league.\n" );

          free( leagueFile );
	  free( playerFile );

	  return EXIT_FAILURE;
     }

     free( leagueFile );
     free( playerFile );

     printInjuries( mba );

     free( mba );

     return EXIT_SUCCESS;
}
