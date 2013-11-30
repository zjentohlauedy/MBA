#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

league_division_s *buildLeagueDivision( const int league_id, const int division_id )
{
     league_division_s *league_division = NULL;

     if ( (league_division = malloc( sizeof(league_division_s) )) == NULL ) return NULL;

     memset( league_division, '\0', sizeof(league_division_s) );

     buildIntoLeagueDivision( league_division, league_id, division_id );

     return league_division;
}

void buildIntoLeagueDivision( league_division_s *league_division, const int league_id, const int division_id )
{
     league_division->league_id   = league_id;
     league_division->division_id = division_id;
}
