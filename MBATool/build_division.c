#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

division_s *buildDivision( const int division_id )
{
     division_s *division = NULL;

     if ( (division = malloc( sizeof(division_s) )) == NULL ) return NULL;

     memset( division, '\0', sizeof(division_s) );

     buildIntoDivision( division, division_id );

     return division;
}

void buildIntoDivision( division_s *division, const int division_id )
{
     division->division_id = division_id;

     sprintf( division->name, "Division%d", division_id );
}
