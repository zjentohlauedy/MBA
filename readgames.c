#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main( int argc, char *argv[] )
{
     char *games = getenv( "GAMES" );
     char  home[20];
     char  road[20];

     if ( games == NULL )
     {
	  printf( "Environment variable GAMES not found.\n" );

	  return;
     }

     while ( sscanf( games, "%s %s ", road, home ) != EOF )
     {
	  printf( "<%s> at <%s>\n", road, home );

	  games += strlen(road) + strlen(home) + 2;
     }
}
