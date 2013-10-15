#ifndef __INC_ORG_H__
#define __INC_ORG_H__

#include "file_formats.h"
#include "league.h"

typedef struct
{
     league_s *league;

} org_league_s;

typedef struct
{
     org_league_s *leagues;

} org_s;

typedef struct
{
     fileleagname_s *league_data;
     fileparks_s    *parks_data;
     fileplayer_s   *players_data;
     int             season;

} org_data_s;

org_s *convertOrg( const org_data_s *org_data );

void freeOrg( org_s *org );

#endif
