#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "player.h"
#include "unit_test.h"


static      player_accolade_s player_accolade_sentinel = PLAYER_ACCOLADE_SENTINEL;
static      pitcher_stats_s pitcher_stats_sentinel = PITCHER_STATS_SENTINEL;
static      pitcher_accolade_s pitcher_accolade_sentinel = PITCHER_ACCOLADE_SENTINEL;
static      batter_stats_s batter_stats_sentinel = BATTER_STATS_SENTINEL;
static      batter_accolade_s batter_accolade_sentinel = BATTER_ACCOLADE_SENTINEL;


static sqlite3 *db;
static char    *result;


static char *get_player__ShouldReturnTheMatchingPlayerObject_GivenAPlayerId()
{
     player_s expected = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( expected.player_id,       actual->player_id       );
     assertEqualsStr( expected.first_name,      actual->first_name      );
     assertEqualsStr( expected.last_name,       actual->last_name       );
     assertEqualsStr( expected.first_phoenetic, actual->first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual->last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual->skin_tone       );
     assertEqualsInt( expected.handedness,      actual->handedness      );
     assertEqualsInt( expected.player_type,     actual->player_type     );
     assertEqualsInt( expected.rookie_season,   actual->rookie_season   );
     assertEqualsInt( expected.longevity,       actual->longevity       );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );

     return NULL;
}

static char *get_player__ShoultReturnTheMatchingPlayerWithAccolades_GivenAPlayerId()
{
     player_s          expected           = { 0 };
     player_accolade_s expected_accolade1 = { 0 };
     player_accolade_s expected_accolade2 = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_accolade1.player_id = 1;
     expected_accolade1.season    = 1;
     expected_accolade1.accolade  = acc_All_Star;

     expected_accolade2.player_id = 1;
     expected_accolade2.season    = 2;
     expected_accolade2.accolade  = acc_All_Star;

     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolade2 ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     player_accolade_s *actual_accolade = actual->accolades;

     assertNotNull( actual_accolade );

     assertEqualsInt( expected_accolade1.player_id, actual_accolade[0].player_id );
     assertEqualsInt( expected_accolade1.season,    actual_accolade[0].season    );
     assertEqualsInt( expected_accolade1.accolade,  actual_accolade[0].accolade  );

     assertEqualsInt( expected_accolade2.player_id, actual_accolade[1].player_id );
     assertEqualsInt( expected_accolade2.season,    actual_accolade[1].season    );
     assertEqualsInt( expected_accolade2.accolade,  actual_accolade[1].accolade  );

     assertEqualsInt( player_accolade_sentinel.player_id, actual_accolade[2].player_id );
     assertEqualsInt( player_accolade_sentinel.season,    actual_accolade[2].season    );
     assertEqualsInt( player_accolade_sentinel.accolade,  actual_accolade[2].accolade  );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolade2 ) );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithPitchingDetails_GivenAPlayerId()
{
     player_s  expected         = { 0 };
     pitcher_s expected_pitcher = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Pitcher, actual->player_type );

     pitcher_s *actual_pitcher = actual->details.pitching;

     assertNotNull( actual_pitcher );

     assertEqualsInt( expected_pitcher.player_id, actual_pitcher->player_id );
     assertEqualsInt( expected_pitcher.speed,     actual_pitcher->speed     );
     assertEqualsInt( expected_pitcher.control,   actual_pitcher->control   );
     assertEqualsInt( expected_pitcher.bunt,      actual_pitcher->bunt      );
     assertEqualsInt( expected_pitcher.fatigue,   actual_pitcher->fatigue   );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithPitchingStats_GivenAPlayerId()
{
     player_s         expected                = { 0 };
     pitcher_s        expected_pitcher        = { 0 };
     pitcher_stats_s *expected_pitcher_stats1 = buildPitcherStats( 1, 1, sp_Regular );
     pitcher_stats_s *expected_pitcher_stats2 = buildPitcherStats( 1, 1, sp_Playoff );

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, expected_pitcher_stats1 ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, expected_pitcher_stats2 ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Pitcher, actual->player_type );

     pitcher_s *actual_pitcher = actual->details.pitching;

     assertNotNull( actual_pitcher );

     pitcher_stats_s *actual_pitcher_stats = actual_pitcher->stats;

     assertNotNull( actual_pitcher_stats );

     comparePitcherStats( expected_pitcher_stats1, &actual_pitcher_stats[0] );
     comparePitcherStats( expected_pitcher_stats2, &actual_pitcher_stats[1] );
     comparePitcherStats( &pitcher_stats_sentinel, &actual_pitcher_stats[2] );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, expected_pitcher_stats1 ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, expected_pitcher_stats2 ) );

     free( expected_pitcher_stats1 );
     free( expected_pitcher_stats2 );
     free_player( actual );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithPitchingAccolades_GivenAPlayerId()
{
     player_s           expected           = { 0 };
     pitcher_s          expected_pitcher   = { 0 };
     pitcher_accolade_s expected_accolade1 = { 0 };
     pitcher_accolade_s expected_accolade2 = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     expected_accolade1.player_id = 1;
     expected_accolade1.season    = 1;
     expected_accolade1.accolade  = pacc_Atlantic_Pitcher_of_the_Year;

     expected_accolade2.player_id = 1;
     expected_accolade2.season    = 2;
     expected_accolade2.accolade  = pacc_Most_Wins;

     assertEquals( SQLITE_OK, pitcher_accolades_t_create( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_create( db, &expected_accolade2 ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Pitcher, actual->player_type );

     pitcher_s *actual_pitcher = actual->details.pitching;

     assertNotNull( actual_pitcher );

     pitcher_accolade_s *actual_pitcher_accolades = actual_pitcher->accolades;

     assertNotNull( actual_pitcher_accolades );

     assertEqualsInt( expected_accolade1.player_id, actual_pitcher_accolades[0].player_id );
     assertEqualsInt( expected_accolade1.season,    actual_pitcher_accolades[0].season    );
     assertEqualsInt( expected_accolade1.accolade,  actual_pitcher_accolades[0].accolade  );

     assertEqualsInt( expected_accolade2.player_id, actual_pitcher_accolades[1].player_id );
     assertEqualsInt( expected_accolade2.season,    actual_pitcher_accolades[1].season    );
     assertEqualsInt( expected_accolade2.accolade,  actual_pitcher_accolades[1].accolade  );

     assertEqualsInt( pitcher_accolade_sentinel.player_id, actual_pitcher_accolades[2].player_id );
     assertEqualsInt( pitcher_accolade_sentinel.season,    actual_pitcher_accolades[2].season    );
     assertEqualsInt( pitcher_accolade_sentinel.accolade,  actual_pitcher_accolades[2].accolade  );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_delete( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_delete( db, &expected_accolade2 ) );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithBattingDetails_GivenAPlayerId()
{
     player_s expected        = { 0 };
     batter_s expected_batter = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Batter, actual->player_type );

     batter_s *actual_batter = actual->details.batting;

     assertNotNull( actual_batter );

     assertEquals( expected_batter.player_id,          actual_batter->player_id          );
     assertEquals( expected_batter.power,              actual_batter->power              );
     assertEquals( expected_batter.hit_n_run,          actual_batter->hit_n_run          );
     assertEquals( expected_batter.bunt,               actual_batter->bunt               );
     assertEquals( expected_batter.running,            actual_batter->running            );
     assertEquals( expected_batter.range,              actual_batter->range              );
     assertEquals( expected_batter.arm,                actual_batter->arm                );
     assertEquals( expected_batter.primary_position,   actual_batter->primary_position   );
     assertEquals( expected_batter.secondary_position, actual_batter->secondary_position );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithBattingStats_GivenAPlayerId()
{
     player_s       expected               = { 0 };
     batter_s       expected_batter        = { 0 };
     batter_stats_s expected_batter_stats1 = { 0 };
     batter_stats_s expected_batter_stats2 = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     expected_batter_stats1.player_id      = 1;
     expected_batter_stats1.season         = 1;
     expected_batter_stats1.season_phase   = sp_Regular;
     expected_batter_stats1.games          = 134;
     expected_batter_stats1.at_bats        = 543;
     expected_batter_stats1.runs           = 101;
     expected_batter_stats1.hits           = 220;
     expected_batter_stats1.doubles        = 35;
     expected_batter_stats1.triples        = 16;
     expected_batter_stats1.home_runs      = 56;
     expected_batter_stats1.runs_batted_in = 115;
     expected_batter_stats1.walks          = 62;
     expected_batter_stats1.strike_outs    = 131;
     expected_batter_stats1.steals         = 45;
     expected_batter_stats1.errors         = 5;

     expected_batter_stats2.player_id      = 1;
     expected_batter_stats2.season         = 2;
     expected_batter_stats2.season_phase   = sp_Regular;
     expected_batter_stats2.games          = 143;
     expected_batter_stats2.at_bats        = 555;
     expected_batter_stats2.runs           = 99;
     expected_batter_stats2.hits           = 206;
     expected_batter_stats2.doubles        = 22;
     expected_batter_stats2.triples        = 11;
     expected_batter_stats2.home_runs      = 44;
     expected_batter_stats2.runs_batted_in = 109;
     expected_batter_stats2.walks          = 58;
     expected_batter_stats2.strike_outs    = 144;
     expected_batter_stats2.steals         = 37;
     expected_batter_stats2.errors         = 8;

     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected_batter_stats1 ) );
     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected_batter_stats2 ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Batter, actual->player_type );

     batter_s *actual_batter = actual->details.batting;

     assertNotNull( actual_batter );

     batter_stats_s *actual_batter_stats = actual_batter->stats;

     assertNotNull( actual_batter_stats );

     assertEquals( expected_batter_stats1.player_id,      actual_batter_stats[0].player_id      );
     assertEquals( expected_batter_stats1.season,         actual_batter_stats[0].season         );
     assertEquals( expected_batter_stats1.season_phase,   actual_batter_stats[0].season_phase   );
     assertEquals( expected_batter_stats1.games,          actual_batter_stats[0].games          );
     assertEquals( expected_batter_stats1.at_bats,        actual_batter_stats[0].at_bats        );
     assertEquals( expected_batter_stats1.runs,           actual_batter_stats[0].runs           );
     assertEquals( expected_batter_stats1.hits,           actual_batter_stats[0].hits           );
     assertEquals( expected_batter_stats1.doubles,        actual_batter_stats[0].doubles        );
     assertEquals( expected_batter_stats1.triples,        actual_batter_stats[0].triples        );
     assertEquals( expected_batter_stats1.home_runs,      actual_batter_stats[0].home_runs      );
     assertEquals( expected_batter_stats1.runs_batted_in, actual_batter_stats[0].runs_batted_in );
     assertEquals( expected_batter_stats1.walks,          actual_batter_stats[0].walks          );
     assertEquals( expected_batter_stats1.strike_outs,    actual_batter_stats[0].strike_outs    );
     assertEquals( expected_batter_stats1.steals,         actual_batter_stats[0].steals         );
     assertEquals( expected_batter_stats1.errors,         actual_batter_stats[0].errors         );

     assertEquals( expected_batter_stats2.player_id,      actual_batter_stats[1].player_id      );
     assertEquals( expected_batter_stats2.season,         actual_batter_stats[1].season         );
     assertEquals( expected_batter_stats2.season_phase,   actual_batter_stats[1].season_phase   );
     assertEquals( expected_batter_stats2.games,          actual_batter_stats[1].games          );
     assertEquals( expected_batter_stats2.at_bats,        actual_batter_stats[1].at_bats        );
     assertEquals( expected_batter_stats2.runs,           actual_batter_stats[1].runs           );
     assertEquals( expected_batter_stats2.hits,           actual_batter_stats[1].hits           );
     assertEquals( expected_batter_stats2.doubles,        actual_batter_stats[1].doubles        );
     assertEquals( expected_batter_stats2.triples,        actual_batter_stats[1].triples        );
     assertEquals( expected_batter_stats2.home_runs,      actual_batter_stats[1].home_runs      );
     assertEquals( expected_batter_stats2.runs_batted_in, actual_batter_stats[1].runs_batted_in );
     assertEquals( expected_batter_stats2.walks,          actual_batter_stats[1].walks          );
     assertEquals( expected_batter_stats2.strike_outs,    actual_batter_stats[1].strike_outs    );
     assertEquals( expected_batter_stats2.steals,         actual_batter_stats[1].steals         );
     assertEquals( expected_batter_stats2.errors,         actual_batter_stats[1].errors         );

     assertEquals( batter_stats_sentinel.player_id,      actual_batter_stats[2].player_id      );
     assertEquals( batter_stats_sentinel.season,         actual_batter_stats[2].season         );
     assertEquals( batter_stats_sentinel.season_phase,   actual_batter_stats[2].season_phase   );
     assertEquals( batter_stats_sentinel.games,          actual_batter_stats[2].games          );
     assertEquals( batter_stats_sentinel.at_bats,        actual_batter_stats[2].at_bats        );
     assertEquals( batter_stats_sentinel.runs,           actual_batter_stats[2].runs           );
     assertEquals( batter_stats_sentinel.hits,           actual_batter_stats[2].hits           );
     assertEquals( batter_stats_sentinel.doubles,        actual_batter_stats[2].doubles        );
     assertEquals( batter_stats_sentinel.triples,        actual_batter_stats[2].triples        );
     assertEquals( batter_stats_sentinel.home_runs,      actual_batter_stats[2].home_runs      );
     assertEquals( batter_stats_sentinel.runs_batted_in, actual_batter_stats[2].runs_batted_in );
     assertEquals( batter_stats_sentinel.walks,          actual_batter_stats[2].walks          );
     assertEquals( batter_stats_sentinel.strike_outs,    actual_batter_stats[2].strike_outs    );
     assertEquals( batter_stats_sentinel.steals,         actual_batter_stats[2].steals         );
     assertEquals( batter_stats_sentinel.errors,         actual_batter_stats[2].errors         );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );
     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected_batter_stats1 ) );
     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected_batter_stats2 ) );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithBattingAccolades_GivenAPlayerId()
{
     player_s          expected                  = { 0 };
     batter_s          expected_batter           = { 0 };
     batter_accolade_s expected_batter_accolade1 = { 0 };
     batter_accolade_s expected_batter_accolade2 = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     expected_batter_accolade1.player_id = 1;
     expected_batter_accolade1.season    = 1;
     expected_batter_accolade1.accolade  = bacc_Best_Batting_Average;

     expected_batter_accolade2.player_id = 1;
     expected_batter_accolade2.season    = 1;
     expected_batter_accolade2.accolade  = bacc_Most_Home_Runs;

     assertEquals( SQLITE_OK, batter_accolades_t_create( db, &expected_batter_accolade1 ) );
     assertEquals( SQLITE_OK, batter_accolades_t_create( db, &expected_batter_accolade2 ) );

     player_s *actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Batter, actual->player_type );

     batter_s *actual_batter = actual->details.batting;

     assertNotNull( actual_batter );

     batter_accolade_s *actual_batter_accolades = actual_batter->accolades;

     assertNotNull( actual_batter_accolades );

     assertEquals( expected_batter_accolade1.player_id, actual_batter_accolades[0].player_id );
     assertEquals( expected_batter_accolade1.season,    actual_batter_accolades[0].season    );
     assertEquals( expected_batter_accolade1.accolade,  actual_batter_accolades[0].accolade  );

     assertEquals( expected_batter_accolade2.player_id, actual_batter_accolades[1].player_id );
     assertEquals( expected_batter_accolade2.season,    actual_batter_accolades[1].season    );
     assertEquals( expected_batter_accolade2.accolade,  actual_batter_accolades[1].accolade  );

     assertEquals( batter_accolade_sentinel.player_id, actual_batter_accolades[2].player_id );
     assertEquals( batter_accolade_sentinel.season,    actual_batter_accolades[2].season    );
     assertEquals( batter_accolade_sentinel.accolade,  actual_batter_accolades[2].accolade  );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );
     assertEquals( SQLITE_OK, batter_accolades_t_delete( db, &expected_batter_accolade1 ) );
     assertEquals( SQLITE_OK, batter_accolades_t_delete( db, &expected_batter_accolade2 ) );

     return NULL;
}

static char *save_player__ShouldSaveThePlayerToTheDatabase_GivenAPlayerObject()
{
     player_s expected = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( expected.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );

     return NULL;
}

static char *save_player__ShouldOnlyUpdatePhoeneticsIfThePlayerExists_GivenAPlayerObject()
{
     player_s expected = { 0 };
     player_s modified = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     modified = expected;

     strcpy( modified.first_name,       "FirstName2" );
     strcpy( modified.last_name,        "LastName2"  );
     strcpy( modified.first_phoenetic,  "FirstPho2"  );
     strcpy( modified.last_phoenetic,   "LastPho2"   );
     /**/    modified.skin_tone        = st_Dark;
     /**/    modified.handedness       = hnd_Left;
     /**/    modified.player_type      = pt_Batter;
     /**/    modified.rookie_season    = 2;
     /**/    modified.longevity        = 3;

     assertEquals( SQLITE_OK, save_player( db, &modified ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( modified.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( modified.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );

     return NULL;
}

static char *save_player__ShouldSavePlayerAccoladesToTheDatabase_GivenAPlayerWithAccolades()
{
     player_s          expected              = { 0 };
     player_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_accolades[0].player_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = acc_All_Star;

     expected_accolades[1].player_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = acc_All_Star;

     expected_accolades[2] = player_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     player_accolade_s *actual_accolades = list.data;

     assertNotNull( actual_accolades );

     assertEqualsInt( expected_accolades[0].player_id, actual_accolades[0].player_id );
     assertEqualsInt( expected_accolades[0].season,    actual_accolades[0].season    );
     assertEqualsInt( expected_accolades[0].accolade,  actual_accolades[0].accolade  );

     assertEqualsInt( expected_accolades[1].player_id, actual_accolades[1].player_id );
     assertEqualsInt( expected_accolades[1].season,    actual_accolades[1].season    );
     assertEqualsInt( expected_accolades[1].accolade,  actual_accolades[1].accolade  );

     free( actual_accolades );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *save_player__ShouldSavePitcherDetailsToTheDatabase_GivenAPlayerWithPitchingDetails()
{
     player_s  expected         = { 0 };
     pitcher_s expected_pitcher = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     expected.details.pitching = &expected_pitcher;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     pitcher_s actual_pitcher = { 0 };

     actual_pitcher.player_id = expected_pitcher.player_id;

     assertEquals( SQLITE_OK, pitchers_t_read( db, &actual_pitcher ) );

     assertEqualsInt( expected_pitcher.player_id, actual_pitcher.player_id );
     assertEqualsInt( expected_pitcher.speed,     actual_pitcher.speed     );
     assertEqualsInt( expected_pitcher.control,   actual_pitcher.control   );
     assertEqualsInt( expected_pitcher.bunt,      actual_pitcher.bunt      );
     assertEqualsInt( expected_pitcher.fatigue,   actual_pitcher.fatigue   );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );

     return NULL;
}

static char *save_player__ShouldSavePitcherStatsToTheDatabase_GivenAPlayerWithPitchingStats()
{
     player_s        expected                  = { 0 };
     pitcher_s       expected_pitcher          = { 0 };
     pitcher_stats_s expected_pitcher_stats[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     buildIntoPitcherStats( &expected_pitcher_stats[0], 1, 1, sp_Regular );
     buildIntoPitcherStats( &expected_pitcher_stats[1], 1, 1, sp_Playoff );

     expected_pitcher_stats[2] = pitcher_stats_sentinel;

     expected_pitcher.stats    =  expected_pitcher_stats;
     expected.details.pitching = &expected_pitcher;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, pitcher_stats_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     pitcher_stats_s *actual_pitcher_stats = list.data;

     assertNotNull( actual_pitcher_stats );

     comparePitcherStats( &expected_pitcher_stats[0], &actual_pitcher_stats[0] );
     comparePitcherStats( &expected_pitcher_stats[1], &actual_pitcher_stats[1] );

     free( actual_pitcher_stats );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, &expected_pitcher_stats[0] ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, &expected_pitcher_stats[1] ) );

     return NULL;
}

static char *save_player__ShouldSavePitcherAccoladesToTheDatabase_GivenAPlayerWithPitchingAccolades()
{
     player_s           expected              = { 0 };
     pitcher_s          expected_pitcher      = { 0 };
     pitcher_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     expected_accolades[0].player_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = pacc_Atlantic_Pitcher_of_the_Year;

     expected_accolades[1].player_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = pacc_Most_Wins;

     expected_accolades[2] = pitcher_accolade_sentinel;

     expected_pitcher.accolades =  expected_accolades;
     expected.details.pitching  = &expected_pitcher;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, pitcher_accolades_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     pitcher_accolade_s *actual_pitcher_accolades = list.data;

     assertNotNull( actual_pitcher_accolades );

     assertEqualsInt( expected_accolades[0].player_id, actual_pitcher_accolades[0].player_id );
     assertEqualsInt( expected_accolades[0].season,    actual_pitcher_accolades[0].season    );
     assertEqualsInt( expected_accolades[0].accolade,  actual_pitcher_accolades[0].accolade  );

     assertEqualsInt( expected_accolades[1].player_id, actual_pitcher_accolades[1].player_id );
     assertEqualsInt( expected_accolades[1].season,    actual_pitcher_accolades[1].season    );
     assertEqualsInt( expected_accolades[1].accolade,  actual_pitcher_accolades[1].accolade  );

     free( actual_pitcher_accolades );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *save_player__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenAPitcher()
{
     player_s           expected                      = { 0 };
     player_accolade_s  expected_accolades[3]         = { 0 };
     pitcher_s          expected_pitcher              = { 0 };
     pitcher_stats_s    expected_pitcher_stats[3]     = { 0 };
     pitcher_accolade_s expected_pitcher_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_accolades[0].player_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = acc_All_Star;

     expected_accolades[1].player_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = acc_All_Star;

     expected_accolades[2] = player_accolade_sentinel;

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     buildIntoPitcherStats( &expected_pitcher_stats[0], 1, 1, sp_Regular );
     buildIntoPitcherStats( &expected_pitcher_stats[1], 1, 1, sp_Playoff );

     expected_pitcher_stats[2] = pitcher_stats_sentinel;

     expected_pitcher_accolades[0].player_id = 1;
     expected_pitcher_accolades[0].season    = 1;
     expected_pitcher_accolades[0].accolade  = pacc_Atlantic_Pitcher_of_the_Year;

     expected_pitcher_accolades[1].player_id = 1;
     expected_pitcher_accolades[1].season    = 2;
     expected_pitcher_accolades[1].accolade  = pacc_Most_Wins;

     expected_pitcher_accolades[2] = pitcher_accolade_sentinel;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, &expected_pitcher_stats[0] ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_create( db, &expected_pitcher_accolades[0] ) );

     expected_pitcher_stats[0].wins   = 18;
     expected_pitcher_stats[0].losses = 12;

     expected_pitcher.stats     =  expected_pitcher_stats;
     expected_pitcher.accolades =  expected_pitcher_accolades;
     expected.details.pitching  = &expected_pitcher;
     expected.accolades         =  expected_accolades;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( expected.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     data_list_s accolade_list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected.player_id, &accolade_list ) );

     assertEquals( 2, accolade_list.count );

     player_accolade_s *actual_accolades = accolade_list.data;

     assertNotNull( actual_accolades );

     assertEqualsInt( expected_accolades[0].player_id, actual_accolades[0].player_id );
     assertEqualsInt( expected_accolades[0].season,    actual_accolades[0].season    );
     assertEqualsInt( expected_accolades[0].accolade,  actual_accolades[0].accolade  );

     assertEqualsInt( expected_accolades[1].player_id, actual_accolades[1].player_id );
     assertEqualsInt( expected_accolades[1].season,    actual_accolades[1].season    );
     assertEqualsInt( expected_accolades[1].accolade,  actual_accolades[1].accolade  );

     free( actual_accolades );

     pitcher_s actual_pitcher = { 0 };

     actual_pitcher.player_id = expected_pitcher.player_id;

     assertEquals( SQLITE_OK, pitchers_t_read( db, &actual_pitcher ) );

     assertEqualsInt( expected_pitcher.player_id, actual_pitcher.player_id );
     assertEqualsInt( expected_pitcher.speed,     actual_pitcher.speed     );
     assertEqualsInt( expected_pitcher.control,   actual_pitcher.control   );
     assertEqualsInt( expected_pitcher.bunt,      actual_pitcher.bunt      );
     assertEqualsInt( expected_pitcher.fatigue,   actual_pitcher.fatigue   );

     data_list_s pitcher_stats_list = { 0 };

     assertEquals( SQLITE_OK, pitcher_stats_t_read_by_player( db, expected.player_id, &pitcher_stats_list ) );

     assertEquals( 2, pitcher_stats_list.count );

     pitcher_stats_s *actual_pitcher_stats = pitcher_stats_list.data;

     assertNotNull( actual_pitcher_stats );

     comparePitcherStats( &expected_pitcher_stats[0], &actual_pitcher_stats[0] );
     comparePitcherStats( &expected_pitcher_stats[1], &actual_pitcher_stats[1] );

     free( actual_pitcher_stats );

     data_list_s pitcher_accolade_list = { 0 };

     assertEquals( SQLITE_OK, pitcher_accolades_t_read_by_player( db, expected.player_id, &pitcher_accolade_list ) );

     assertEquals( 2, pitcher_accolade_list.count );

     pitcher_accolade_s *actual_pitcher_accolades = pitcher_accolade_list.data;

     assertNotNull( actual_pitcher_accolades );

     assertEqualsInt( expected_pitcher_accolades[0].player_id, actual_pitcher_accolades[0].player_id );
     assertEqualsInt( expected_pitcher_accolades[0].season,    actual_pitcher_accolades[0].season    );
     assertEqualsInt( expected_pitcher_accolades[0].accolade,  actual_pitcher_accolades[0].accolade  );

     assertEqualsInt( expected_pitcher_accolades[1].player_id, actual_pitcher_accolades[1].player_id );
     assertEqualsInt( expected_pitcher_accolades[1].season,    actual_pitcher_accolades[1].season    );
     assertEqualsInt( expected_pitcher_accolades[1].accolade,  actual_pitcher_accolades[1].accolade  );

     free( actual_pitcher_accolades );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[1] ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, &expected_pitcher_stats[0] ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, &expected_pitcher_stats[1] ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_delete( db, &expected_pitcher_accolades[0] ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_delete( db, &expected_pitcher_accolades[1] ) );

     return NULL;
}

static char *save_player__ShouldNotUpdatePitcherRecordsIfTheyExist_GivenAPitcher()
{
     player_s           expected                      = { 0 };
     pitcher_s          modified_pitcher              = { 0 };
     pitcher_s          expected_pitcher              = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     modified_pitcher = expected_pitcher;

     modified_pitcher.speed     = 8;
     modified_pitcher.control   = 6;
     modified_pitcher.bunt      = 4;
     modified_pitcher.fatigue   = 9;

     expected.details.pitching  = &modified_pitcher;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( expected.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     pitcher_s actual_pitcher = { 0 };

     actual_pitcher.player_id = expected_pitcher.player_id;

     assertEquals( SQLITE_OK, pitchers_t_read( db, &actual_pitcher ) );

     assertEqualsInt( expected_pitcher.player_id, actual_pitcher.player_id );
     assertEqualsInt( expected_pitcher.speed,     actual_pitcher.speed     );
     assertEqualsInt( expected_pitcher.control,   actual_pitcher.control   );
     assertEqualsInt( expected_pitcher.bunt,      actual_pitcher.bunt      );
     assertEqualsInt( expected_pitcher.fatigue,   actual_pitcher.fatigue   );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );

     return NULL;
}

static char *save_player__ShouldSaveBatterDetailsToTheDatabase_GivenAPlayerWithBattingDetails()
{
     player_s expected        = { 0 };
     batter_s expected_batter = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     expected.details.batting = &expected_batter;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     batter_s actual_batter = { 0 };

     actual_batter.player_id = expected_batter.player_id;

     assertEquals( SQLITE_OK, batters_t_read( db, &actual_batter ) );

     assertEquals( expected_batter.player_id,          actual_batter.player_id          );
     assertEquals( expected_batter.power,              actual_batter.power              );
     assertEquals( expected_batter.hit_n_run,          actual_batter.hit_n_run          );
     assertEquals( expected_batter.bunt,               actual_batter.bunt               );
     assertEquals( expected_batter.running,            actual_batter.running            );
     assertEquals( expected_batter.range,              actual_batter.range              );
     assertEquals( expected_batter.arm,                actual_batter.arm                );
     assertEquals( expected_batter.primary_position,   actual_batter.primary_position   );
     assertEquals( expected_batter.secondary_position, actual_batter.secondary_position );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );

     return NULL;
}

static char *save_player__ShouldSaveBatterStatsToTheDatabase_GivenAPlayerWithBattingStats()
{
     player_s       expected                 = { 0 };
     batter_s       expected_batter          = { 0 };
     batter_stats_s expected_batter_stats[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     expected_batter_stats[0].player_id      = 1;
     expected_batter_stats[0].season         = 1;
     expected_batter_stats[0].season_phase   = sp_Regular;
     expected_batter_stats[0].games          = 134;
     expected_batter_stats[0].at_bats        = 543;
     expected_batter_stats[0].runs           = 101;
     expected_batter_stats[0].hits           = 220;
     expected_batter_stats[0].doubles        = 35;
     expected_batter_stats[0].triples        = 16;
     expected_batter_stats[0].home_runs      = 56;
     expected_batter_stats[0].runs_batted_in = 115;
     expected_batter_stats[0].walks          = 62;
     expected_batter_stats[0].strike_outs    = 131;
     expected_batter_stats[0].steals         = 45;
     expected_batter_stats[0].errors         = 5;

     expected_batter_stats[1].player_id      = 1;
     expected_batter_stats[1].season         = 2;
     expected_batter_stats[1].season_phase   = sp_Regular;
     expected_batter_stats[1].games          = 143;
     expected_batter_stats[1].at_bats        = 555;
     expected_batter_stats[1].runs           = 99;
     expected_batter_stats[1].hits           = 206;
     expected_batter_stats[1].doubles        = 22;
     expected_batter_stats[1].triples        = 11;
     expected_batter_stats[1].home_runs      = 44;
     expected_batter_stats[1].runs_batted_in = 109;
     expected_batter_stats[1].walks          = 58;
     expected_batter_stats[1].strike_outs    = 144;
     expected_batter_stats[1].steals         = 37;
     expected_batter_stats[1].errors         = 8;

     expected_batter_stats[2] = batter_stats_sentinel;

     expected_batter.stats    = expected_batter_stats;
     expected.details.batting = &expected_batter;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, batter_stats_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     batter_stats_s *actual_batter_stats = list.data;

     assertNotNull( actual_batter_stats );

     assertEquals( expected_batter_stats[0].player_id,      actual_batter_stats[0].player_id      );
     assertEquals( expected_batter_stats[0].season,         actual_batter_stats[0].season         );
     assertEquals( expected_batter_stats[0].season_phase,   actual_batter_stats[0].season_phase   );
     assertEquals( expected_batter_stats[0].games,          actual_batter_stats[0].games          );
     assertEquals( expected_batter_stats[0].at_bats,        actual_batter_stats[0].at_bats        );
     assertEquals( expected_batter_stats[0].runs,           actual_batter_stats[0].runs           );
     assertEquals( expected_batter_stats[0].hits,           actual_batter_stats[0].hits           );
     assertEquals( expected_batter_stats[0].doubles,        actual_batter_stats[0].doubles        );
     assertEquals( expected_batter_stats[0].triples,        actual_batter_stats[0].triples        );
     assertEquals( expected_batter_stats[0].home_runs,      actual_batter_stats[0].home_runs      );
     assertEquals( expected_batter_stats[0].runs_batted_in, actual_batter_stats[0].runs_batted_in );
     assertEquals( expected_batter_stats[0].walks,          actual_batter_stats[0].walks          );
     assertEquals( expected_batter_stats[0].strike_outs,    actual_batter_stats[0].strike_outs    );
     assertEquals( expected_batter_stats[0].steals,         actual_batter_stats[0].steals         );
     assertEquals( expected_batter_stats[0].errors,         actual_batter_stats[0].errors         );

     assertEquals( expected_batter_stats[1].player_id,      actual_batter_stats[1].player_id      );
     assertEquals( expected_batter_stats[1].season,         actual_batter_stats[1].season         );
     assertEquals( expected_batter_stats[1].season_phase,   actual_batter_stats[1].season_phase   );
     assertEquals( expected_batter_stats[1].games,          actual_batter_stats[1].games          );
     assertEquals( expected_batter_stats[1].at_bats,        actual_batter_stats[1].at_bats        );
     assertEquals( expected_batter_stats[1].runs,           actual_batter_stats[1].runs           );
     assertEquals( expected_batter_stats[1].hits,           actual_batter_stats[1].hits           );
     assertEquals( expected_batter_stats[1].doubles,        actual_batter_stats[1].doubles        );
     assertEquals( expected_batter_stats[1].triples,        actual_batter_stats[1].triples        );
     assertEquals( expected_batter_stats[1].home_runs,      actual_batter_stats[1].home_runs      );
     assertEquals( expected_batter_stats[1].runs_batted_in, actual_batter_stats[1].runs_batted_in );
     assertEquals( expected_batter_stats[1].walks,          actual_batter_stats[1].walks          );
     assertEquals( expected_batter_stats[1].strike_outs,    actual_batter_stats[1].strike_outs    );
     assertEquals( expected_batter_stats[1].steals,         actual_batter_stats[1].steals         );
     assertEquals( expected_batter_stats[1].errors,         actual_batter_stats[1].errors         );

     free( actual_batter_stats );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );
     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected_batter_stats[0] ) );
     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected_batter_stats[1] ) );

     return NULL;
}

static char *save_player__ShouldSaveBatterAccoladesToTheDatabase_GivenAPlayerWithBattingAccolades()
{
     player_s          expected                     = { 0 };
     batter_s          expected_batter              = { 0 };
     batter_accolade_s expected_batter_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     expected_batter_accolades[0].player_id = 1;
     expected_batter_accolades[0].season    = 1;
     expected_batter_accolades[0].accolade  = bacc_Best_Batting_Average;

     expected_batter_accolades[1].player_id = 1;
     expected_batter_accolades[1].season    = 1;
     expected_batter_accolades[1].accolade  = bacc_Most_Home_Runs;

     expected_batter_accolades[2] = batter_accolade_sentinel;

     expected_batter.accolades =  expected_batter_accolades;
     expected.details.batting  = &expected_batter;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, batter_accolades_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     batter_accolade_s *actual_batter_accolades = list.data;

     assertNotNull( actual_batter_accolades );

     assertEquals( expected_batter_accolades[0].player_id, actual_batter_accolades[0].player_id );
     assertEquals( expected_batter_accolades[0].season,    actual_batter_accolades[0].season    );
     assertEquals( expected_batter_accolades[0].accolade,  actual_batter_accolades[0].accolade  );

     assertEquals( expected_batter_accolades[1].player_id, actual_batter_accolades[1].player_id );
     assertEquals( expected_batter_accolades[1].season,    actual_batter_accolades[1].season    );
     assertEquals( expected_batter_accolades[1].accolade,  actual_batter_accolades[1].accolade  );

     free( actual_batter_accolades );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );
     assertEquals( SQLITE_OK, batter_accolades_t_delete( db, &expected_batter_accolades[0] ) );
     assertEquals( SQLITE_OK, batter_accolades_t_delete( db, &expected_batter_accolades[1] ) );

     return NULL;

}

static char *save_player__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenABatter()
{
     player_s          expected                     = { 0 };
     player_accolade_s expected_accolades[3]        = { 0 };
     batter_s          expected_batter              = { 0 };
     batter_stats_s    expected_batter_stats[3]     = { 0 };
     batter_accolade_s expected_batter_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_accolades[0].player_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = acc_All_Star;

     expected_accolades[1].player_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = acc_All_Star;

     expected_accolades[2] = player_accolade_sentinel;

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     expected_batter_stats[0].player_id      = 1;
     expected_batter_stats[0].season         = 1;
     expected_batter_stats[0].season_phase   = sp_Regular;
     expected_batter_stats[0].games          = 134;
     expected_batter_stats[0].at_bats        = 543;
     expected_batter_stats[0].runs           = 101;
     expected_batter_stats[0].hits           = 220;
     expected_batter_stats[0].doubles        = 35;
     expected_batter_stats[0].triples        = 16;
     expected_batter_stats[0].home_runs      = 56;
     expected_batter_stats[0].runs_batted_in = 115;
     expected_batter_stats[0].walks          = 62;
     expected_batter_stats[0].strike_outs    = 131;
     expected_batter_stats[0].steals         = 45;
     expected_batter_stats[0].errors         = 5;

     expected_batter_stats[1].player_id      = 1;
     expected_batter_stats[1].season         = 2;
     expected_batter_stats[1].season_phase   = sp_Regular;
     expected_batter_stats[1].games          = 143;
     expected_batter_stats[1].at_bats        = 555;
     expected_batter_stats[1].runs           = 99;
     expected_batter_stats[1].hits           = 206;
     expected_batter_stats[1].doubles        = 22;
     expected_batter_stats[1].triples        = 11;
     expected_batter_stats[1].home_runs      = 44;
     expected_batter_stats[1].runs_batted_in = 109;
     expected_batter_stats[1].walks          = 58;
     expected_batter_stats[1].strike_outs    = 144;
     expected_batter_stats[1].steals         = 37;
     expected_batter_stats[1].errors         = 8;

     expected_batter_stats[2] = batter_stats_sentinel;

     expected_batter_accolades[0].player_id = 1;
     expected_batter_accolades[0].season    = 1;
     expected_batter_accolades[0].accolade  = bacc_Best_Batting_Average;

     expected_batter_accolades[1].player_id = 1;
     expected_batter_accolades[1].season    = 1;
     expected_batter_accolades[1].accolade  = bacc_Most_Home_Runs;

     expected_batter_accolades[2] = batter_accolade_sentinel;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );
     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected_batter_stats[0] ) );
     assertEquals( SQLITE_OK, batter_accolades_t_create( db, &expected_batter_accolades[0] ) );

     expected_batter.stats     =  expected_batter_stats;
     expected_batter.accolades =  expected_batter_accolades;
     expected.details.batting  = &expected_batter;
     expected.accolades        =  expected_accolades;

     expected_batter_stats[0].games   = 141;
     expected_batter_stats[0].at_bats = 567;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( expected.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     data_list_s accolade_list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected.player_id, &accolade_list ) );

     assertEquals( 2, accolade_list.count );

     player_accolade_s *actual_accolades = accolade_list.data;

     assertNotNull( actual_accolades );

     assertEqualsInt( expected_accolades[0].player_id, actual_accolades[0].player_id );
     assertEqualsInt( expected_accolades[0].season,    actual_accolades[0].season    );
     assertEqualsInt( expected_accolades[0].accolade,  actual_accolades[0].accolade  );

     assertEqualsInt( expected_accolades[1].player_id, actual_accolades[1].player_id );
     assertEqualsInt( expected_accolades[1].season,    actual_accolades[1].season    );
     assertEqualsInt( expected_accolades[1].accolade,  actual_accolades[1].accolade  );

     free( actual_accolades );

     batter_s actual_batter = { 0 };

     actual_batter.player_id = expected_batter.player_id;

     assertEquals( SQLITE_OK, batters_t_read( db, &actual_batter ) );

     assertEquals( expected_batter.player_id,          actual_batter.player_id          );
     assertEquals( expected_batter.power,              actual_batter.power              );
     assertEquals( expected_batter.hit_n_run,          actual_batter.hit_n_run          );
     assertEquals( expected_batter.bunt,               actual_batter.bunt               );
     assertEquals( expected_batter.running,            actual_batter.running            );
     assertEquals( expected_batter.range,              actual_batter.range              );
     assertEquals( expected_batter.arm,                actual_batter.arm                );
     assertEquals( expected_batter.primary_position,   actual_batter.primary_position   );
     assertEquals( expected_batter.secondary_position, actual_batter.secondary_position );

     data_list_s batter_stats_list = { 0 };

     assertEquals( SQLITE_OK, batter_stats_t_read_by_player( db, expected.player_id, &batter_stats_list ) );

     assertEquals( 2, batter_stats_list.count );

     batter_stats_s *actual_batter_stats = batter_stats_list.data;

     assertNotNull( actual_batter_stats );

     assertEquals( expected_batter_stats[0].player_id,      actual_batter_stats[0].player_id      );
     assertEquals( expected_batter_stats[0].season,         actual_batter_stats[0].season         );
     assertEquals( expected_batter_stats[0].season_phase,   actual_batter_stats[0].season_phase   );
     assertEquals( expected_batter_stats[0].games,          actual_batter_stats[0].games          );
     assertEquals( expected_batter_stats[0].at_bats,        actual_batter_stats[0].at_bats        );
     assertEquals( expected_batter_stats[0].runs,           actual_batter_stats[0].runs           );
     assertEquals( expected_batter_stats[0].hits,           actual_batter_stats[0].hits           );
     assertEquals( expected_batter_stats[0].doubles,        actual_batter_stats[0].doubles        );
     assertEquals( expected_batter_stats[0].triples,        actual_batter_stats[0].triples        );
     assertEquals( expected_batter_stats[0].home_runs,      actual_batter_stats[0].home_runs      );
     assertEquals( expected_batter_stats[0].runs_batted_in, actual_batter_stats[0].runs_batted_in );
     assertEquals( expected_batter_stats[0].walks,          actual_batter_stats[0].walks          );
     assertEquals( expected_batter_stats[0].strike_outs,    actual_batter_stats[0].strike_outs    );
     assertEquals( expected_batter_stats[0].steals,         actual_batter_stats[0].steals         );
     assertEquals( expected_batter_stats[0].errors,         actual_batter_stats[0].errors         );

     assertEquals( expected_batter_stats[1].player_id,      actual_batter_stats[1].player_id      );
     assertEquals( expected_batter_stats[1].season,         actual_batter_stats[1].season         );
     assertEquals( expected_batter_stats[1].season_phase,   actual_batter_stats[1].season_phase   );
     assertEquals( expected_batter_stats[1].games,          actual_batter_stats[1].games          );
     assertEquals( expected_batter_stats[1].at_bats,        actual_batter_stats[1].at_bats        );
     assertEquals( expected_batter_stats[1].runs,           actual_batter_stats[1].runs           );
     assertEquals( expected_batter_stats[1].hits,           actual_batter_stats[1].hits           );
     assertEquals( expected_batter_stats[1].doubles,        actual_batter_stats[1].doubles        );
     assertEquals( expected_batter_stats[1].triples,        actual_batter_stats[1].triples        );
     assertEquals( expected_batter_stats[1].home_runs,      actual_batter_stats[1].home_runs      );
     assertEquals( expected_batter_stats[1].runs_batted_in, actual_batter_stats[1].runs_batted_in );
     assertEquals( expected_batter_stats[1].walks,          actual_batter_stats[1].walks          );
     assertEquals( expected_batter_stats[1].strike_outs,    actual_batter_stats[1].strike_outs    );
     assertEquals( expected_batter_stats[1].steals,         actual_batter_stats[1].steals         );
     assertEquals( expected_batter_stats[1].errors,         actual_batter_stats[1].errors         );

     free( actual_batter_stats );

     data_list_s batter_accolade_list = { 0 };

     assertEquals( SQLITE_OK, batter_accolades_t_read_by_player( db, expected.player_id, &batter_accolade_list ) );

     assertEquals( 2, batter_accolade_list.count );

     batter_accolade_s *actual_batter_accolades = batter_accolade_list.data;

     assertNotNull( actual_batter_accolades );

     assertEquals( expected_batter_accolades[0].player_id, actual_batter_accolades[0].player_id );
     assertEquals( expected_batter_accolades[0].season,    actual_batter_accolades[0].season    );
     assertEquals( expected_batter_accolades[0].accolade,  actual_batter_accolades[0].accolade  );

     assertEquals( expected_batter_accolades[1].player_id, actual_batter_accolades[1].player_id );
     assertEquals( expected_batter_accolades[1].season,    actual_batter_accolades[1].season    );
     assertEquals( expected_batter_accolades[1].accolade,  actual_batter_accolades[1].accolade  );

     free( actual_batter_accolades );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[1] ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );
     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected_batter_stats[0] ) );
     assertEquals( SQLITE_OK, batter_stats_t_delete( db, &expected_batter_stats[1] ) );
     assertEquals( SQLITE_OK, batter_accolades_t_delete( db, &expected_batter_accolades[0] ) );
     assertEquals( SQLITE_OK, batter_accolades_t_delete( db, &expected_batter_accolades[1] ) );

     return NULL;
}

static char *save_player__ShouldNotUpdateBatterRecordsIfTheyExist_GivenABatter()
{
     player_s          expected                     = { 0 };
     batter_s          modified_batter              = { 0 };
     batter_s          expected_batter              = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     modified_batter = expected_batter;

     modified_batter.power              = 8;
     modified_batter.hit_n_run          = 6;
     modified_batter.bunt               = 2;
     modified_batter.running            = 5;
     modified_batter.range              = 4;
     modified_batter.arm                = 7;
     modified_batter.primary_position   = pos_FirstBase;
     modified_batter.secondary_position = pos_LeftField;

     expected.details.batting  = &modified_batter;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     assertEqualsInt( expected.player_id,       actual.player_id       );
     assertEqualsStr( expected.first_name,      actual.first_name      );
     assertEqualsStr( expected.last_name,       actual.last_name       );
     assertEqualsStr( expected.first_phoenetic, actual.first_phoenetic );
     assertEqualsStr( expected.last_phoenetic,  actual.last_phoenetic  );
     assertEqualsInt( expected.skin_tone,       actual.skin_tone       );
     assertEqualsInt( expected.handedness,      actual.handedness      );
     assertEqualsInt( expected.player_type,     actual.player_type     );
     assertEqualsInt( expected.rookie_season,   actual.rookie_season   );
     assertEqualsInt( expected.longevity,       actual.longevity       );

     batter_s actual_batter = { 0 };

     actual_batter.player_id = expected_batter.player_id;

     assertEquals( SQLITE_OK, batters_t_read( db, &actual_batter ) );

     assertEquals( expected_batter.player_id,          actual_batter.player_id          );
     assertEquals( expected_batter.power,              actual_batter.power              );
     assertEquals( expected_batter.hit_n_run,          actual_batter.hit_n_run          );
     assertEquals( expected_batter.bunt,               actual_batter.bunt               );
     assertEquals( expected_batter.running,            actual_batter.running            );
     assertEquals( expected_batter.range,              actual_batter.range              );
     assertEquals( expected_batter.arm,                actual_batter.arm                );
     assertEquals( expected_batter.primary_position,   actual_batter.primary_position   );
     assertEquals( expected_batter.secondary_position, actual_batter.secondary_position );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, batters_t_delete( db, &expected_batter ) );

     return NULL;
}

static char *remove_player__ShouldRemoveThePlayerFromTheDatabase_GivenAPlayerObject()
{
     player_s expected = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_NOTFOUND, players_t_read( db, &actual ) );

     return NULL;
}

static char *remove_player__ShouldRemoveThePlayerAccoladesFromTheDatabase_GivenAPlayerWithAccolades()
{
     player_s          expected              = { 0 };
     player_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_accolades[0].player_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = acc_All_Star;

     expected_accolades[1].player_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = acc_All_Star;

     expected_accolades[2] = player_accolade_sentinel;

     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolades[1] ) );

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, player_accolades_t_read_by_player( db, expected.player_id, &list ) );

     return NULL;
}

static char *remove_player__ShouldRemoveThePitchingDetailsFromTheDatabase_GivenAPlayerWithPitchingDetails()
{
     player_s  expected         = { 0 };
     pitcher_s expected_pitcher = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     expected.details.pitching = &expected_pitcher;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     pitcher_s actual_pitcher = { 0 };

     actual_pitcher.player_id = expected.player_id;

     assertEquals( SQLITE_NOTFOUND, pitchers_t_read( db, &actual_pitcher ) );

     return NULL;
}

static char *remove_player__ShouldRemoveThePitchingStatsFromTheDatabase_GivenAPlayerWithPitchingStats()
{
     player_s        expected                  = { 0 };
     pitcher_s       expected_pitcher          = { 0 };
     pitcher_stats_s expected_pitcher_stats[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     buildIntoPitcherStats( &expected_pitcher_stats[0], 1, 1, sp_Regular );
     buildIntoPitcherStats( &expected_pitcher_stats[1], 1, 1, sp_Playoff );

     expected_pitcher_stats[2] = pitcher_stats_sentinel;

     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, &expected_pitcher_stats[0] ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, &expected_pitcher_stats[1] ) );

     expected_pitcher.stats    =  expected_pitcher_stats;
     expected.details.pitching = &expected_pitcher;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, pitcher_stats_t_read_by_player( db, expected.player_id, &list ) );

     return NULL;
}

static char *remove_player__ShouldRemoveThePitchingAccoladesFromTheDatabase_GivenAPlayerWithPitchingAccolades()
{
     player_s           expected              = { 0 };
     pitcher_s          expected_pitcher      = { 0 };
     pitcher_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Pitcher;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     expected_accolades[0].player_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = pacc_Atlantic_Pitcher_of_the_Year;

     expected_accolades[1].player_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = pacc_Most_Wins;

     expected_accolades[2] = pitcher_accolade_sentinel;

     assertEquals( SQLITE_OK, pitcher_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, pitcher_accolades_t_create( db, &expected_accolades[1] ) );

     expected_pitcher.accolades =  expected_accolades;
     expected.details.pitching  = &expected_pitcher;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, pitcher_accolades_t_read_by_player( db, expected.player_id, &list ) );

     return NULL;
}

static char *remove_player__ShouldRemoveTheBattingDetailsFromTheDatabase_GivenAPlayerWithBattingDetails()
{
     player_s expected        = { 0 };
     batter_s expected_batter = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     expected.details.batting = &expected_batter;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     batter_s actual_batter = { 0 };

     actual_batter.player_id = expected.player_id;

     assertEquals( SQLITE_NOTFOUND, batters_t_read( db, &actual_batter ) );

     return NULL;
}

static char *remove_player__ShouldRemoveTheBattingStatsFromTheDatabase_GivenAPlayerWithBattingStats()
{
     player_s       expected                 = { 0 };
     batter_s       expected_batter          = { 0 };
     batter_stats_s expected_batter_stats[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     expected_batter_stats[0].player_id      = 1;
     expected_batter_stats[0].season         = 1;
     expected_batter_stats[0].season_phase   = sp_Regular;
     expected_batter_stats[0].games          = 134;
     expected_batter_stats[0].at_bats        = 543;
     expected_batter_stats[0].runs           = 101;
     expected_batter_stats[0].hits           = 220;
     expected_batter_stats[0].doubles        = 35;
     expected_batter_stats[0].triples        = 16;
     expected_batter_stats[0].home_runs      = 56;
     expected_batter_stats[0].runs_batted_in = 115;
     expected_batter_stats[0].walks          = 62;
     expected_batter_stats[0].strike_outs    = 131;
     expected_batter_stats[0].steals         = 45;
     expected_batter_stats[0].errors         = 5;

     expected_batter_stats[1].player_id      = 1;
     expected_batter_stats[1].season         = 2;
     expected_batter_stats[1].season_phase   = sp_Regular;
     expected_batter_stats[1].games          = 143;
     expected_batter_stats[1].at_bats        = 555;
     expected_batter_stats[1].runs           = 99;
     expected_batter_stats[1].hits           = 206;
     expected_batter_stats[1].doubles        = 22;
     expected_batter_stats[1].triples        = 11;
     expected_batter_stats[1].home_runs      = 44;
     expected_batter_stats[1].runs_batted_in = 109;
     expected_batter_stats[1].walks          = 58;
     expected_batter_stats[1].strike_outs    = 144;
     expected_batter_stats[1].steals         = 37;
     expected_batter_stats[1].errors         = 8;

     expected_batter_stats[2] = batter_stats_sentinel;

     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected_batter_stats[0] ) );
     assertEquals( SQLITE_OK, batter_stats_t_create( db, &expected_batter_stats[1] ) );

     expected_batter.stats    =  expected_batter_stats;
     expected.details.batting = &expected_batter;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, batter_stats_t_read_by_player( db, expected.player_id, &list ) );

     return NULL;
}

static char *remove_player__ShouldRemoveTheBattingAccoladesFromTheDatabase_GivenAPlayerWithBattingAccolades()
{
     player_s          expected                     = { 0 };
     batter_s          expected_batter              = { 0 };
     batter_accolade_s expected_batter_accolades[3] = { 0 };

     /**/    expected.player_id        = 1;
     strcpy( expected.first_name,       "FirstName1" );
     strcpy( expected.last_name,        "LastName1"  );
     strcpy( expected.first_phoenetic,  "FirstPho1"  );
     strcpy( expected.last_phoenetic,   "LastPho1"   );
     /**/    expected.skin_tone        = st_Light;
     /**/    expected.handedness       = hnd_Right;
     /**/    expected.player_type      = pt_Batter;
     /**/    expected.rookie_season    = 1;
     /**/    expected.longevity        = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_batter.player_id          = 1;
     expected_batter.power              = 6;
     expected_batter.hit_n_run          = 4;
     expected_batter.bunt               = 5;
     expected_batter.running            = 3;
     expected_batter.range              = 7;
     expected_batter.arm                = 8;
     expected_batter.primary_position   = pos_SecondBase;
     expected_batter.secondary_position = pos_ThirdBase;

     assertEquals( SQLITE_OK, batters_t_create( db, &expected_batter ) );

     expected_batter_accolades[0].player_id = 1;
     expected_batter_accolades[0].season    = 1;
     expected_batter_accolades[0].accolade  = bacc_Best_Batting_Average;

     expected_batter_accolades[1].player_id = 1;
     expected_batter_accolades[1].season    = 1;
     expected_batter_accolades[1].accolade  = bacc_Most_Home_Runs;

     expected_batter_accolades[2] = batter_accolade_sentinel;

     assertEquals( SQLITE_OK, batter_accolades_t_create( db, &expected_batter_accolades[0] ) );
     assertEquals( SQLITE_OK, batter_accolades_t_create( db, &expected_batter_accolades[1] ) );

     expected_batter.accolades =  expected_batter_accolades;
     expected.details.batting  = &expected_batter;

     assertEquals( SQLITE_OK, remove_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, batter_accolades_t_read_by_player( db, expected.player_id, &list ) );

     return NULL;
}

static void check_sqlite_error()
{
     if ( sqlite3_errcode( db ) != 0 )
     {
          printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
     }
}

static void run_all_tests()
{
     // get_player()
     run_test( get_player__ShouldReturnTheMatchingPlayerObject_GivenAPlayerId,                check_sqlite_error );
     run_test( get_player__ShoultReturnTheMatchingPlayerWithAccolades_GivenAPlayerId,         check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithPitchingDetails_GivenAPlayerId,   check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithPitchingStats_GivenAPlayerId,     check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithPitchingAccolades_GivenAPlayerId, check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithBattingDetails_GivenAPlayerId,    check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithBattingStats_GivenAPlayerId,      check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithBattingAccolades_GivenAPlayerId,  check_sqlite_error );

     // save_player()
     run_test( save_player__ShouldSaveThePlayerToTheDatabase_GivenAPlayerObject,                       check_sqlite_error );
     run_test( save_player__ShouldOnlyUpdatePhoeneticsIfThePlayerExists_GivenAPlayerObject,            check_sqlite_error );
     run_test( save_player__ShouldSavePlayerAccoladesToTheDatabase_GivenAPlayerWithAccolades,          check_sqlite_error );
     run_test( save_player__ShouldSavePitcherDetailsToTheDatabase_GivenAPlayerWithPitchingDetails,     check_sqlite_error );
     run_test( save_player__ShouldSavePitcherStatsToTheDatabase_GivenAPlayerWithPitchingStats,         check_sqlite_error );
     run_test( save_player__ShouldSavePitcherAccoladesToTheDatabase_GivenAPlayerWithPitchingAccolades, check_sqlite_error );
     run_test( save_player__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenAPitcher,           check_sqlite_error );
     run_test( save_player__ShouldNotUpdatePitcherRecordsIfTheyExist_GivenAPitcher,                    check_sqlite_error );
     run_test( save_player__ShouldSaveBatterDetailsToTheDatabase_GivenAPlayerWithBattingDetails,       check_sqlite_error );
     run_test( save_player__ShouldSaveBatterStatsToTheDatabase_GivenAPlayerWithBattingStats,           check_sqlite_error );
     run_test( save_player__ShouldSaveBatterAccoladesToTheDatabase_GivenAPlayerWithBattingAccolades,   check_sqlite_error );
     run_test( save_player__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenABatter,            check_sqlite_error );
     run_test( save_player__ShouldNotUpdateBatterRecordsIfTheyExist_GivenABatter,                      check_sqlite_error );

     // remove_player()
     run_test( remove_player__ShouldRemoveThePlayerFromTheDatabase_GivenAPlayerObject,                           check_sqlite_error );
     run_test( remove_player__ShouldRemoveThePlayerAccoladesFromTheDatabase_GivenAPlayerWithAccolades,           check_sqlite_error );
     run_test( remove_player__ShouldRemoveThePitchingDetailsFromTheDatabase_GivenAPlayerWithPitchingDetails,     check_sqlite_error );
     run_test( remove_player__ShouldRemoveThePitchingStatsFromTheDatabase_GivenAPlayerWithPitchingStats,         check_sqlite_error );
     run_test( remove_player__ShouldRemoveThePitchingAccoladesFromTheDatabase_GivenAPlayerWithPitchingAccolades, check_sqlite_error );
     run_test( remove_player__ShouldRemoveTheBattingDetailsFromTheDatabase_GivenAPlayerWithBattingDetails,       check_sqlite_error );
     run_test( remove_player__ShouldRemoveTheBattingStatsFromTheDatabase_GivenAPlayerWithBattingStats,           check_sqlite_error );
     run_test( remove_player__ShouldRemoveTheBattingAccoladesFromTheDatabase_GivenAPlayerWithBattingAccolades,   check_sqlite_error );
}

int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a db file name.\n" );

          return EXIT_FAILURE;
     }

     sqlite3_open( argv[1], &db );
     sqlite3_exec( db, "begin", NULL, NULL, NULL );


     run_all_tests();

     show_test_results();


     sqlite3_exec( db, "rollback", NULL, NULL, NULL );
     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
