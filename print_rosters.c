#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ewbfiles.h"


typedef enum
{
     pt_Pitcher,
     pt_Hitter,
     pt_None

} playertype_e;

typedef enum
{
     ps_Fielding,
     ps_Potential,
     ps_Simulated

} printstyle_e;

typedef struct
{
     int   wins;
     int   losses;
     int   games;
     int   saves;
     float innings;
     int   at_bats;
     int   hits;
     int   earned_runs;
     int   home_runs;
     int   walks;
     int   strike_outs;

} pitching_s;

typedef struct
{
     char        first_name[ 20 + 1 ];
     char        last_name [ 20 + 1 ];
     int         year;
     position_e  primary_pos;
     position_e  secondary_pos;
     int         speed;
     int         control;
     int         fatigue;
     int         bunt;
     pitching_s  potential;
     pitching_s  simulated;
     float       fielding_avg;

} pitcher_s;

typedef struct
{
     int games;
     int at_bats;
     int runs;
     int hits;
     int doubles;
     int triples;
     int home_runs;
     int rbi;
     int walks;
     int strike_outs;
     int stolen_bases;
     int errors;

} hitting_s;

typedef struct
{
     int   put_outs;
     int   assists;
     int   errors;
     float secondary_avg;

} fielding_s;

typedef struct
{
     char        first_name[ 20 + 1 ];
     char        last_name [ 20 + 1 ];
     int         year;
     position_e  primary_pos;
     position_e  secondary_pos;
     int         power;
     int         hit_n_run;
     int         bunt;
     int         running;
     int         range;
     int         arm;
     hitting_s   potential;
     hitting_s   simulated;
     fielding_s  fielding;

} batter_s;

typedef struct
{
     playertype_e type;

     union
     {
	  pitcher_s    pitcher;
	  batter_s     batter;

     }            data;

} player_s;

typedef struct
{
     char     name[20 + 1];
     int      wins;
     int      losses;
     color_e  color;
     player_s players[25];

} team_s;

typedef struct
{
     char   name[20 + 1];
     team_s teams[8];

} division_s;

typedef struct
{
     char       name[20 + 1];
     division_s divisions[2];

} league_s;


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
     const int total = fielding->put_outs + fielding->assists + fielding->errors;

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

     printf( "%-2s %-20s ",
             positionName( b->primary_pos ),
             /**/          name_buf       );

     printf( "%2d %2d %2d %2d %2d %2d ",
             b->power, b->hit_n_run, b->bunt, b->running, b->range, b->arm );

     if ( print_style == ps_Fielding )
     {
          printf( "%3d ", b->potential.games );
     }

     switch ( print_style )
     {
     case ps_Fielding:  printHitterFielding( &(b->fielding), b->secondary_pos ); break;
     case ps_Potential: printHitterStats( &(b->potential) );   break;
     case ps_Simulated: printHitterStats( &(b->simulated) );   break;
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

     printf( "%2d %2d %2d %2d  ", p->speed, p->control, p->fatigue, p->bunt );

     switch ( print_style )
     {
     case ps_Fielding:  printPitcherFielding( p );            break;
     case ps_Potential: printPitcherStats( &(p->potential) ); break;
     case ps_Simulated: printPitcherStats( &(p->simulated) ); break;
     }

     printf( "\n" );
}


static void printPlayer( const player_s *player )
{
     if      ( player->type == pt_Pitcher ) printPitcher( player );
     else if ( player->type == pt_Hitter  ) printHitter(  player );
}


static int cmpplr( const void *arg1, const void *arg2 )
{
     const player_s *p1 = (player_s *)arg1;
     const player_s *p2 = (player_s *)arg2;
     /**/  int       cmp;

     if ( p1->type != p2->type ) return p1->type - p2->type;

     if ( p1->type == pt_None ) return 0;

     if ( p1->type == pt_Hitter )
     {
	  const batter_s *b1 = &(p1->data.batter);
	  const batter_s *b2 = &(p2->data.batter);

	  if ( b1->primary_pos != b2->primary_pos ) return b1->primary_pos - b2->primary_pos;

	  if ( (cmp = strcmp( b1->last_name, b2->last_name )) != 0 ) return cmp;

	  return strcmp( b1->first_name, b2->first_name );
     }

     if ( p1->type == pt_Pitcher )
     {
	  const pitcher_s *pt1 = &(p1->data.pitcher);
	  const pitcher_s *pt2 = &(p2->data.pitcher);

	  if ( (cmp = strcmp( pt1->last_name, pt2->last_name )) != 0 ) return cmp;

	  return strcmp( pt1->first_name, pt2->first_name );
     }

     return 0;
}


static void copyPlayer( player_s *player, const fileplayer_s *fileplayer )
{
     // Don't copy empty players
     if ( fileplayer->last_name[0] == 0 )
     {
	  player->type = pt_None;

	  return;
     }

     char first_name [sizeof(fileplayer->first_name)];
     char last_name  [sizeof(fileplayer->last_name) ];

     memcpy( first_name, fileplayer->first_name, sizeof(fileplayer->first_name) );
     memcpy( last_name,  fileplayer->last_name,  sizeof(fileplayer->last_name)  );

     if ( (fileplayer->position[0]>>4) == 0 )
     {
	  // Pitcher
	  player->type = pt_Pitcher;

	  pitcher_s *pitcher = &(player->data.pitcher);

	  // The last byte of each name has the high bit set, it needs to be cleared
          untermName( first_name, sizeof(fileplayer->first_name) );
          untermName( last_name,  sizeof(fileplayer->last_name)  );

	  snprintf( pitcher->first_name, sizeof(pitcher->first_name), "%.*s", sizeof(fileplayer->first_name), first_name );
	  snprintf( pitcher->last_name,  sizeof(pitcher->last_name),  "%.*s", sizeof(fileplayer->last_name),  last_name );

	  pitcher->year = fileplayer->year[0] + 1845;
	  pitcher->primary_pos   = pos_Pitcher;
	  pitcher->secondary_pos = pos_Pitcher;

	  const struct pitching_s *p = &(fileplayer->filestats.filepitching);

	  pitcher->speed        = (p->ratings[0]>>4);
	  pitcher->control      = (p->ratings[0] & 0x0F);
	  pitcher->fatigue      = (p->ratings[1]>>4);
	  pitcher->bunt         = (p->ratings[1] & 0x0F);
          pitcher->fielding_avg = 1.0 - (((float)p->real_fa[0]) / 1000.0);

	  pitching_s *stats = &(pitcher->potential);

	  stats->wins        = p->real_wins[0];
	  stats->losses      = p->real_losses[0];
	  stats->games       = p->real_games[0];
	  stats->saves       = p->real_saves[0];
	  stats->innings     = (float)word2int( p->real_inn ) / 10.0;
          stats->at_bats     = word2int( p->vl_ab   ) + word2int( p->vr_ab   );
	  stats->hits        = word2int( p->vl_hits ) + word2int( p->vr_hits );
	  stats->earned_runs = p->real_er[0];
	  stats->home_runs   = p->vl_hr[0] + p->vr_hr[0];
	  stats->walks       = p->vl_bb[0] + p->vr_bb[0];
	  stats->strike_outs = word2int( p->vl_so ) + word2int( p->vr_so );

	  const acc_pch_stats_s *simulated = &(p->simulated);

	  stats = &(pitcher->simulated);

	  stats->wins        = simulated->acc_wins[0];
	  stats->losses      = simulated->acc_losses[0];
	  stats->games       = simulated->acc_starts[0];
	  stats->saves       = simulated->acc_saves[0];
	  stats->innings     = (float)word2int( simulated->acc_innings ) / 10.0;
	  stats->hits        = word2int( simulated->acc_hits );
	  stats->earned_runs = word2int( simulated->acc_er );
	  stats->home_runs   = simulated->acc_hr[0];
	  stats->walks       = simulated->acc_bb[0];
	  stats->strike_outs = word2int( simulated->acc_so );

          float norm_inn  = ((int)stats->innings);
          /**/  norm_inn += (stats->innings - norm_inn) / 3.0;
          int   est_ab    = (int)(norm_inn * 3) + stats->hits + stats->walks;

          stats->at_bats = est_ab;
     }
     else
     {
	  // Hitter
	  player->type = pt_Hitter;

	  batter_s *batter = &(player->data.batter);

	  // The last byte of each name has the high bit set, it needs to be cleared
          untermName( first_name, sizeof(fileplayer->first_name) );
          untermName( last_name,  sizeof(fileplayer->last_name)  );

	  snprintf( batter->first_name, sizeof(batter->first_name), "%.*s", sizeof(fileplayer->first_name), first_name );
	  snprintf( batter->last_name,  sizeof(batter->last_name),  "%.*s", sizeof(fileplayer->last_name),  last_name );

	  batter->year = fileplayer->year[0] + 1845;
	  batter->primary_pos   = (fileplayer->position[0]>>4);
	  batter->secondary_pos = (fileplayer->position[0] & 0x0F);

	  const struct batting_s *b = &(fileplayer->filestats.filebatting);

	  batter->power     = (b->ratings[2]>>4);
	  batter->hit_n_run = (b->ratings[3] & 0x0F);
	  batter->bunt      = (b->ratings[3]>>4);
	  batter->running   = (b->ratings[1]>>4);
	  batter->range     = (b->ratings[1] & 0x0F);
	  batter->arm       = (b->ratings[0] & 0x0F);

          fielding_s *fielding = &(batter->fielding);

          fielding->put_outs      = word2int( b->real_po );
          fielding->assists       = word2int( b->real_as );
          fielding->errors        = b->real_err[0];
          fielding->secondary_avg = 1.0 - (((float)b->real_fa2[0]) / 1000.0);

	  hitting_s *stats = &(batter->potential);

	  stats->games        = (b->real_games[0]);
	  stats->at_bats      = word2int( b->vl_ab ) + word2int( b->vr_ab );
	  stats->runs         = (b->real_runs[0]);
	  stats->hits         = word2int( b->vl_hits ) + word2int( b->vr_hits );
	  stats->doubles      = (b->vl_2b[0] + b->vr_2b[0]);
	  stats->triples      = (b->vl_3b[0] + b->vr_3b[0]);
	  stats->home_runs    = (b->vl_hr[0] + b->vr_hr[0]);
	  stats->rbi          = (b->real_rbi[0]);
	  stats->walks        = (b->vl_bb[0] + b->vr_bb[0]);
	  stats->strike_outs  = (b->vl_so[0] + b->vr_so[0]);
	  stats->stolen_bases = (b->real_sb[0]);
	  stats->errors       = (b->real_err[0]);

	  const acc_bat_stats_s *simulated = &(b->simulated);
	  const acc_bat_stats_s *action    = &(b->action);

	  stats = &(batter->simulated);

	  stats->games        = (simulated->acc_games[0]);
	  stats->at_bats      = word2int( simulated->acc_ab );
	  stats->runs         = (simulated->acc_runs[0]);
	  stats->hits         = word2int( simulated->acc_hits );
	  stats->doubles      = (simulated->acc_2b[0]);
	  stats->triples      = (simulated->acc_3b[0]);
	  stats->home_runs    = (simulated->acc_hr[0]);
	  stats->rbi          = (simulated->acc_rbi[0] + action->acc_rbi[0]);
	  stats->walks        = (simulated->acc_bb[0]);
	  stats->strike_outs  = (simulated->acc_so[0] + action->acc_so[0]);
	  stats->stolen_bases = (simulated->acc_sb[0]);
	  stats->errors       = (simulated->acc_err[0]);
     }
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


static league_s *convertFilesToLeagues( fileleague_s *leagueFile, const fileplayer_s *playerFile )
{
     league_s *leagues;
     int       leag;
     int       div;
     int       team;
     int       plyr;
     int       idx;


     if ( (leagues = malloc( sizeof(league_s) * TOTAL_LEAGUES )) == NULL )
     {
	  printf( "Cannot allocate memory for leagues.\n" );

	  return NULL;
     }

     strcpy( leagues[0].name, leagueFile->league1 );
     strcpy( leagues[1].name, leagueFile->league2 );

     strcpy( leagues[0].divisions[0].name, leagueFile->division1 );
     strcpy( leagues[0].divisions[1].name, leagueFile->division2 );
     strcpy( leagues[1].divisions[0].name, leagueFile->division3 );
     strcpy( leagues[1].divisions[1].name, leagueFile->division4 );


     // leagues[0] - div[0] - team[0] - players   0 -  24
     // ...
     // leagues[1] - div[1] - team[7] - players 775 - 799

     for ( leag = 0; leag < TOTAL_LEAGUES; ++leag )
     {
	  for ( div = 0; div < DIVISIONS_PER_LEAGUE; ++div )
	  {
	       for ( team = 0; team < TEAMS_PER_DIVISION; ++team )
	       {
		    idx = (leag * TEAMS_PER_LEAGUE) + (div * TEAMS_PER_DIVISION) + team;

		    snprintf( leagues[leag].divisions[div].teams[team].name,
			      sizeof(leagues[leag].divisions[div].teams[team].name),
			      "%.*s",
			      sizeof(leagueFile->teams[idx].name),
			      leagueFile->teams[idx].name );

		    leagues[leag].divisions[div].teams[team].wins   = leagueFile->teams[idx].wins[0];
		    leagues[leag].divisions[div].teams[team].losses = leagueFile->teams[idx].losses[0];
		    leagues[leag].divisions[div].teams[team].color  = leagueFile->teams[idx].color[0];

		    for ( plyr = 0; plyr < PLAYERS_PER_TEAM; ++plyr )
		    {
			 // copy into leagues[l].divisions[d].teams[t].players[p]
			 //      from playerFile[ (l * 400) + (d * 200) + (t * 25) + p ]
			 idx = (leag * PLAYERS_PER_LEAGUE) + (div * PLAYERS_PER_DIVISION) + (team * PLAYERS_PER_TEAM) + plyr;

			 copyPlayer( &leagues[leag].divisions[div].teams[team].players[plyr], &playerFile[idx] );
		    }

		    // For each team sort players by:
		    // 1. non-empty before empty entries
		    // 2. position
		    // 3. alphabetical

		    qsort( leagues[leag].divisions[div].teams[team].players, PLAYERS_PER_TEAM, sizeof(player_s), cmpplr );
	       }
	  }
     }

     return leagues;
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
          printf( "                         F    Fielding Stats\n" );

	  return EXIT_SUCCESS;
     }

     switch ( *argv[3] )
     {
     case 'F': print_style = ps_Fielding;  break;
     case 'P': print_style = ps_Potential; break;
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
