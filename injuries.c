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


typedef enum
{
     ps_Fielding,
     ps_Potential,
     ps_Simulated

} printstyle_e;

static printstyle_e  print_style;
static boolean_e  print_pitcher_rest;


static char *displayAvg( const double avg )
{
     static char buffer[5 + 1];

     sprintf( buffer, "%5.3f", avg );

     if ( buffer[0] == '0' ) buffer[0] = ' ';

     return buffer;
}


static void printPitchingHeader( void )
{
     if ( print_style == ps_Fielding )
     {
          printf( "   Pitchers              S  C  F  B     FA\n" );
          //       P  Pitcher, Average      5  5  5  5   .987
     }
     else
     {
          printf( "   Pitchers              S  C  F  B   W  L   ERA   G SV   INN   H  ER  HR  BB  SO  VSBA INN/G   H/9  BB/9  SO/9  HR/9\n" );
          //       P  Alphin, Nick          8  9  7  6  12  5  3.64  24  0 173.1 187  70  27  34 232 0.000  7.21  9.73  1.40 12.07  6.93
     }
}


static void printHittingHeader( void )
{
     if ( print_style == ps_Fielding )
     {
          printf( "   Batters               P  H  B  S  R  A   G   PO   AS  E    FA  POS2   FA2\n" );
          //       C  Batter, Average       5  5  5 10  5  5 123  600   75  9  .987   P   1.000
     }
     else
     {
          printf( "   Batters               P  H  B  S  R  A    BA   G  AB   R   H  2B  3B  HR RBI  BB  SO  SB  E    SA   OBA   SOA  RPG\n" );
          //       CF Base, Darryl         10  5  4  3  4  3 0.345  96 374  84 129  23   7  46 109  29 129   0  0 0.813 0.392 0.000 0.00
     }
}


static void printHitterFielding( const fielding_s *fielding, const position_e secondary_pos )
{
     int total = fielding->put_outs + fielding->assists + fielding->errors;

     printf( "%4d %4d %2d ", fielding->put_outs, fielding->assists, fielding->errors );
     printf( "%s ", displayAvg( (total > 0) ? ((float)(fielding->put_outs + fielding->assists)) / ((float)total) : 0 ) );
     printf( "  %-2s  ", positionName( secondary_pos ) );
     printf( "%s", displayAvg( fielding->secondary_avg ) );
}


static void printHitterStats( const hitting_s *stats )
{
     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%3d ", stats->games );

     printf( "%3d %3d %3d ", stats->at_bats, stats->runs, stats->hits );

     printf( "%3d %3d %3d ", stats->doubles, stats->triples, stats->home_runs );

     printf( "%3d %3d %3d ", stats->rbi, stats->walks, stats->strike_outs );

     printf( "%3d %2d ", stats->stolen_bases, stats->errors );

     int singles  = stats->hits - (stats->doubles + stats->triples + stats->home_runs);
     int slugging = singles + (2 * stats->doubles) + (3 * stats->triples) + (4 * stats->home_runs);

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)slugging) / ((float)stats->at_bats) : 0 ) );

     printf( "%s ", displayAvg( ((stats->at_bats + stats->walks) > 0) ? (float)(stats->hits + stats->walks) / (float)(stats->at_bats + stats->walks) : 0 ) );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->strike_outs) / ((float)stats->at_bats) : 0 ) );

     int run_prod = stats->runs + stats->rbi - stats->home_runs;

     printf( "%4.2f", (stats->games > 0) ? ((float)run_prod) / ((float)stats->games) : 0 );
}


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


static void printPitcherFielding( const pitcher_s *pitcher )
{
     printf( "%s", displayAvg( pitcher->fielding_avg ) );
}


static void printPitcherStats( const pitching_s *stats )
{
     printf( "%2d %2d ", stats->wins, stats->losses );

     float norm_inn  = ((int)stats->innings);
     /**/  norm_inn += (stats->innings - norm_inn) / 3.0;

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->earned_runs) / norm_inn * 9.0 : 0 );

     printf( "%3d ", stats->games );

     printf( "%2d ", stats->saves );

     printf( "%5.1f ", stats->innings );

     printf( "%3d %3d %3d %3d %3d ", stats->hits, stats->earned_runs, stats->home_runs, stats->walks, stats->strike_outs );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%5.2f ", (stats->games > 0) ? norm_inn / ((float)stats->games) : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->hits) / norm_inn * 9.0 : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->walks) / norm_inn * 9.0 : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->strike_outs) / norm_inn * 9.0 : 0 );

     printf( "%5.2f", (norm_inn > 0) ? ((float)stats->home_runs) / norm_inn * 9.0 : 0 );
}


static void printPitcher( const player_s *player )
{
     char name_buf[ 42 + 1 ];

     const pitcher_s *p = &(player->data.pitcher);

     snprintf( name_buf, sizeof(name_buf), "%s, %s", p->last_name, p->first_name );

     printf( "%-2s %-20s ",
             positionName( p->primary_pos ),
             /**/          name_buf       );

     printf( "%2d Games", player->injury_days );

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
                              pl->injury_days    <= 4             ) continue;

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
