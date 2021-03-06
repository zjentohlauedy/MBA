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
     ps_Ratings,
     ps_Simulated

} printstyle_e;

static printstyle_e  print_style;


static char *displayHand( const int hand )
{
     switch ( hand )
     {
     case 0: return "R";
     case 1: return "L";
     case 2: return "S";
     }

     return "X";
}

static char *displayAvg( const double avg )
{
     static char buffer[5 + 1];

     sprintf( buffer, "%5.3f", avg );

     if ( buffer[0] == '0' ) buffer[0] = ' ';

     return buffer;
}


static void printPitchingHeader( void )
{
     switch ( print_style )
     {
     case ps_Fielding:  printf( "   Pitchers                FA\n" ); break;
          //                     P  Pitcher, Average      .987
     case ps_Ratings:   printf( "   Pitchers                ID H  S  C  F  B  L\n" ); break;
          //                     P  Bosanquet, Simon       123 L  4 10  9  8  8
     case ps_Potential:
     case ps_Simulated: printf( "   Pitchers              W  L   ERA   G SV   INN   H  ER  HR  BB  SO  VSBA INN/G  WHIP  SO/9  HR/9    EFF\n" ); break;
          //                     P  Alphin, Nick         12  5  3.64  24  0 173.1 187  70  27  34 232 0.000  7.21 0.973 12.07  6.93 -12.34
     }
}


static void printHittingHeader( void )
{
     switch ( print_style )
     {
     case ps_Fielding:  printf( "   Batters                G   PO   AS  E    FA  POS2   FA2\n" ); break;
          //                     C  Batter, Average      123  600   75  9  .987   P   1.000
     case ps_Ratings:   printf( "   Batters                 ID H  P  H  B  S  R  A  L\n" ); break;
          //                     2B Hammerman, Howard      123 R  9  4  4  4  7  9
     case ps_Potential:
     case ps_Simulated: printf( "   Batters                 BA   G  AB   R   H  2B  3B  HR RBI  BB  SO  SB  E    SA   OBA   SOA  RPG\n" ); break;
          //                     CF Base, Darryl          .345  96 374  84 129  23   7  46 109  29 129   0  0 0.813 0.392 0.000 0.00
     }
}


static void printHitterFielding( const batter_s *batter )
{
     const fielding_s *fielding = &(batter->fielding);

     const int total = fielding->put_outs + fielding->assists + fielding->errors;

     printf( "%3d ", batter->potential.games );
     printf( "%4d %4d %2d ", fielding->put_outs, fielding->assists, fielding->errors );
     printf( "%s ", displayAvg( (total > 0) ? ((float)(fielding->put_outs + fielding->assists)) / ((float)total) : 0 ) );
     printf( "  %-2s  ", positionName( batter->secondary_pos ) );
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

     const int singles  = stats->hits - (stats->doubles + stats->triples + stats->home_runs);
     const int slugging = singles + (2 * stats->doubles) + (3 * stats->triples) + (4 * stats->home_runs);

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)slugging) / ((float)stats->at_bats) : 0 ) );

     printf( "%s ", displayAvg( ((stats->at_bats + stats->walks) > 0) ? (float)(stats->hits + stats->walks) / (float)(stats->at_bats + stats->walks) : 0 ) );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->strike_outs) / ((float)stats->at_bats) : 0 ) );

     const int run_prod = stats->runs + stats->rbi - stats->home_runs;

     printf( "%4.2f", (stats->games > 0) ? ((float)run_prod) / ((float)stats->games) : 0 );
}


static void printHitterRatings( const player_s *player )
{
     printf( "%5d ", player->player_id );

     printf( "%s ", displayHand( player->hand ) );

     const batter_s *batter = &(player->data.batter);

     printf( "%2d %2d %2d %2d %2d %2d %2d",
             batter->power, batter->hit_n_run, batter->bunt, batter->running, batter->range, batter->arm, batter->longevity );
}


static void printHitter( const player_s *player )
{
     char name_buf[ 42 + 1 ];

     const batter_s *b = &(player->data.batter);

     snprintf( name_buf, sizeof(name_buf), "%s, %s", b->last_name, b->first_name );

     printf( "%-2s %-20s ",
             positionName( b->primary_pos ),
             /**/          name_buf       );

     switch ( print_style )
     {
     case ps_Fielding:  printHitterFielding( b );            break;
     case ps_Potential: printHitterStats( &(b->potential) ); break;
     case ps_Simulated: printHitterStats( &(b->simulated) ); break;
     case ps_Ratings:   printHitterRatings( player );        break;
     }

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
     /**/  norm_inn += (stats->innings - norm_inn) / 0.3;

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->earned_runs) / norm_inn * 9.0 : 0 );

     printf( "%3d ", stats->games );

     printf( "%2d ", stats->saves );

     printf( "%5.1f ", stats->innings );

     printf( "%3d %3d %3d %3d %3d ", stats->hits, stats->earned_runs, stats->home_runs, stats->walks, stats->strike_outs );

     printf( "%s ", displayAvg( (stats->at_bats > 0) ? ((float)stats->hits) / ((float)stats->at_bats) : 0 ) );

     printf( "%5.2f ", (stats->games > 0) ? norm_inn / ((float)stats->games) : 0 );

     printf( "%5.3f ", (norm_inn > 0) ? ((float)stats->hits + (float)stats->walks) / norm_inn : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->strike_outs) / norm_inn * 9.0 : 0 );

     printf( "%5.2f ", (norm_inn > 0) ? ((float)stats->home_runs) / norm_inn * 9.0 : 0 );

     int eff = ((int)stats->innings - stats->hits) + (stats->strike_outs - stats->hits);

//     printf( "%+4d", ((int)stats->innings - stats->hits) + (stats->strike_outs - stats->hits) );
     printf( "%+6.2f", (norm_inn > 0) ? ((float)eff) / norm_inn * 9.0 : 0 );
}


static void printPitcherRatings( const player_s *player )
{
     printf( "%5d ", player->player_id );

     printf( "%s ", displayHand( player->hand ) );

     const pitcher_s *pitcher = &(player->data.pitcher);

     printf( "%2d %2d %2d %2d %2d", pitcher->speed, pitcher->control, pitcher->fatigue, pitcher->bunt, pitcher->longevity );
}


static void printPitcher( const player_s *player )
{
     char name_buf[ 42 + 1 ];

     const pitcher_s *p = &(player->data.pitcher);

     snprintf( name_buf, sizeof(name_buf), "%s, %s", p->last_name, p->first_name );

     printf( "%-2s %-20s ",
             positionName( p->primary_pos ),
             /**/          name_buf       );

     switch ( print_style )
     {
     case ps_Fielding:  printPitcherFielding( p );            break;
     case ps_Potential: printPitcherStats( &(p->potential) ); break;
     case ps_Simulated: printPitcherStats( &(p->simulated) ); break;
     case ps_Ratings:   printPitcherRatings( player );        break;
     }

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
          printf( "                         F    Fielding Stats\n"  );
          printf( "                         R    Ratings\n"         );

	  return EXIT_SUCCESS;
     }

     switch ( *argv[3] )
     {
     case 'F': print_style = ps_Fielding;  break;
     case 'P': print_style = ps_Potential; break;
     case 'R': print_style = ps_Ratings;   break;
     case 'S': print_style = ps_Simulated; break;
     default:  print_style = ps_Potential; break;
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

     printRosters( mba );

     free( mba );

     return EXIT_SUCCESS;
}
