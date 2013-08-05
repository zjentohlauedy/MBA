#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#define SWAP( A, B )  do { int x = A; A = B; B = x; } while ( 0 )


typedef struct
{
     int road;
     int home;

} game_s;


static game_s games[152][16]    = { 0 };
static int    teams[32]         = { 0 };
static int    day;
static char   teamNames[32][10] = { 0 };


static void initTeamNames( void )
{
     strcpy( teamNames[ 0], "Sabres"    );
     strcpy( teamNames[ 1], "Portsmen"  );
     strcpy( teamNames[ 2], "Settlers"  );
     strcpy( teamNames[ 3], "Kings"     );
     strcpy( teamNames[ 4], "Voyagers"  );
     strcpy( teamNames[ 5], "Rockets"   );
     strcpy( teamNames[ 6], "Knights"   );
     strcpy( teamNames[ 7], "Drizzle"   );
     strcpy( teamNames[ 8], "Scorpions" );
     strcpy( teamNames[ 9], "Lightning" );
     strcpy( teamNames[10], "Goblins"   );
     strcpy( teamNames[11], "Photons"   );
     strcpy( teamNames[12], "Dragons"   );
     strcpy( teamNames[13], "Hammers"   );
     strcpy( teamNames[14], "Expos"     );
     strcpy( teamNames[15], "Dynamo"    );
     strcpy( teamNames[16], "Aces"      );
     strcpy( teamNames[17], "Cyclone"   );
     strcpy( teamNames[18], "Stormers"  );
     strcpy( teamNames[19], "Express"   );
     strcpy( teamNames[20], "Warriors"  );
     strcpy( teamNames[21], "Wanderers" );
     strcpy( teamNames[22], "Thunder"   );
     strcpy( teamNames[23], "Glory"     );
     strcpy( teamNames[24], "Eclipse"   );
     strcpy( teamNames[25], "Legends"   );
     strcpy( teamNames[26], "Waves"     );
     strcpy( teamNames[27], "Horizon"   );
     strcpy( teamNames[28], "Sharks"    );
     strcpy( teamNames[29], "Flames"    );
     strcpy( teamNames[30], "Techs"     );
     strcpy( teamNames[31], "Quasars"   );
}


static void shuffle( int list[], int length )
{
     int i;

     // Randomize the schedule
     for( i = length; i > 1; --i )
     {
          int n = rand() % (i - 1);
          int x;

          x           = list[n    ];
          list[n    ] = list[i - 1];
          list[i - 1] = x;
     }

}


static void rotate( int list[], int length )
{
     int i;
     int x = list[length - 1];

     for ( i = length - 1; i > 0; --i )
     {
          list[i] = list[i - 1];
     }

     list[0] = x;
}


static void scheduleOverallGames( game_s *gameday )
{
     int i;

     for ( i = 0; i < 16; ++i )
     {
	  int match = i;
	  int road  = i;
	  int home  = 31 - i;

	  // Find the last time the teams played and make sure location is reversed
	  {
	       int x, y;

	       for ( x = day - 1; x >= 0; --x )
	       {
		    game_s *g = games[x];

		    for ( y = 0; y < 16; ++y )
		    {
			 if ( g[y].road == teams[home]  &&  g[y].home == teams[road] ) goto done;

			 if ( g[y].road == teams[road]  &&  g[y].home == teams[home] )
			 {
			      SWAP( road, home );

			      goto done;
			 }
		    }
	       }

          done:
               gameday[match].road = teams[road];
               gameday[match].home = teams[home];
          }
     }
}


static void scheduleDivisionGames( game_s *gameday )
{
     int i;
     int j;

     for ( i = 0; i < 32; i += 8 )
     {
          for ( j = 0; j < 4; ++j )
          {
               int match = (i / 2) + j;
               int road  = i + j;
               int home  = i + 7 - j;

	       // Find the last time the teams played and make sure location is reversed
               {
                    int x, y;

                    for ( x = day - 1; x >= 0; --x )
                    {
                         game_s *g = games[x];

                         for ( y = 0; y < 16; ++y )
                         {
                              if ( g[y].road == teams[home]  &&  g[y].home == teams[road] ) goto done;

                              if ( g[y].road == teams[road]  &&  g[y].home == teams[home] )
                              {
                                   SWAP( road, home );

                                   goto done;
                              }
                         }
                    }
               }

          done:
               gameday[match].road = teams[road];
               gameday[match].home = teams[home];
          }
     }
}



void main( int argc, char *argv[] )
{
     time_t  t = time( NULL );

     // seed random number generator...
     srand( t );

     for ( int i = 0; i < 32; ++i ) teams[i] = i;

     day = 0;

     // Divisions
     for ( int matches = 0; matches < 2; ++matches )
     {
	  shuffle( &teams[ 0], 8 );
	  shuffle( &teams[ 8], 8 );
	  shuffle( &teams[16], 8 );
	  shuffle( &teams[24], 8 );

	  for ( int series = 0; series < 8 - 1; ++series )
	  {
	       for ( int round = 0; round < 2; ++round )
	       {
		    scheduleDivisionGames( games[day] );

		    day++;
	       }

	       // Rotate each division individually (first team in each div. stays put)
	       rotate( &teams[ 0], 8 - 1 );
	       rotate( &teams[ 8], 8 - 1 );
	       rotate( &teams[16], 8 - 1 );
	       rotate( &teams[24], 8 - 1 );
	  }
     }

     // Overall
     for ( int matches = 0; matches < 2; ++matches )
     {
	  shuffle( teams, 32 );

	  for ( int series = 0; series < 32 - 1; ++series )
	  {
	       for ( int round = 0; round < 2; ++round )
	       {
		    scheduleOverallGames( games[day] );

		    day++;
	       }

	       // Rotate entire league
	       rotate( teams, 32 - 1 );
	  }
     }

     // Randomize the schedule
     for( int i = 152; i > 1; --i )
     {
	  int n = rand() % (i - 1);

	  // swap n and i - 1;
	  game_s  gameday[16];

	  memcpy( gameday,      games[n    ], sizeof(game_s) * 16 );
	  memcpy( games[n    ], games[i - 1], sizeof(game_s) * 16 );
	  memcpy( games[i - 1], gameday,      sizeof(game_s) * 16 );
     }

     // Randomize the games within each gameday
     for ( int i = 0; i < 152; ++i )
     {
	  for ( int j = 16; j > 1; --j )
	  {
	       int n = rand() % (j - 1);

	       // swap n and i - 1;
	       game_s  game;

	       game            = games[i][j - 1];
	       games[i][j - 1] = games[i][n];
	       games[i][n]     = game;
	  }
     }

     initTeamNames();

     for ( int i = 0; i < 16; ++i )
     {
	  if   ( i > 0 ) printf( ",," );
	  else           printf( ","  );

	  printf( "%02d", i + 1 );
     }

     printf( "\n" );

     for ( int i = 0; i < 152; ++i )
     {
	  for ( int j = 0; j < 16; ++j )
	  {
	       if   ( j > 0 ) printf( ",," );
	       else           printf( ","  );

	       printf( "%s", teamNames[games[i][j].road] );
	  }

	  printf( "\n" );
	  printf( "%03d", i + 1 );

	  for ( int j = 0; j < 16; ++j )
	  {
	       if   ( j > 0 ) printf( ",," );
	       else           printf( ","  );

	       printf( "%s", teamNames[games[i][j].home] );
	  }

	  printf( "\n" );

	  printf( ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n" );
     }
}
