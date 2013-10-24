#include <stdio.h>
#include "builders.h"
#include "file_formats.h"

fileparks_s *buildFileParks( void )
{
     static fileparks_s parks_data = { 0 };

     for ( int i = 0; i < TOTAL_TEAMS; ++i )
     {
          sprintf( parks_data.park_names[i].text, "Park%d", i + 1 );
     }

     return &parks_data;
}
