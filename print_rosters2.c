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
     ps_Potential,
     ps_Simulated

} printstyle_e;

static printstyle_e  print_style;


static char *displayAvg( const double avg )
{
     static char buffer[5 + 1];

     sprintf( buffer, "%5.3f", avg );

     if ( buffer[0] == '0' ) buffer[0] = ' ';

     return buffer;
}


static void printPitchingHeader( void )
{
     printf( "   Pitchers              S  C  F  B  W  L   ERA   G  GS  CG SV   INN   AB   H  ER  HR  BB  SO  VSBA   WPCT   DPCT AB/IN  BB/9  HR/9\n" );
     //       P  Pitcher, Pitcher      7  4  5  5 15 12  1.90  34  34  20  0 218.2  836 160  46  16  21 179  .191 055.56 079.41  3.83  7.39 10.00
}


static void printHittingHeader( void )
{
     printf( "      Batters               P  H  B  S  R  A    BA   G  AB   R   H  2B  3B  HR RBI  BB  SO  SB E    SA   OBA   SOA  RPG\n" );
     //       CF XX Base, Darryl         10  5  4  3  4  3 0.345  96 374  84 129  23   7  46 109  29 129   0 0 0.813 0.392 0.000 0.00
}


static void printHitterStats( const hitting_s *stats )
{
     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%3d ", stats->games );

     printf( "%3d %3d %3d ", stats->at_bats, stats->runs, stats->hits );

     printf( "%3d %3d %3d ", stats->doubles, stats->triples, stats->home_runs );

     printf( "%3d %3d %3d ", stats->rbi, stats->walks, stats->strike_outs );

     printf( "%3d %1d ", stats->stolen_bases, stats->errors );

     const int singles  = stats->hits - (stats->doubles + stats->triples + stats->home_runs);
     const int slugging = singles + (2 * stats->doubles) + (3 * stats->triples) + (4 * stats->home_runs);

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)slugging) / ((float)stats->at_bats) : 0 ) );

     printf( "%s ", displayAvg( ((stats->at_bats + stats->walks) > 0) ? (float)(stats->hits + stats->walks) / (float)(stats->at_bats + stats->walks) : 0 ) );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->strike_outs) / ((float)stats->at_bats) : 0 ) );

     const int run_prod = stats->runs + stats->rbi - stats->home_runs;

     printf( "%4.2f", (stats->games > 0) ? ((float)run_prod) / ((float)stats->games) : 0 );
}


static void printHitter( const player_s *player )
{
     char name_buf[ 42 + 1 ];

     const batter_s *b = &(player->data.batter);

     snprintf( name_buf, sizeof(name_buf), "%s, %s", b->last_name, b->first_name );

     printf( "%-2s %-2s %-20s ",
             positionName( b->primary_pos ),
             positionName( b->secondary_pos ),
             /**/          name_buf       );

     printf( "%2d %2d %2d %2d %2d %2d ",
             b->power, b->hit_n_run, b->bunt, b->running, b->range, b->arm );

     printHitterStats( (print_style == ps_Potential) ? &(b->potential) : &(b->simulated) );

     printf( "\n" );
}


static void printPitcherStats( const pitching_s *stats )
{
     printf( "%2d %2d ", stats->wins, stats->losses );

     float norm_inn  = ((int)stats->innings);
     /**/  norm_inn += (stats->innings - norm_inn) / 3.0;

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->earned_runs) / norm_inn * 9.0 : 0 );

     printf( "%3d ", stats->games );
     printf( "%3d ", stats->games_started );
     printf( "%3d ", stats->games_completed );

     printf( "%2d ", stats->saves );

     printf( "%5.1f ", stats->innings );

     printf( "%4d ", stats->at_bats );

     printf( "%3d %3d %3d %3d %3d ", stats->hits, stats->earned_runs, stats->home_runs, stats->walks, stats->strike_outs );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%6.2f ", ((stats->wins + stats->losses) > 0) ? (float)stats->wins / ((float)(stats->wins + stats->losses)) * 100.0 : 0 );

     printf( "%6.2f ", (stats->games > 0) ? ((float)(stats->wins + stats->losses)) / (float)stats->games * 100.0 : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->at_bats) / norm_inn : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->walks) / norm_inn * 9.0 : 0 );

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

     printf( "%2d %2d %2d %2d ", p->speed, p->control, p->fatigue, p->bunt );

     printPitcherStats( (print_style == ps_Potential) ? &(p->potential) : &(p->simulated) );

     printf( "\n" );
}


static void printPlayer( const player_s *player )
{
     if      ( player->type == pt_Pitcher ) printPitcher( player );
     else if ( player->type == pt_Hitter  ) printHitter(  player );
}


static void printRosters( const league_s *leagues )
{
     int  pitchingHeader = 0;
     int  hittingHeader  = 0;
     int  leag;
     int  div;
     int  team;
     int  plyr;

     for ( leag = 0; leag < TOTAL_LEAGUES; ++leag )
     {
	  printf( "%s League\n", leagues[leag].name );
	  printf( "\n" );

	  for ( div = 0; div < DIVISIONS_PER_LEAGUE; ++div )
	  {
	       printf( "%s\n", leagues[leag].divisions[div].name );
	       printf( "\n" );

	       for ( team = 0; team < TEAMS_PER_DIVISION; ++team )
	       {
		    printf( "%s  %3d - %3d\n",
			    leagues[leag].divisions[div].teams[team].name,
			    leagues[leag].divisions[div].teams[team].wins,
			    leagues[leag].divisions[div].teams[team].losses );
		    printf( "\n" );

		    pitchingHeader = 0;
		    hittingHeader  = 0;

		    for ( plyr = 0; plyr < PLAYERS_PER_TEAM; ++plyr )
		    {
			 const player_s *pl = &leagues[leag].divisions[div].teams[team].players[plyr];

			 if ( pl->type == pt_Pitcher  &&  pitchingHeader == 0 )
			 {
			      printPitchingHeader();

			      pitchingHeader = 1;
			 }

			 if ( pl->type == pt_Hitter  &&  hittingHeader == 0 )
			 {
			      printf( "\n" );

			      printHittingHeader();

			      hittingHeader = 1;
			 }

			 printPlayer( pl );
		    }

		    printf( "\n" );
	       }
	  }
     }
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playerFile;
     fileleague_s *leagueFile;
     league_s     *mba;


     if ( argc < 4 )
     {
	  printf( "Usage: %s <leaguefile> <playerfile> <style>.\n", argv[0] );
          printf( "       Where <style> in: S    Simulated Stats\n" );
          printf( "                         P    Potential Stats\n" );

	  return EXIT_SUCCESS;
     }

     print_style = (*argv[3] == 'P') ? ps_Potential : ps_Simulated;

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

     printRosters( mba );

     free( mba );

     return EXIT_SUCCESS;
}
