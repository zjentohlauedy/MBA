#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static char *get_player__ShouldReturnTheMatchingPlayerObject_GivenAPlayerId()
{
     player_s expected = { 0 };

     /**/    expected.player_id       = 1;
     strcpy( expected.first_name,      "FirstName1" );
     strcpy( expected.last_name,       "LastName1"  );
     strcpy( expected.first_phonetic,  "FirstPho1"  );
     strcpy( expected.last_phonetic,   "LastPho1"   );
     /**/    expected.skin_tone       = st_Light;
     /**/    expected.handedness      = hnd_Right;
     /**/    expected.player_type     = pt_Pitcher;
     /**/    expected.rookie_season   = 1;
     /**/    expected.longevity       = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     player_s *actual = { 0 };

     actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( expected.player_id,      actual->player_id      );
     assertEqualsStr( expected.first_name,     actual->first_name     );
     assertEqualsStr( expected.last_name,      actual->last_name      );
     assertEqualsStr( expected.first_phonetic, actual->first_phonetic );
     assertEqualsStr( expected.last_phonetic,  actual->last_phonetic  );
     assertEqualsInt( expected.skin_tone,      actual->skin_tone      );
     assertEqualsInt( expected.handedness,     actual->handedness     );
     assertEqualsInt( expected.player_type,    actual->player_type    );
     assertEqualsInt( expected.rookie_season,  actual->rookie_season  );
     assertEqualsInt( expected.longevity,      actual->longevity      );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );

     return NULL;
}

static char *get_player__ShoultReturnTheMatchingPlayerWithAccolades_GivenAPlayerId()
{
     player_s          expected           = { 0 };
     player_accolade_s expected_accolade1 = { 0 };
     player_accolade_s expected_accolade2 = { 0 };

     /**/    expected.player_id       = 1;
     strcpy( expected.first_name,      "FirstName1" );
     strcpy( expected.last_name,       "LastName1"  );
     strcpy( expected.first_phonetic,  "FirstPho1"  );
     strcpy( expected.last_phonetic,   "LastPho1"   );
     /**/    expected.skin_tone       = st_Light;
     /**/    expected.handedness      = hnd_Right;
     /**/    expected.player_type     = pt_Pitcher;
     /**/    expected.rookie_season   = 1;
     /**/    expected.longevity       = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_accolade1.player_id = 1;
     expected_accolade1.season    = 1;
     expected_accolade1.accolade  = acc_All_Star;

     expected_accolade2.player_id = 1;
     expected_accolade2.season    = 2;
     expected_accolade2.accolade  = acc_All_Star;

     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected_accolade2 ) );

     player_s *actual = { 0 };

     actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     player_accolade_s *actual_accolade = actual->accolades;

     assertNotNull( actual_accolade );

     assertEqualsInt( expected_accolade1.player_id, actual_accolade[0].player_id );
     assertEqualsInt( expected_accolade1.season,    actual_accolade[0].season    );
     assertEqualsInt( expected_accolade1.accolade,  actual_accolade[0].accolade  );

     assertEqualsInt( expected_accolade2.player_id, actual_accolade[1].player_id );
     assertEqualsInt( expected_accolade2.season,    actual_accolade[1].season    );
     assertEqualsInt( expected_accolade2.accolade,  actual_accolade[1].accolade  );

     player_accolade_s sentinel = PLAYER_ACCOLADE_SENTINEL;

     assertEqualsInt( sentinel.player_id, actual_accolade[2].player_id );
     assertEqualsInt( sentinel.season,    actual_accolade[2].season    );
     assertEqualsInt( sentinel.accolade,  actual_accolade[2].accolade  );

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

     /**/    expected.player_id       = 1;
     strcpy( expected.first_name,      "FirstName1" );
     strcpy( expected.last_name,       "LastName1"  );
     strcpy( expected.first_phonetic,  "FirstPho1"  );
     strcpy( expected.last_phonetic,   "LastPho1"   );
     /**/    expected.skin_tone       = st_Light;
     /**/    expected.handedness      = hnd_Right;
     /**/    expected.player_type     = pt_Pitcher;
     /**/    expected.rookie_season   = 1;
     /**/    expected.longevity       = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     player_s *actual = { 0 };

     actual = get_player( db, expected.player_id );

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
     player_s        expected                = { 0 };
     pitcher_s       expected_pitcher        = { 0 };
     pitcher_stats_s expected_pitcher_stats1 = { 0 };
     pitcher_stats_s expected_pitcher_stats2 = { 0 };

     /**/    expected.player_id       = 1;
     strcpy( expected.first_name,      "FirstName1" );
     strcpy( expected.last_name,       "LastName1"  );
     strcpy( expected.first_phonetic,  "FirstPho1"  );
     strcpy( expected.last_phonetic,   "LastPho1"   );
     /**/    expected.skin_tone       = st_Light;
     /**/    expected.handedness      = hnd_Right;
     /**/    expected.player_type     = pt_Pitcher;
     /**/    expected.rookie_season   = 1;
     /**/    expected.longevity       = 5;

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     expected_pitcher.player_id = 1;
     expected_pitcher.speed     = 5;
     expected_pitcher.control   = 7;
     expected_pitcher.bunt      = 3;
     expected_pitcher.fatigue   = 8;

     assertEquals( SQLITE_OK, pitchers_t_create( db, &expected_pitcher ) );

     expected_pitcher_stats1.player_id    = 1;
     expected_pitcher_stats1.season       = 1;
     expected_pitcher_stats1.season_phase = sp_Regular;
     expected_pitcher_stats1.wins         = 12;
     expected_pitcher_stats1.losses       = 15;
     expected_pitcher_stats1.games        = 54;
     expected_pitcher_stats1.saves        = 11;
     expected_pitcher_stats1.innings      = 321.2;
     expected_pitcher_stats1.hits         = 165;
     expected_pitcher_stats1.earned_runs  = 23;
     expected_pitcher_stats1.home_runs    = 14;
     expected_pitcher_stats1.walks        = 25;
     expected_pitcher_stats1.strike_outs  = 234;

     expected_pitcher_stats2.player_id    = 1;
     expected_pitcher_stats2.season       = 1;
     expected_pitcher_stats2.season_phase = sp_Playoff;
     expected_pitcher_stats2.wins         = 2;
     expected_pitcher_stats2.losses       = 1;
     expected_pitcher_stats2.games        = 3;
     expected_pitcher_stats2.saves        = 0;
     expected_pitcher_stats2.innings      = 25.2;
     expected_pitcher_stats2.hits         = 20;
     expected_pitcher_stats2.earned_runs  = 5;
     expected_pitcher_stats2.home_runs    = 1;
     expected_pitcher_stats2.walks        = 3;
     expected_pitcher_stats2.strike_outs  = 25;

     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, &expected_pitcher_stats1 ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_create( db, &expected_pitcher_stats2 ) );

     player_s *actual = { 0 };

     actual = get_player( db, expected.player_id );

     assertNotNull( actual );

     assertEqualsInt( pt_Pitcher, actual->player_type );

     pitcher_s *actual_pitcher = actual->details.pitching;

     assertNotNull( actual_pitcher );

     pitcher_stats_s *actual_pitcher_stats = actual_pitcher->stats;

     assertNotNull( actual_pitcher_stats );

     assertEqualsInt( expected_pitcher_stats1.player_id,    actual_pitcher_stats[0].player_id    );
     assertEqualsInt( expected_pitcher_stats1.season,       actual_pitcher_stats[0].season       );
     assertEqualsInt( expected_pitcher_stats1.season_phase, actual_pitcher_stats[0].season_phase );
     assertEqualsInt( expected_pitcher_stats1.wins,         actual_pitcher_stats[0].wins         );
     assertEqualsInt( expected_pitcher_stats1.losses,       actual_pitcher_stats[0].losses       );
     assertEqualsInt( expected_pitcher_stats1.games,        actual_pitcher_stats[0].games        );
     assertEqualsInt( expected_pitcher_stats1.saves,        actual_pitcher_stats[0].saves        );
     assertEqualsDbl( expected_pitcher_stats1.innings,      actual_pitcher_stats[0].innings      );
     assertEqualsInt( expected_pitcher_stats1.hits,         actual_pitcher_stats[0].hits         );
     assertEqualsInt( expected_pitcher_stats1.earned_runs,  actual_pitcher_stats[0].earned_runs  );
     assertEqualsInt( expected_pitcher_stats1.home_runs,    actual_pitcher_stats[0].home_runs    );
     assertEqualsInt( expected_pitcher_stats1.walks,        actual_pitcher_stats[0].walks        );
     assertEqualsInt( expected_pitcher_stats1.strike_outs,  actual_pitcher_stats[0].strike_outs  );

     assertEqualsInt( expected_pitcher_stats2.player_id,    actual_pitcher_stats[1].player_id    );
     assertEqualsInt( expected_pitcher_stats2.season,       actual_pitcher_stats[1].season       );
     assertEqualsInt( expected_pitcher_stats2.season_phase, actual_pitcher_stats[1].season_phase );
     assertEqualsInt( expected_pitcher_stats2.wins,         actual_pitcher_stats[1].wins         );
     assertEqualsInt( expected_pitcher_stats2.losses,       actual_pitcher_stats[1].losses       );
     assertEqualsInt( expected_pitcher_stats2.games,        actual_pitcher_stats[1].games        );
     assertEqualsInt( expected_pitcher_stats2.saves,        actual_pitcher_stats[1].saves        );
     assertEqualsDbl( expected_pitcher_stats2.innings,      actual_pitcher_stats[1].innings      );
     assertEqualsInt( expected_pitcher_stats2.hits,         actual_pitcher_stats[1].hits         );
     assertEqualsInt( expected_pitcher_stats2.earned_runs,  actual_pitcher_stats[1].earned_runs  );
     assertEqualsInt( expected_pitcher_stats2.home_runs,    actual_pitcher_stats[1].home_runs    );
     assertEqualsInt( expected_pitcher_stats2.walks,        actual_pitcher_stats[1].walks        );
     assertEqualsInt( expected_pitcher_stats2.strike_outs,  actual_pitcher_stats[1].strike_outs  );

     pitcher_stats_s sentinel = PITCHER_STATS_SENTINEL;

     assertEqualsInt( sentinel.player_id,    actual_pitcher_stats[2].player_id    );
     assertEqualsInt( sentinel.season,       actual_pitcher_stats[2].season       );
     assertEqualsInt( sentinel.season_phase, actual_pitcher_stats[2].season_phase );
     assertEqualsInt( sentinel.wins,         actual_pitcher_stats[2].wins         );
     assertEqualsInt( sentinel.losses,       actual_pitcher_stats[2].losses       );
     assertEqualsInt( sentinel.games,        actual_pitcher_stats[2].games        );
     assertEqualsInt( sentinel.saves,        actual_pitcher_stats[2].saves        );
     assertEqualsDbl( sentinel.innings,      actual_pitcher_stats[2].innings      );
     assertEqualsInt( sentinel.hits,         actual_pitcher_stats[2].hits         );
     assertEqualsInt( sentinel.earned_runs,  actual_pitcher_stats[2].earned_runs  );
     assertEqualsInt( sentinel.home_runs,    actual_pitcher_stats[2].home_runs    );
     assertEqualsInt( sentinel.walks,        actual_pitcher_stats[2].walks        );
     assertEqualsInt( sentinel.strike_outs,  actual_pitcher_stats[2].strike_outs  );

     free_player( actual );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, pitchers_t_delete( db, &expected_pitcher ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, &expected_pitcher_stats1 ) );
     assertEquals( SQLITE_OK, pitcher_stats_t_delete( db, &expected_pitcher_stats2 ) );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithBattingDetails_GivenAPlayerId()
{
     player_s expected        = { 0 };
     batter_s expected_batter = { 0 };

     /**/    expected.player_id       = 1;
     strcpy( expected.first_name,      "FirstName1" );
     strcpy( expected.last_name,       "LastName1"  );
     strcpy( expected.first_phonetic,  "FirstPho1"  );
     strcpy( expected.last_phonetic,   "LastPho1"   );
     /**/    expected.skin_tone       = st_Light;
     /**/    expected.handedness      = hnd_Right;
     /**/    expected.player_type     = pt_Batter;
     /**/    expected.rookie_season   = 1;
     /**/    expected.longevity       = 5;

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

     player_s *actual = { 0 };

     actual = get_player( db, expected.player_id );

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
//   run_test( get_player__ShouldReturnTheMatchingPlayerWithPitchingAccolades_GivenAPlayerId, check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithBattingDetails_GivenAPlayerId,    check_sqlite_error );
//   run_test( get_player__ShouldReturnTheMatchingPlayerWithBattingStats_GivenAPlayerId,      check_sqlite_error );
//   run_test( get_player__ShouldReturnTheMatchingPlayerWithBattingAccolades_GivenAPlayerId,  check_sqlite_error );
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
