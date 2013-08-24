#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ewbfiles.h"
#include "league.h"


static int cmpplr( const void *arg1, const void *arg2 )
{
     const player_s *p1 = (player_s *)arg1;
     const player_s *p2 = (player_s *)arg2;
     /**/  int       cmp;

     if ( p1->type != p2->type ) return p1->type - p2->type;

     if ( p1->type == pt_None ) return 0;

     if ( p1->type == pt_Hitter )
     {
	  const batter_s *b1 = &(p1->data.batter);
	  const batter_s *b2 = &(p2->data.batter);

	  if ( b1->primary_pos != b2->primary_pos ) return b1->primary_pos - b2->primary_pos;

	  if ( (cmp = strcmp( b1->last_name, b2->last_name )) != 0 ) return cmp;

	  return strcmp( b1->first_name, b2->first_name );
     }

     if ( p1->type == pt_Pitcher )
     {
	  const pitcher_s *pt1 = &(p1->data.pitcher);
	  const pitcher_s *pt2 = &(p2->data.pitcher);

	  if ( (cmp = strcmp( pt1->last_name, pt2->last_name )) != 0 ) return cmp;

	  return strcmp( pt1->first_name, pt2->first_name );
     }

     return 0;
}


static void copyPlayer( player_s *player, const fileplayer_s *fileplayer )
{
     // Don't copy empty players
     if ( fileplayer->last_name[0] == 0 )
     {
	  player->type = pt_None;

	  return;
     }

     char first_name [sizeof(fileplayer->first_name)];
     char last_name  [sizeof(fileplayer->last_name) ];

     memcpy( first_name, fileplayer->first_name, sizeof(fileplayer->first_name) );
     memcpy( last_name,  fileplayer->last_name,  sizeof(fileplayer->last_name)  );

     if ( (fileplayer->position[0]>>4) == 0 )
     {
	  // Pitcher
	  player->type = pt_Pitcher;

	  pitcher_s *pitcher = &(player->data.pitcher);

	  // The last byte of each name has the high bit set, it needs to be cleared
          untermName( first_name, sizeof(fileplayer->first_name) );
          untermName( last_name,  sizeof(fileplayer->last_name)  );

	  snprintf( pitcher->first_name, sizeof(pitcher->first_name), "%.*s", sizeof(fileplayer->first_name), first_name );
	  snprintf( pitcher->last_name,  sizeof(pitcher->last_name),  "%.*s", sizeof(fileplayer->last_name),  last_name );

	  pitcher->year = fileplayer->year[0] + 1845;
	  pitcher->primary_pos   = pos_Pitcher;
	  pitcher->secondary_pos = pos_Pitcher;

	  const struct pitching_s *p = &(fileplayer->filestats.filepitching);

	  pitcher->speed        = (p->ratings[0]>>4);
	  pitcher->control      = (p->ratings[0] & 0x0F);
	  pitcher->fatigue      = (p->ratings[1]>>4);
	  pitcher->bunt         = (p->ratings[1] & 0x0F);
          pitcher->fielding_avg = 1.0 - (((float)p->real_fa[0]) / 1000.0);

	  pitching_s *stats = &(pitcher->potential);

	  stats->wins        = p->real_wins[0];
	  stats->losses      = p->real_losses[0];
	  stats->games       = p->real_games[0];
	  stats->saves       = p->real_saves[0];
	  stats->innings     = (float)word2int( p->real_inn ) / 10.0;
          stats->at_bats     = word2int( p->vl_ab   ) + word2int( p->vr_ab   );
	  stats->hits        = word2int( p->vl_hits ) + word2int( p->vr_hits );
	  stats->earned_runs = p->real_er[0];
	  stats->home_runs   = p->vl_hr[0] + p->vr_hr[0];
	  stats->walks       = p->vl_bb[0] + p->vr_bb[0];
	  stats->strike_outs = word2int( p->vl_so ) + word2int( p->vr_so );

	  const acc_pch_stats_s *simulated = &(p->simulated);

	  stats = &(pitcher->simulated);

	  stats->wins        = simulated->acc_wins[0];
	  stats->losses      = simulated->acc_losses[0];
	  stats->games       = simulated->acc_starts[0];
	  stats->saves       = simulated->acc_saves[0];
	  stats->innings     = (float)word2int( simulated->acc_innings ) / 10.0;
	  stats->hits        = word2int( simulated->acc_hits );
	  stats->earned_runs = word2int( simulated->acc_er );
	  stats->home_runs   = simulated->acc_hr[0];
	  stats->walks       = simulated->acc_bb[0];
	  stats->strike_outs = word2int( simulated->acc_so );

          float norm_inn  = ((int)stats->innings);
          /**/  norm_inn += (stats->innings - norm_inn) / 3.0;
          int   est_ab    = (int)(norm_inn * 3) + stats->hits + stats->walks;

          stats->at_bats = est_ab;
     }
     else
     {
	  // Hitter
	  player->type = pt_Hitter;

	  batter_s *batter = &(player->data.batter);

	  // The last byte of each name has the high bit set, it needs to be cleared
          untermName( first_name, sizeof(fileplayer->first_name) );
          untermName( last_name,  sizeof(fileplayer->last_name)  );

	  snprintf( batter->first_name, sizeof(batter->first_name), "%.*s", sizeof(fileplayer->first_name), first_name );
	  snprintf( batter->last_name,  sizeof(batter->last_name),  "%.*s", sizeof(fileplayer->last_name),  last_name );

	  batter->year = fileplayer->year[0] + 1845;
	  batter->primary_pos   = (fileplayer->position[0]>>4);
	  batter->secondary_pos = (fileplayer->position[0] & 0x0F);

	  const struct batting_s *b = &(fileplayer->filestats.filebatting);

	  batter->power     = (b->ratings[2]>>4);
	  batter->hit_n_run = (b->ratings[3] & 0x0F);
	  batter->bunt      = (b->ratings[3]>>4);
	  batter->running   = (b->ratings[1]>>4);
	  batter->range     = (b->ratings[1] & 0x0F);
	  batter->arm       = (b->ratings[0] & 0x0F);

          fielding_s *fielding = &(batter->fielding);

          fielding->put_outs      = word2int( b->real_po );
          fielding->assists       = word2int( b->real_as );
          fielding->errors        = b->real_err[0];
          fielding->secondary_avg = 1.0 - (((float)b->real_fa2[0]) / 1000.0);

	  hitting_s *stats = &(batter->potential);

	  stats->games        = (b->real_games[0]);
	  stats->at_bats      = word2int( b->vl_ab ) + word2int( b->vr_ab );
	  stats->runs         = (b->real_runs[0]);
	  stats->hits         = word2int( b->vl_hits ) + word2int( b->vr_hits );
	  stats->doubles      = (b->vl_2b[0] + b->vr_2b[0]);
	  stats->triples      = (b->vl_3b[0] + b->vr_3b[0]);
	  stats->home_runs    = (b->vl_hr[0] + b->vr_hr[0]);
	  stats->rbi          = (b->real_rbi[0]);
	  stats->walks        = (b->vl_bb[0] + b->vr_bb[0]);
	  stats->strike_outs  = (b->vl_so[0] + b->vr_so[0]);
	  stats->stolen_bases = (b->real_sb[0]);
	  stats->errors       = (b->real_err[0]);

	  const acc_bat_stats_s *simulated = &(b->simulated);
	  const acc_bat_stats_s *action    = &(b->action);

	  stats = &(batter->simulated);

	  stats->games        = (simulated->acc_games[0]);
	  stats->at_bats      = word2int( simulated->acc_ab );
	  stats->runs         = (simulated->acc_runs[0]);
	  stats->hits         = word2int( simulated->acc_hits );
	  stats->doubles      = (simulated->acc_2b[0]);
	  stats->triples      = (simulated->acc_3b[0]);
	  stats->home_runs    = (simulated->acc_hr[0]);
	  stats->rbi          = (simulated->acc_rbi[0] + action->acc_rbi[0]);
	  stats->walks        = (simulated->acc_bb[0]);
	  stats->strike_outs  = (simulated->acc_so[0] + action->acc_so[0]);
	  stats->stolen_bases = (simulated->acc_sb[0]);
	  stats->errors       = (simulated->acc_err[0]);
     }
}


league_s *convertFilesToLeagues( fileleague_s *leagueFile, const fileplayer_s *playerFile )
{
     league_s *leagues;
     int       leag;
     int       div;
     int       team;
     int       plyr;
     int       idx;


     if ( (leagues = malloc( sizeof(league_s) * TOTAL_LEAGUES )) == NULL )
     {
	  printf( "Cannot allocate memory for leagues.\n" );

	  return NULL;
     }

     strcpy( leagues[0].name, leagueFile->league1 );
     strcpy( leagues[1].name, leagueFile->league2 );

     strcpy( leagues[0].divisions[0].name, leagueFile->division1 );
     strcpy( leagues[0].divisions[1].name, leagueFile->division2 );
     strcpy( leagues[1].divisions[0].name, leagueFile->division3 );
     strcpy( leagues[1].divisions[1].name, leagueFile->division4 );


     // leagues[0] - div[0] - team[0] - players   0 -  24
     // ...
     // leagues[1] - div[1] - team[7] - players 775 - 799

     for ( leag = 0; leag < TOTAL_LEAGUES; ++leag )
     {
	  for ( div = 0; div < DIVISIONS_PER_LEAGUE; ++div )
	  {
	       for ( team = 0; team < TEAMS_PER_DIVISION; ++team )
	       {
		    idx = (leag * TEAMS_PER_LEAGUE) + (div * TEAMS_PER_DIVISION) + team;

		    snprintf( leagues[leag].divisions[div].teams[team].name,
			      sizeof(leagues[leag].divisions[div].teams[team].name),
			      "%.*s",
			      sizeof(leagueFile->teams[idx].name),
			      leagueFile->teams[idx].name );

		    leagues[leag].divisions[div].teams[team].wins   = leagueFile->teams[idx].wins[0];
		    leagues[leag].divisions[div].teams[team].losses = leagueFile->teams[idx].losses[0];
		    leagues[leag].divisions[div].teams[team].color  = leagueFile->teams[idx].color[0];

		    for ( plyr = 0; plyr < PLAYERS_PER_TEAM; ++plyr )
		    {
			 // copy into leagues[l].divisions[d].teams[t].players[p]
			 //      from playerFile[ (l * 400) + (d * 200) + (t * 25) + p ]
			 idx = (leag * PLAYERS_PER_LEAGUE) + (div * PLAYERS_PER_DIVISION) + (team * PLAYERS_PER_TEAM) + plyr;

			 copyPlayer( &leagues[leag].divisions[div].teams[team].players[plyr], &playerFile[idx] );
		    }

		    // For each team sort players by:
		    // 1. non-empty before empty entries
		    // 2. position
		    // 3. alphabetical

		    qsort( leagues[leag].divisions[div].teams[team].players, PLAYERS_PER_TEAM, sizeof(player_s), cmpplr );
	       }
	  }
     }

     return leagues;
}
