#ifndef __INC_ORG_H__
#define __INC_ORG_H__

#include "league.h"

typedef struct
{
     league_s *league;

} org_league_s;

typedef struct
{
     org_league_s *leagues;

} org_s;

org_s *convertOrg( const fileleague_s *league_data );
void   freeOrg(          org_s        *org         );

#endif
