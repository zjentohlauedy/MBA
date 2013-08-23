#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "ewbfiles.h"


typedef struct
{
     char  name[12 + 1];
     int   index;
     int   pitchers_needed;
     int   batters_needed;
     /**/
} draftteam_s;


typedef struct
{
     fileplayer_s *players;
     int           total;
     int           index;
     /**/
} playerlist_s;


typedef struct
{
     draftteam_s  *draftTeams;
     fileplayer_s *teamPlayers;
     playerlist_s  freePitchers;
     playerlist_s  freeHitters;
     /**/
} draft_s;


static void cleanup( const draft_s *draft )
{
     free( draft->draftTeams           );
     free( draft->teamPlayers          );
     free( draft->freePitchers.players );
     free( draft->freeHitters.players  );
}


static void shuffleTeams( draftteam_s *teams )
{
     draftteam_s team;

     for( int i = TOTAL_TEAMS; i > 1; --i )
     {
          int n = rand() % (i - 1);

          team         = teams[n    ];
          teams[n    ] = teams[i - 1];
          teams[i - 1] = team;
     }
}


static int findOpenPlayerSlot( const fileplayer_s *players, const int teamIdx )
{
     int start = (teamIdx * PLAYERS_PER_TEAM);

     for ( int i = start; i < start + PLAYERS_PER_TEAM; ++i )
     {
          if ( players[i].last_name[0] == '\0' ) return i;
     }

     return -1;
}


static boolean_e draftPlayer( fileplayer_s *slot, playerlist_s *list )
{
     if ( list->index == list->total )
     {
          printf( "No more players in list.\n" );

          return bl_False;
     }

     if ( list->players[list->index].last_name[0] == '\0' )
     {
          printf( "No more players in list.\n" );

          return bl_False;
     }

     *slot = list->players[list->index++];

     return bl_True;
}


static boolean_e draftRound( draft_s *draft )
{
     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          draftteam_s  *draftTeam = &(draft->draftTeams[i]);
          playerlist_s *players;
          int           freeSlot;

          if ( (freeSlot = findOpenPlayerSlot( draft->teamPlayers, draftTeam->index )) < 0 )
          {
               printf( "Team does not have an available free slot.\n" );

               return bl_False;
          }

          if      ( draftTeam->pitchers_needed == 0 ) players = &(draft->freeHitters);
          else if ( draftTeam->batters_needed  == 0 ) players = &(draft->freePitchers);
          else
          {
               if   ( (rand() % 2) == 0 ) players = &(draft->freePitchers);
               else                       players = &(draft->freeHitters);
          }

          if ( draftPlayer( &(draft->teamPlayers[freeSlot]), players ) == bl_False )
          {
               printf( "Cannot draft a player.\n" );

               return bl_False;
          }

          if   ( players == &(draft->freePitchers) ) draftTeam->pitchers_needed--;
          else                                       draftTeam->batters_needed--;

          printf( "%s has drafted %s %.*s, %.*s\n",
                  draftTeam->name,
                  positionName( nibble( draft->teamPlayers[freeSlot].position[0], n_High ) ),
                  sizeof(draft->teamPlayers[freeSlot].last_name), draft->teamPlayers[freeSlot].last_name,
                  sizeof(draft->teamPlayers[freeSlot].first_name), draft->teamPlayers[freeSlot].first_name );
     }

     return bl_True;
}


static void draftPlayers( draft_s *draft )
{
     draft->freePitchers.index = 0;
     draft->freeHitters.index  = 0;

     for ( int i = 0; i < 4; ++i )
     {
          shuffleTeams( draft->draftTeams );

          if ( draftRound( draft ) == bl_False )
          {
               printf( "Draft failed in round %d.\n", i + 1 );

               return;
          }
     }
}


static int calcVsba( const fileplayer_s *player )
{
     const struct pitching_s *p = &(player->filestats.filepitching);

     const int ab = word2int( p->vl_ab   ) + word2int( p->vr_ab   );
     const int h  = word2int( p->vl_hits ) + word2int( p->vr_hits );

     return (int)ceil( (double)h / (double)ab * 1000.0 );
}


static int calcEra( const fileplayer_s *player )
{
     const struct pitching_s *p = &(player->filestats.filepitching);

     const float inn = (float)word2int( p->real_inn ) / 10.0;
     const int   er  = p->real_er[0];

     return (int)ceil( (double)er / (double)inn * 9.0 );
}


static int comparePitchers( const void *arg1, const void *arg2 )
{
     fileplayer_s *p1 = (fileplayer_s *)arg1;
     fileplayer_s *p2 = (fileplayer_s *)arg2;

     int n1, n2;

     if ( p1->last_name[0] == '\0' ) return (p2->last_name[0] == '\0') ? 0 : 1;

     if ( p2->last_name[0] == '\0' ) return -1;

     if ( (n1 = calcVsba( p1 )) != (n2 = calcVsba( p2 )) ) return n1 - n2;

     if ( (n1 = calcEra( p1 )) != (n2 = calcEra( p2 )) ) return n1 - n2;

     return 0;
}


static int calcBa( const fileplayer_s *player )
{
     const struct batting_s *b = &(player->filestats.filebatting);

     const int ab = word2int( b->vl_ab ) + word2int( b->vr_ab );
     const int h  = word2int( b->vl_hits ) + word2int( b->vr_hits );

     return (int)ceil( (double)h / (double)ab * 1000.0 );
}


static int getPower( const fileplayer_s *player )
{
     const struct batting_s *b = &(player->filestats.filebatting);

     return nibble( b->ratings[2], n_High );
}

static int compareHitters( const void *arg1, const void *arg2 )
{
     fileplayer_s *p1 = (fileplayer_s *)arg1;
     fileplayer_s *p2 = (fileplayer_s *)arg2;

     int n1, n2;

     if ( p1->last_name[0] == '\0' ) return (p2->last_name[0] == '\0') ? 0 : 1;

     if ( p2->last_name[0] == '\0' ) return -1;


     // Note: want to sort highest -> lowest so return calc is reversed
     if ( (n1 = calcBa( p1 )) != (n2 = calcBa( p2 )) ) return n2 - n1;

     if ( (n1 = getPower( p1 )) != (n2 = getPower( p2 )) ) return n2 - n1;

     return 0;
}


static void sortPlayerList( const playerlist_s *list, int(*compar)(const void *, const void *) )
{
     qsort( list->players, list->total, sizeof(fileplayer_s), compar );
}


static boolean_e addPlayerToList( playerlist_s *list, const fileplayer_s *player )
{
     if ( list->total == 0 )
     {
          list->total = 50;

          if ( (list->players = (fileplayer_s *)malloc( sizeof(fileplayer_s) * list->total )) == NULL )
          {
               printf( "Cannot allocate memory for player list.\n" );

               return bl_False;
          }
     }

     if ( list->index == list->total )
     {
          fileplayer_s *players;

          list->total *= 2;

          if ( (players = (fileplayer_s *)realloc( list->players, sizeof(fileplayer_s) * list->total )) == NULL )
          {
               printf( "Cannot allocate memory for player list.\n" );

               free( list->players );

               list->players = NULL;

               return bl_False;
          }

          list->players = players;
     }

     list->players[list->index++] = *player;
}


static boolean_e separateFreeAgentsByPosition( const fileplayer_s *agentsFile, draft_s *draft )
{
     playerlist_s *players;

     for ( int i = 0, pi = 0, hi = 0; i < TOTAL_PLAYERS; ++i )
     {
          if ( agentsFile[i].last_name[0] == '\0' ) continue;

          if   ( nibble( agentsFile[i].position[0], n_High ) == pos_Pitcher ) players = &(draft->freePitchers);
          else                                                                players = &(draft->freeHitters);

          if ( addPlayerToList( players, &(agentsFile[i]) ) == bl_False )
          {
               printf( "Cannot add player to list.\n" );

               return bl_False;
          }
     }

     printf( "%d pitchers found.\n", draft->freePitchers.index );
     printf( "%d hitters found.\n", draft->freeHitters.index );

     sortPlayerList( &(draft->freePitchers), comparePitchers );
     sortPlayerList( &(draft->freeHitters),  compareHitters  );

     return bl_True;
}


static boolean_e loadTeamsArray( const fileleague_s *leagueFile, draft_s *draft )
{
     const fileteam_s  *fileTeam;
     draftteam_s *draftTeams;

     if ( (draftTeams = malloc( sizeof(draftteam_s) * TOTAL_TEAMS)) == NULL )
     {
          printf( "Cannot allocate memory for draft teams array.\n" );

          return bl_False;
     }

     fileTeam = leagueFile->teams;

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          sprintf( draftTeams[i].name, "%.*s", sizeof(fileTeam[i].name), fileTeam[i].name );

          draftTeams[i].index           = i;
          draftTeams[i].pitchers_needed = 2;
          draftTeams[i].batters_needed  = 2;
     }

     draft->draftTeams = draftTeams;

     return bl_True;
}


static boolean_e initDraft( draft_s *draft, const char *argv[] )
{
     fileplayer_s *agentsFile;
     fileplayer_s *playerFile;
     fileleague_s *leagueFile;

     if ( (leagueFile = readLeagueFile( argv[1] )) == NULL )
     {
          printf( "Cannot load league file.\n" );

          return bl_False;
     }

     if ( loadTeamsArray( leagueFile, draft ) == bl_False )
     {
          printf( "Cannot load teams array.\n" );

          return bl_False;
     }

     free( leagueFile );

     if ( (playerFile = readPlayersFile( argv[2] )) == NULL )
     {
          printf( "Cannot load team players file.\n" );

          return bl_False;
     }

     if ( (agentsFile = readPlayersFile( argv[3] )) == NULL )
     {
          printf( "Cannot load free agents players file.\n" );

          free( playerFile );

          return bl_False;
     }

     if ( separateFreeAgentsByPosition( agentsFile, draft ) == bl_False )
     {
          printf( "Cannot separate free agents by position.\n" );

          free( playerFile );
          free( agentsFile );

          return bl_False;
     }

     free( agentsFile );

     draft->teamPlayers = playerFile;

     return bl_True;
}


int main( const int argc, const char *argv[] )
{
     draft_s draft = { 0 };

     if ( argc < 5 )
     {
	  printf( "Usage: %s <leaguefile> <teamplayerfile> <freeagentsfile> <outputfile>.\n", argv[0] );

	  return EXIT_SUCCESS;
     }

     srand( time( NULL ) );

     if ( initDraft( &draft, argv ) == bl_False )
     {
          printf( "Cannot initialize draft.\n" );

          return EXIT_FAILURE;
     }

     draftPlayers( &draft );

     if ( ! writePlayersFile( argv[4], draft.teamPlayers ) )
     {
          printf( "Cannot save player changes.\n" );

          cleanup( &draft );

          return EXIT_FAILURE;
     }

     cleanup( &draft );

     return EXIT_SUCCESS;
}
