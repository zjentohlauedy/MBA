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

static void printHitterFielding( const batter_s *batter )
{
     const fielding_s *fielding = &(batter->fielding);

     const int total = fielding->put_outs + fielding->assists + fielding->errors;

     printf( "{\"put_outs\":%d", fielding->put_outs );
     printf( ",\"assists\":%d", fielding->assists );
     printf( ",\"errors\":%d", fielding->errors );
     printf( ",\"secondary_average\":\"%s\"", displayAvg( fielding->secondary_avg ) );
     printf( "}" );
}


static void printHitterStats( const hitting_s *stats )
{
     printf( "{\"games\":%d", stats->games );
     printf( ",\"at_bats\":%d", stats->at_bats );
     printf( ",\"runs\":%d", stats->runs );
     printf( ",\"hits\":%d", stats->hits );
     printf( ",\"doubles\":%d", stats->doubles );
     printf( ",\"triples\":%d", stats->triples );
     printf( ",\"home_runs\":%d", stats->home_runs );
     printf( ",\"runs_batted_in\":%d", stats->rbi );
     printf( ",\"walks\":%d", stats->walks );
     printf( ",\"strike_outs\":%d", stats->strike_outs );
     printf( ",\"steals\":%d", stats->stolen_bases );
     printf( ",\"errors\":%d", stats->errors );
     printf( "}" );
}


static void printHitterRatings( const batter_s *batter )
{
     printf( "{\"power\":%d", batter->power );
     printf( ",\"hit_n_run\":%d", batter->hit_n_run );
     printf( ",\"bunt\":%d", batter->bunt );
     printf( ",\"running\":%d", batter->running );
     printf( ",\"range\":%d", batter->range );
     printf( ",\"arm\":%d", batter->arm );
     printf( ",\"longevity\":%d", batter->longevity );

     printf( "}" );
}


static void printHitter( const player_s *player )
{
     const batter_s *b = &(player->data.batter);

     printf( "{\"id\":%d", player->player_id );
     printf( ",\"type\":\"batter\"" );
     printf( ",\"first_name\":\"%s\"", b->first_name );
     printf( ",\"last_name\":\"%s\"", b->last_name );
     printf( ",\"primary_position\":%d", b->primary_pos );
     printf( ",\"secondary_position\":%d", b->secondary_pos );
     printf( ",\"handedness\":%d", player->hand );

     printf( ",\"ratings\":" );

     printHitterRatings( b );

     printf( ",\"stats\":{" );

     printf( "\"potential\":" );

     printHitterStats( &(b->potential) );

     printf( ",\"simulated\":" );

     printHitterStats( &(b->simulated) );

     printf( ",\"fielding\":" );

     printHitterFielding( b );

     printf( "}" );
     printf( "}" );
}


static void printPitcherFielding( const pitcher_s *pitcher )
{
     printf( "{\"average\":\"%s\"}", displayAvg( pitcher->fielding_avg ) );
}


static void printPitcherStats( const pitching_s *stats )
{
     const int innings  = (int)stats->innings;
     const int outs = (int)((stats->innings - innings) * 10.0);


     printf( "{\"wins\":%d", stats->wins );
     printf( ",\"losses\":%d", stats->losses );
     printf( ",\"games\":%d", stats->games );
     printf( ",\"saves\":%d", stats->saves );
     printf( ",\"innings\":%d", innings );
     printf( ",\"outs\":%d", outs );
     printf( ",\"hits\":%d", stats->hits );
     printf( ",\"earned_runs\":%d", stats->earned_runs );
     printf( ",\"home_runs\":%d", stats->home_runs );
     printf( ",\"walks\":%d", stats->walks );
     printf( ",\"strike_outs\":%d", stats->strike_outs );


     printf( "}" );
}


static void printPitcherRatings( const pitcher_s *pitcher )
{
     printf( "{\"speed\":%d", pitcher->speed );
     printf( ",\"control\":%d", pitcher->control );
     printf( ",\"fatigue\":%d", pitcher->fatigue );
     printf( ",\"bunt\":%d", pitcher->bunt );
     printf( ",\"longevity\":%d", pitcher->longevity );

     printf( "}" );
}


static void printPitcher( const player_s *player )
{
     const pitcher_s *p = &(player->data.pitcher);

     printf( "{\"id\":%d", player->player_id );
     printf( ",\"type\":\"pitcher\"" );
     printf( ",\"first_name\":\"%s\"", p->first_name );
     printf( ",\"last_name\":\"%s\"", p->last_name );
     printf( ",\"position\":%d", p->primary_pos );
     printf( ",\"handedness\":%d", player->hand );

     printf( ",\"ratings\":" );

     printPitcherRatings( p );

     printf( ",\"stats\":{" );

     printf( "\"potential\":" );

     printPitcherStats( &(p->potential) );

     printf( ",\"simulated\":" );

     printPitcherStats( &(p->simulated) );

     printf( ",\"fielding\":" );

     printPitcherFielding( p );

     printf( "}" );

     printf( "}" );
}


static void printPlayer( const player_s *player )
{
     if      ( player->type == pt_Pitcher ) printPitcher( player );
     else if ( player->type == pt_Hitter  ) printHitter(  player );
}


static void printTeam( const team_s *team )
{
     printf( "{\"name\":\"%s\"", team->name );

     if ( team->players != NULL )
     {
          printf( ",\"players\":[" );

          for ( int i = 0; i < PLAYERS_PER_TEAM; ++i )
          {
               if ( team->players[i].type != pt_Pitcher &&
                    team->players[i].type != pt_Hitter     ) continue;

               if ( i > 0 ) printf( "," );

               printPlayer( &(team->players[i]) );
          }

          printf( "]" );
     }

     printf( "}" );
}


static void printDivision( const division_s *division )
{
     printf( "{\"name\":\"%s\"", division->name );

     if ( division->teams != NULL )
     {
          printf( ",\"teams\":[" );

          for ( int i = 0; i < TEAMS_PER_DIVISION; ++i )
          {
               if ( i > 0 ) printf( "," );

               printTeam( &(division->teams[i]) );
          }

          printf( "]" );
     }

     printf( "}" );
}


static void printLeague( const league_s *league )
{
     printf( "{\"name\":\"%s\"", league->name );

     if ( league->divisions != NULL )
     {
          printf( ",\"divisions\":[" );

          for ( int i = 0; i < DIVISIONS_PER_LEAGUE; ++i )
          {
               if ( i > 0 ) printf( "," );

               printDivision( &(league->divisions[i]) );
          }

          printf( "]" );
     }

     printf( "}" );
}


static void printData( const league_s *leagues )
{
     int  pitchingHeader = 0;
     int  hittingHeader  = 0;
     int  leag;
     int  div;
     int  team;
     int  plyr;


     printf( "{" );

     printf( "\"name\":\"Metropolitan Baseball Association\"" );
     printf( ",\"abbreviation\":\"MBA\"" );
     printf( ",\"season\":null" );

     if ( leagues != NULL )
     {
          printf( ",\"leagues\":[" );

          for ( int i = 0; i < TOTAL_LEAGUES; ++i )
          {
               if ( i > 0 ) printf( "," );

               printLeague( &(leagues[i]) );
          }

          printf( "]" );
     }

     printf( "}\n" );
}


int main( const int argc, const char *argv[] )
{
     fileplayer_s *playerFile;
     fileleague_s *leagueFile;
     league_s     *mba;


     if ( argc < 3 )
     {
	  printf( "Usage: %s <leaguefile> <playerfile>\n", argv[0] );

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

     printData( mba );

     free( mba );

     return EXIT_SUCCESS;
}
