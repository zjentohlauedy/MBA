#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct
{
     unsigned char acc_ab      [ 2];
     unsigned char acc_hits    [ 2];
     unsigned char acc_2b      [ 1];
     unsigned char acc_3b      [ 1];
     unsigned char acc_hr      [ 1];
     unsigned char acc_bb      [ 1];
     unsigned char acc_so      [ 1];
     unsigned char acc_games   [ 1];
     unsigned char acc_runs    [ 1];
     unsigned char acc_rbi     [ 1];
     unsigned char acc_sb      [ 1];
     unsigned char acc_err     [ 1];

} acc_bat_stats_s;

typedef struct
{
     unsigned char acc_wins    [ 1];
     unsigned char acc_losses  [ 1];
     unsigned char acc_games   [ 1];
     unsigned char acc_saves   [ 1];
     unsigned char acc_innings [ 2];
     unsigned char acc_hits    [ 2];
     unsigned char acc_er      [ 2];
     unsigned char acc_hr      [ 1];
     unsigned char acc_bb      [ 1];
     unsigned char acc_so      [ 2];

} acc_pch_stats_s;

typedef struct
{
     /**/     char last_name   [10];
     /**/     char first_name  [ 8];
     /**/     char last_phoen  [14];
     /**/     char first_phoen [12];
     unsigned char real_avg    [ 2]; // era for pitchers, ba for batters
     unsigned char year        [ 1]; // added to 1845
     unsigned char position    [ 1]; // first nibble POS1; second nibble POS2 ???

     union
     {
	  struct batting_s
	  {
	       unsigned char ratings     [ 4]; // by nibble: ??,arm,runs,range,power,??,bunt,h&r
	       unsigned char real_games  [ 1];
	       unsigned char real_runs   [ 1];
	       unsigned char real_rbi    [ 1];
	       unsigned char real_sb     [ 1];
	       unsigned char real_cs     [ 1];
	       unsigned char real_sh     [ 1];

	       unsigned char vl_ab       [ 2];
	       unsigned char vl_hits     [ 2];
	       unsigned char vl_2b       [ 1];
	       unsigned char vl_3b       [ 1];
	       unsigned char vl_hr       [ 1];
	       unsigned char vl_bb       [ 1];
	       unsigned char vl_so       [ 1];

	       unsigned char real_fa2    [ 1]; // FA2?

	       unsigned char vr_ab       [ 2];
	       unsigned char vr_hits     [ 2];
	       unsigned char vr_2b       [ 1];
	       unsigned char vr_3b       [ 1];
	       unsigned char vr_hr       [ 1];
	       unsigned char vr_bb       [ 1];
	       unsigned char vr_so       [ 1];

	       unsigned char real_err    [ 1];
	       unsigned char real_po     [ 2];
	       unsigned char real_as     [ 2];

	       unsigned char unknown     [ 2];

	       acc_bat_stats_s action;
	       acc_bat_stats_s simulated;

	  } batting;

	  struct pitching_s
	  {
	       unsigned char ratings     [ 2]; // by nibble: speed,control,fatigue,bunt
	       unsigned char real_wins   [ 1];
	       unsigned char real_losses [ 1];
	       unsigned char real_games  [ 1];
	       unsigned char real_starts [ 1];
	       unsigned char real_cg     [ 1];
	       unsigned char real_saves  [ 1];
	       unsigned char real_inn    [ 2];

	       unsigned char vl_ab       [ 2];
	       unsigned char vl_hits     [ 2];
	       unsigned char vl_hr       [ 1];
	       unsigned char vl_bb       [ 1];
	       unsigned char vl_so       [ 2];

	       unsigned char real_er     [ 1];
	       unsigned char real_fa     [ 1]; // FA?

	       unsigned char vr_ab       [ 2];
	       unsigned char vr_hits     [ 2];
	       unsigned char vr_hr       [ 1];
	       unsigned char vr_bb       [ 1];
	       unsigned char vr_so       [ 2];

	       unsigned char real_hb     [ 1];
	       unsigned char real_balk   [ 1];
	       unsigned char real_ab     [ 1]; // batting
	       unsigned char real_hits   [ 1]; // batting
	       unsigned char real_hr     [ 1]; // batting
	       unsigned char real_sh     [ 1]; // batting
	       unsigned char injury_days [ 1];

	       unsigned char unknown     [ 1];

	       acc_pch_stats_s action;
	       acc_pch_stats_s simulated;

	  } pitching;

     } stats;

} player_s;


int main( int argc, char *argv[] )
{
     player_s   *player;
     char       *filename_r;
     char       *filename_w;
     char        input_buf[sizeof(player_s)];
     int         fd_r;
     int         fd_w;
     int         bytes_read;
     int         bytes_written;

     if ( argc < 3 )
     {
	  printf( "Usage: %s <input file> <output file>\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     filename_r = argv[1];
     filename_w = argv[2];

     if ( (fd_r = open( filename_r, O_RDONLY )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename_r, strerror(errno) );

	  return EXIT_FAILURE;
     }

     if ( (fd_w = creat( filename_w, S_IRUSR | S_IWUSR )) < 0 )
     {
	  printf( "Cannot open file <%s>: %s\n", filename_w, strerror(errno) );

	  if ( close( fd_r ) < 0 )
	  {
	       printf( "Error closing input file: %s\n", strerror(errno) );
	  }

	  return EXIT_FAILURE;
     }

     while ( 1 )
     {
	  if ( (bytes_read = read( fd_r, input_buf, sizeof(player_s) )) < sizeof(player_s) )
	  {
	       if ( bytes_read == 0 ) break;

	       printf( "Warning: data may be truncated!\n" );
	  }

	  player = (player_s *)input_buf;

	  if ( player->last_name[0] != 0 )
	  {

	       unsigned int pos = player->position[0] / 0x10;

	       if   ( pos != 0 )
	       {
		    struct batting_s *batting = &(player->stats.batting);

		    batting->action = batting->simulated;

		    memset( &(batting->simulated), 0, sizeof(acc_bat_stats_s) );
	       }
	       else
	       {
		    unsigned char games = player->stats.pitching.simulated.acc_games[0];

		    player->stats.pitching.simulated.acc_games[0] = 0;

		    player->stats.batting.action.acc_games[0] = games;
	       }
	  }

	  if ( (bytes_written = write( fd_w, input_buf, sizeof(player_s) )) < bytes_read )
	  {
	       if ( bytes_written < 0 )
	       {
		    printf( "Error writing to output file: %s\n", strerror( errno ) );

		    if ( close( fd_r ) < 0 )
		    {
			 printf( "Error closing input file: %s\n", strerror(errno) );
		    }

		    if ( close( fd_w ) < 0 )
		    {
			 printf( "Error closing output file: %s\n", strerror(errno) );
		    }

		    return EXIT_FAILURE;
	       }

	       printf( "Warning: incomplete buffer written to output file!\n" );
	  }
     }

     if ( close( fd_r ) < 0 )
     {
	  printf( "Error closing input file: %s\n", strerror(errno) );

	  return EXIT_FAILURE;
     }

     if ( close( fd_w ) < 0 )
     {
	  printf( "Error closing output file: %s\n", strerror(errno) );

	  return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
