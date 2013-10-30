#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_list.h"
#include "file_formats.h"
#include "org.h"


static org_s *createOrg( void )
{
     org_s *org = NULL;

     if ( (org = malloc( sizeof(org_s) )) == NULL ) return NULL;

     memset( org, '\0', sizeof(org_s) );

     return org;
}

org_s *convertOrg( const org_data_s *org_data )
{
     org_s *org = NULL;

     if ( (org = createOrg()) == NULL ) return NULL;

     if ( (org->leagues = convertLeagues( org_data )) == NULL )
     {
          free( org );

          return NULL;
     }

     return org;
}

void freeOrg( org_s *org )
{
     if ( org->leagues != NULL )
     {
          freeOrgLeagues( org->leagues );
     }

     free( org );
}
