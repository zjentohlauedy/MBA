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
     printf( "Pitching     W   L   ERA   G SV    INN    H   ER  HR  BB   SO  VSBA INN/G   H/9  HR/9  SO/9  H/HR\n" );
     //       Sabres      67  83  5.84 304 17 1356.6 1728  880 318 325 1842  .282  4.46 11.47  2.11 12.22  5.43
}


static void printHittingHeader( void )
{
     printf( "Batters       BA    G   AB    R    H  2B  3B  HR  RBI  BB   SO  SB  E    SA   OBA   SOA  RPG\n" );
     //       Quasars     .305 1411 5635  841 1719 240  66 301  835 324 1805  12  0  .531  .343  .320 0.97
}


static void printHitterStats( const hitting_s *stats )
{
     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%4d ", stats->games );

     printf( "%4d %4d %4d ", stats->at_bats, stats->runs, stats->hits );

     printf( "%3d %3d %3d ", stats->doubles, stats->triples, stats->home_runs );

     printf( "%4d %3d %4d ", stats->rbi, stats->walks, stats->strike_outs );

     printf( "%3d %2d ", stats->stolen_bases, stats->errors );

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
     const batter_s *b = &(player->data.batter);

     printf( "%-10s ", b->first_name );

     printHitterStats( (print_style == ps_Potential) ? &(b->potential) : &(b->simulated) );

     printf( "\n" );
}


static void printPitcherStats( const pitching_s *stats )
{
     printf( "%3d %3d ", stats->wins, stats->losses );

     float norm_inn  = ((int)stats->innings);
     /**/  norm_inn += (stats->innings - norm_inn) / 3.0;

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->earned_runs) / norm_inn * 9.0 : 0 );

     printf( "%3d ", stats->games );

     printf( "%2d ", stats->saves );

     printf( "%6.1f ", stats->innings );

     printf( "%4d %4d %3d %3d %4d ", stats->hits, stats->earned_runs, stats->home_runs, stats->walks, stats->strike_outs );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%5.2f ", (stats->games > 0) ? norm_inn / ((float)stats->games) : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->hits) / norm_inn * 9.0 : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->home_runs) / norm_inn * 9.0 : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->strike_outs) / norm_inn * 9.0 : 0 );

     printf( "%5.2f", (stats->home_runs > 0) ? ((float)stats->hits) / ((float)stats->home_runs) : stats->hits );
}


static void printPitcher( const player_s *player )
{
     const pitcher_s *p = &(player->data.pitcher);

     printf( "%-10s ", p->first_name );

     printPitcherStats( (print_style == ps_Potential) ? &(p->potential) : &(p->simulated) );

     printf( "\n" );
}


static void printPlayer( const player_s *player )
{
     if      ( player->type == pt_Pitcher ) printPitcher( player );
     else if ( player->type == pt_Hitter  ) printHitter(  player );
}


static void updateHittingTotals( hitting_s *totals, const hitting_s *player )
{
     totals->games        += player->games;
     totals->at_bats      += player->at_bats;
     totals->runs         += player->runs;
     totals->hits         += player->hits;
     totals->doubles      += player->doubles;
     totals->triples      += player->triples;
     totals->home_runs    += player->home_runs;
     totals->rbi          += player->rbi;
     totals->walks        += player->walks;
     totals->strike_outs  += player->strike_outs;
     totals->stolen_bases += player->stolen_bases;
     totals->errors       += player->errors;
}


static void updatePitchingTotals( pitching_s *totals, const pitching_s *player )
{
     totals->wins        += player->wins;
     totals->losses      += player->losses;
     totals->games       += player->games;
     totals->saves       += player->saves;
     totals->innings     += player->innings;
     totals->at_bats     += player->at_bats;
     totals->hits        += player->hits;
     totals->earned_runs += player->earned_runs;
     totals->home_runs   += player->home_runs;
     totals->walks       += player->walks;
     totals->strike_outs += player->strike_outs;
}


static void updateTeamTotals( player_s *totals, const player_s *player )
{
     if      ( player->type == pt_Pitcher )
     {
          updatePitchingTotals( &(totals->data.pitcher.potential), &(player->data.pitcher.potential) );
          updatePitchingTotals( &(totals->data.pitcher.simulated), &(player->data.pitcher.simulated) );
     }
     else if ( player->type == pt_Hitter  )
     {
          updateHittingTotals( &(totals->data.batter.potential), &(player->data.batter.potential) );
          updateHittingTotals( &(totals->data.batter.simulated), &(player->data.batter.simulated) );
     }
}


static void printTeamTotals( const league_s *leagues, const playertype_e type )
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

               if   ( type == pt_Pitcher ) printPitchingHeader();
               else                        printHittingHeader();

	       for ( team = 0; team < TEAMS_PER_DIVISION; ++team )
	       {
                    player_s totals;

                    memset( &totals, '\0', sizeof(player_s) );

                    totals.type = type;

                    strcpy( totals.data.pitcher.first_name, leagues[leag].divisions[div].teams[team].name );

		    for ( plyr = 0; plyr < PLAYERS_PER_TEAM; ++plyr )
		    {
			 const player_s *pl = &leagues[leag].divisions[div].teams[team].players[plyr];

			 if ( pl->type == type )
			 {
                              updateTeamTotals( &totals, pl );
			 }
		    }

                    printPlayer( &totals );
	       }

               printf( "\n" );
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

     printTeamTotals( mba, pt_Pitcher );
     printTeamTotals( mba, pt_Hitter  );

     free( mba );

     return EXIT_SUCCESS;
}
