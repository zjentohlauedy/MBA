#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

league_s *buildLeague( const int league_id )
{
     league_s *league = NULL;

     if ( (league = malloc( sizeof(league_s) )) == NULL ) return NULL;

     memset( league, '\0', sizeof(league_s) );

     buildIntoLeague( league, league_id );

     return league;
}

void buildIntoLeague( league_s *league, const int league_id )
{
     league->league_id = league_id;

     sprintf( league->name, "League%d", league_id );
}
