#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "league.h"
#include "unit_test.h"


static league_division_s league_division_sentinel = LEAGUE_DIVISION_SENTINEL;
static league_stats_s    league_stats_sentinel    = LEAGUE_STATS_SENTINEL;
static league_accolade_s league_accolade_sentinel = LEAGUE_ACCOLADE_SENTINEL;

static sqlite3 *db;
static char    *result;


static char *get_league__ShouldReturnTheMatchingLeagueObject_GivenALeagueId()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     league_s *actual = get_league( db, expected.league_id );

     assertNotNull( actual );

     assertEqualsInt( expected.league_id, actual->league_id );
     assertEqualsStr( expected.name,      actual->name      );

     free_league( actual );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );

     return NULL;
}

static char *get_league__ShouldReturnTheMatchingLeagueWithDivisions_GivenALeagueId()
{
     league_s expected = { 0 };
     league_division_s expected_league_divisions1 = { 0 };
     league_division_s expected_league_divisions2 = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     expected_league_divisions1.league_id   = 1;
     expected_league_divisions1.division_id = 1;

     expected_league_divisions2.league_id   = 1;
     expected_league_divisions2.division_id = 2;

     assertEquals( SQLITE_OK, league_divisions_t_create( db, &expected_league_divisions1 ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &expected_league_divisions2 ) );

     league_s *actual = get_league( db, expected.league_id );

     league_division_s *actual_league_divisions = actual->divisions;

     assertNotNull( actual_league_divisions );

     assertEquals( expected_league_divisions1.league_id,   actual_league_divisions[0].league_id   );
     assertEquals( expected_league_divisions1.division_id, actual_league_divisions[0].division_id );

     assertEquals( expected_league_divisions2.league_id,   actual_league_divisions[1].league_id   );
     assertEquals( expected_league_divisions2.division_id, actual_league_divisions[1].division_id );

     assertEquals( league_division_sentinel.league_id,   actual_league_divisions[2].league_id   );
     assertEquals( league_division_sentinel.division_id, actual_league_divisions[2].division_id );

     free_league( actual );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions1 ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions2 ) );

     return NULL;
}

static char *get_league__ShouldReturnTheMatchingLeagueWithStats_GivenALeagueId()
{
     league_s       expected        = { 0 };
     league_stats_s expected_stats1 = { 0 };
     league_stats_s expected_stats2 = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     expected_stats1.league_id    = 1;
     expected_stats1.season       = 1;
     expected_stats1.season_phase = sp_Regular;
     expected_stats1.wins         = 522;
     expected_stats1.losses       = 502;
     expected_stats1.home_wins    = 268;
     expected_stats1.home_losses  = 244;
     expected_stats1.road_wins    = 254;
     expected_stats1.road_losses  = 258;
     expected_stats1.runs_scored  = 3482;
     expected_stats1.runs_allowed = 3246;

     expected_stats2.league_id    = 1;
     expected_stats2.season       = 2;
     expected_stats2.season_phase = sp_Regular;
     expected_stats2.wins         = 532;
     expected_stats2.losses       = 492;
     expected_stats2.home_wins    = 288;
     expected_stats2.home_losses  = 224;
     expected_stats2.road_wins    = 244;
     expected_stats2.road_losses  = 268;
     expected_stats2.runs_scored  = 3552;
     expected_stats2.runs_allowed = 3196;

     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected_stats1 ) );
     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected_stats2 ) );

     league_s *actual = get_league( db, expected.league_id );

     league_stats_s *actual_league_stats = actual->stats;

     assertNotNull( actual_league_stats );

     assertEquals( expected_stats1.league_id,    actual_league_stats[0].league_id    );
     assertEquals( expected_stats1.season,       actual_league_stats[0].season       );
     assertEquals( expected_stats1.season_phase, actual_league_stats[0].season_phase );
     assertEquals( expected_stats1.wins,         actual_league_stats[0].wins         );
     assertEquals( expected_stats1.losses,       actual_league_stats[0].losses       );
     assertEquals( expected_stats1.home_wins,    actual_league_stats[0].home_wins    );
     assertEquals( expected_stats1.home_losses,  actual_league_stats[0].home_losses  );
     assertEquals( expected_stats1.road_wins,    actual_league_stats[0].road_wins    );
     assertEquals( expected_stats1.road_losses,  actual_league_stats[0].road_losses  );
     assertEquals( expected_stats1.runs_scored,  actual_league_stats[0].runs_scored  );
     assertEquals( expected_stats1.runs_allowed, actual_league_stats[0].runs_allowed );

     assertEquals( expected_stats2.league_id,    actual_league_stats[1].league_id    );
     assertEquals( expected_stats2.season,       actual_league_stats[1].season       );
     assertEquals( expected_stats2.season_phase, actual_league_stats[1].season_phase );
     assertEquals( expected_stats2.wins,         actual_league_stats[1].wins         );
     assertEquals( expected_stats2.losses,       actual_league_stats[1].losses       );
     assertEquals( expected_stats2.home_wins,    actual_league_stats[1].home_wins    );
     assertEquals( expected_stats2.home_losses,  actual_league_stats[1].home_losses  );
     assertEquals( expected_stats2.road_wins,    actual_league_stats[1].road_wins    );
     assertEquals( expected_stats2.road_losses,  actual_league_stats[1].road_losses  );
     assertEquals( expected_stats2.runs_scored,  actual_league_stats[1].runs_scored  );
     assertEquals( expected_stats2.runs_allowed, actual_league_stats[1].runs_allowed );

     assertEquals( league_stats_sentinel.league_id,    actual_league_stats[2].league_id    );
     assertEquals( league_stats_sentinel.season,       actual_league_stats[2].season       );
     assertEquals( league_stats_sentinel.season_phase, actual_league_stats[2].season_phase );
     assertEquals( league_stats_sentinel.wins,         actual_league_stats[2].wins         );
     assertEquals( league_stats_sentinel.losses,       actual_league_stats[2].losses       );
     assertEquals( league_stats_sentinel.home_wins,    actual_league_stats[2].home_wins    );
     assertEquals( league_stats_sentinel.home_losses,  actual_league_stats[2].home_losses  );
     assertEquals( league_stats_sentinel.road_wins,    actual_league_stats[2].road_wins    );
     assertEquals( league_stats_sentinel.road_losses,  actual_league_stats[2].road_losses  );
     assertEquals( league_stats_sentinel.runs_scored,  actual_league_stats[2].runs_scored  );
     assertEquals( league_stats_sentinel.runs_allowed, actual_league_stats[2].runs_allowed );

     free_league( actual );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected_stats1 ) );
     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected_stats2 ) );

     return NULL;
}

static char *get_league__ShouldReturnTheMatchingLeagueWithAccolades_GivenALeagueId()
{
     league_s          expected           = { 0 };
     league_accolade_s expected_accolade1 = { 0 };
     league_accolade_s expected_accolade2 = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     expected_accolade1.league_id = 1;
     expected_accolade1.season    = 1;
     expected_accolade1.accolade  = lacc_World_Title;

     expected_accolade2.league_id = 1;
     expected_accolade2.season    = 2;
     expected_accolade2.accolade  = lacc_Best_Record;

     assertEquals( SQLITE_OK, league_accolades_t_create( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, league_accolades_t_create( db, &expected_accolade2 ) );

     league_s *actual = get_league( db, expected.league_id );

     league_accolade_s *actual_accolades = actual->accolades;

     assertNotNull( actual_accolades );

     assertEquals( expected_accolade1.league_id, actual_accolades[0].league_id );
     assertEquals( expected_accolade1.season,    actual_accolades[0].season    );
     assertEquals( expected_accolade1.accolade,  actual_accolades[0].accolade  );

     assertEquals( expected_accolade2.league_id, actual_accolades[1].league_id );
     assertEquals( expected_accolade2.season,    actual_accolades[1].season    );
     assertEquals( expected_accolade2.accolade,  actual_accolades[1].accolade  );

     assertEquals( league_accolade_sentinel.league_id, actual_accolades[2].league_id );
     assertEquals( league_accolade_sentinel.season,    actual_accolades[2].season    );
     assertEquals( league_accolade_sentinel.accolade,  actual_accolades[2].accolade  );

     free_league( actual );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected_accolade2 ) );

     return NULL;
}

static char *save_league__ShouldPersistTheLeagueInTheDatabase_GivenALeagueObject()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, save_league( db, &expected ) );

     league_s actual = { 0 };

     actual.league_id = expected.league_id;

     assertEquals( SQLITE_OK, leagues_t_read( db, &actual ) );

     assertEqualsInt( expected.league_id, actual.league_id );
     assertEqualsStr( expected.name,      actual.name      );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );

     return NULL;
}

static char *save_league__ShouldPersistTheLeagueDivisionsInTheDatabase_GivenALeagueWithLeagueDivisions()
{
     league_s          expected                     = { 0 };
     league_division_s expected_league_divisions[3] = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     expected_league_divisions[0].league_id   = 1;
     expected_league_divisions[0].division_id = 1;

     expected_league_divisions[1].league_id   = 1;
     expected_league_divisions[1].division_id = 2;

     expected_league_divisions[2] = league_division_sentinel;

     expected.divisions = expected_league_divisions;

     assertEquals( SQLITE_OK, save_league( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_divisions_t_read_by_league( db, expected.league_id, &list ) );

     assertEquals( 2, list.count );

     league_division_s *actual_league_divisions = list.data;

     assertNotNull( actual_league_divisions );

     assertEquals( expected_league_divisions[0].league_id,   actual_league_divisions[0].league_id   );
     assertEquals( expected_league_divisions[0].division_id, actual_league_divisions[0].division_id );

     assertEquals( expected_league_divisions[1].league_id,   actual_league_divisions[1].league_id   );
     assertEquals( expected_league_divisions[1].division_id, actual_league_divisions[1].division_id );

     free( actual_league_divisions );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions[0] ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions[1] ) );

     return NULL;
}

static char *save_league__ShouldPersistTheLeagueStatsInTheDatabase_GivenALeagueWithLeagueStats()
{
     league_s       expected          = { 0 };
     league_stats_s expected_stats[3] = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     expected_stats[0].league_id    = 1;
     expected_stats[0].season       = 1;
     expected_stats[0].season_phase = sp_Regular;
     expected_stats[0].wins         = 522;
     expected_stats[0].losses       = 502;
     expected_stats[0].home_wins    = 268;
     expected_stats[0].home_losses  = 244;
     expected_stats[0].road_wins    = 254;
     expected_stats[0].road_losses  = 258;
     expected_stats[0].runs_scored  = 3482;
     expected_stats[0].runs_allowed = 3246;

     expected_stats[1].league_id    = 1;
     expected_stats[1].season       = 2;
     expected_stats[1].season_phase = sp_Regular;
     expected_stats[1].wins         = 532;
     expected_stats[1].losses       = 492;
     expected_stats[1].home_wins    = 288;
     expected_stats[1].home_losses  = 224;
     expected_stats[1].road_wins    = 244;
     expected_stats[1].road_losses  = 268;
     expected_stats[1].runs_scored  = 3552;
     expected_stats[1].runs_allowed = 3196;

     expected_stats[2] = league_stats_sentinel;

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, save_league( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_stats_t_read_by_league( db, expected.league_id, &list ) );

     assertEquals( 2, list.count );

     league_stats_s *actual_league_stats = list.data;

     assertNotNull( actual_league_stats );

     assertEquals( expected_stats[0].league_id,    actual_league_stats[0].league_id    );
     assertEquals( expected_stats[0].season,       actual_league_stats[0].season       );
     assertEquals( expected_stats[0].season_phase, actual_league_stats[0].season_phase );
     assertEquals( expected_stats[0].wins,         actual_league_stats[0].wins         );
     assertEquals( expected_stats[0].losses,       actual_league_stats[0].losses       );
     assertEquals( expected_stats[0].home_wins,    actual_league_stats[0].home_wins    );
     assertEquals( expected_stats[0].home_losses,  actual_league_stats[0].home_losses  );
     assertEquals( expected_stats[0].road_wins,    actual_league_stats[0].road_wins    );
     assertEquals( expected_stats[0].road_losses,  actual_league_stats[0].road_losses  );
     assertEquals( expected_stats[0].runs_scored,  actual_league_stats[0].runs_scored  );
     assertEquals( expected_stats[0].runs_allowed, actual_league_stats[0].runs_allowed );

     assertEquals( expected_stats[1].league_id,    actual_league_stats[1].league_id    );
     assertEquals( expected_stats[1].season,       actual_league_stats[1].season       );
     assertEquals( expected_stats[1].season_phase, actual_league_stats[1].season_phase );
     assertEquals( expected_stats[1].wins,         actual_league_stats[1].wins         );
     assertEquals( expected_stats[1].losses,       actual_league_stats[1].losses       );
     assertEquals( expected_stats[1].home_wins,    actual_league_stats[1].home_wins    );
     assertEquals( expected_stats[1].home_losses,  actual_league_stats[1].home_losses  );
     assertEquals( expected_stats[1].road_wins,    actual_league_stats[1].road_wins    );
     assertEquals( expected_stats[1].road_losses,  actual_league_stats[1].road_losses  );
     assertEquals( expected_stats[1].runs_scored,  actual_league_stats[1].runs_scored  );
     assertEquals( expected_stats[1].runs_allowed, actual_league_stats[1].runs_allowed );

     free( actual_league_stats );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected_stats[1] ) );

     return NULL;
}

static char *save_league__ShouldPersistTheAccoladesInTheDatabase_GivenALeagueWithAccolades()
{
     league_s          expected              = { 0 };
     league_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     expected_accolades[0].league_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = lacc_World_Title;

     expected_accolades[1].league_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = lacc_Best_Record;

     expected_accolades[2] = league_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_league( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_accolades_t_read_by_league( db, expected.league_id, &list ) );

     assertEquals( 2, list.count );

     league_accolade_s *actual_accolades = list.data;

     assertNotNull( actual_accolades );

     assertEquals( expected_accolades[0].league_id, actual_accolades[0].league_id );
     assertEquals( expected_accolades[0].season,    actual_accolades[0].season    );
     assertEquals( expected_accolades[0].accolade,  actual_accolades[0].accolade  );

     assertEquals( expected_accolades[1].league_id, actual_accolades[1].league_id );
     assertEquals( expected_accolades[1].season,    actual_accolades[1].season    );
     assertEquals( expected_accolades[1].accolade,  actual_accolades[1].accolade  );

     free( actual_accolades );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *save_league__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenALeague()
{
     league_s          expected                     = { 0 };
     league_division_s expected_league_divisions[3] = { 0 };
     league_stats_s    expected_stats[3]            = { 0 };
     league_accolade_s expected_accolades[3]        = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     expected_league_divisions[0].league_id   = 1;
     expected_league_divisions[0].division_id = 1;

     expected_league_divisions[1].league_id   = 1;
     expected_league_divisions[1].division_id = 2;

     expected_league_divisions[2] = league_division_sentinel;

     expected_stats[0].league_id    = 1;
     expected_stats[0].season       = 1;
     expected_stats[0].season_phase = sp_Regular;
     expected_stats[0].wins         = 522;
     expected_stats[0].losses       = 502;
     expected_stats[0].home_wins    = 268;
     expected_stats[0].home_losses  = 244;
     expected_stats[0].road_wins    = 254;
     expected_stats[0].road_losses  = 258;
     expected_stats[0].runs_scored  = 3482;
     expected_stats[0].runs_allowed = 3246;

     expected_stats[1].league_id    = 1;
     expected_stats[1].season       = 2;
     expected_stats[1].season_phase = sp_Regular;
     expected_stats[1].wins         = 532;
     expected_stats[1].losses       = 492;
     expected_stats[1].home_wins    = 288;
     expected_stats[1].home_losses  = 224;
     expected_stats[1].road_wins    = 244;
     expected_stats[1].road_losses  = 268;
     expected_stats[1].runs_scored  = 3552;
     expected_stats[1].runs_allowed = 3196;

     expected_stats[2] = league_stats_sentinel;

     expected_accolades[0].league_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = lacc_World_Title;

     expected_accolades[1].league_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = lacc_Best_Record;

     expected_accolades[2] = league_accolade_sentinel;

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &expected_league_divisions[0] ) );
     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, league_accolades_t_create( db, &expected_accolades[0] ) );

     strcpy( expected.name, "LeagueName2" );

     expected_stats[0].wins   = 520;
     expected_stats[0].losses = 504;

     expected.divisions = expected_league_divisions;
     expected.stats     = expected_stats;
     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_league( db, &expected ) );

     league_s actual = { 0 };

     actual.league_id = expected.league_id;

     assertEquals( SQLITE_OK, leagues_t_read( db, &actual ) );

     assertEqualsInt( expected.league_id, actual.league_id );
     assertEqualsStr( expected.name,      actual.name      );

     data_list_s league_divisions_list = { 0 };

     assertEquals( SQLITE_OK, league_divisions_t_read_by_league( db, expected.league_id, &league_divisions_list ) );

     assertEquals( 2, league_divisions_list.count );

     league_division_s *actual_league_divisions = league_divisions_list.data;

     assertNotNull( actual_league_divisions );

     assertEquals( expected_league_divisions[0].league_id,   actual_league_divisions[0].league_id   );
     assertEquals( expected_league_divisions[0].division_id, actual_league_divisions[0].division_id );

     assertEquals( expected_league_divisions[1].league_id,   actual_league_divisions[1].league_id   );
     assertEquals( expected_league_divisions[1].division_id, actual_league_divisions[1].division_id );

     data_list_s league_stats_list = { 0 };

     assertEquals( SQLITE_OK, league_stats_t_read_by_league( db, expected.league_id, &league_stats_list ) );

     assertEquals( 2, league_stats_list.count );

     league_stats_s *actual_league_stats = league_stats_list.data;

     assertNotNull( actual_league_stats );

     assertEquals( expected_stats[0].league_id,    actual_league_stats[0].league_id    );
     assertEquals( expected_stats[0].season,       actual_league_stats[0].season       );
     assertEquals( expected_stats[0].season_phase, actual_league_stats[0].season_phase );
     assertEquals( expected_stats[0].wins,         actual_league_stats[0].wins         );
     assertEquals( expected_stats[0].losses,       actual_league_stats[0].losses       );
     assertEquals( expected_stats[0].home_wins,    actual_league_stats[0].home_wins    );
     assertEquals( expected_stats[0].home_losses,  actual_league_stats[0].home_losses  );
     assertEquals( expected_stats[0].road_wins,    actual_league_stats[0].road_wins    );
     assertEquals( expected_stats[0].road_losses,  actual_league_stats[0].road_losses  );
     assertEquals( expected_stats[0].runs_scored,  actual_league_stats[0].runs_scored  );
     assertEquals( expected_stats[0].runs_allowed, actual_league_stats[0].runs_allowed );

     assertEquals( expected_stats[1].league_id,    actual_league_stats[1].league_id    );
     assertEquals( expected_stats[1].season,       actual_league_stats[1].season       );
     assertEquals( expected_stats[1].season_phase, actual_league_stats[1].season_phase );
     assertEquals( expected_stats[1].wins,         actual_league_stats[1].wins         );
     assertEquals( expected_stats[1].losses,       actual_league_stats[1].losses       );
     assertEquals( expected_stats[1].home_wins,    actual_league_stats[1].home_wins    );
     assertEquals( expected_stats[1].home_losses,  actual_league_stats[1].home_losses  );
     assertEquals( expected_stats[1].road_wins,    actual_league_stats[1].road_wins    );
     assertEquals( expected_stats[1].road_losses,  actual_league_stats[1].road_losses  );
     assertEquals( expected_stats[1].runs_scored,  actual_league_stats[1].runs_scored  );
     assertEquals( expected_stats[1].runs_allowed, actual_league_stats[1].runs_allowed );

     data_list_s accolades_list = { 0 };

     assertEquals( SQLITE_OK, league_accolades_t_read_by_league( db, expected.league_id, &accolades_list ) );

     assertEquals( 2, accolades_list.count );

     league_accolade_s *actual_accolades = accolades_list.data;

     assertNotNull( actual_accolades );

     assertEquals( expected_accolades[0].league_id, actual_accolades[0].league_id );
     assertEquals( expected_accolades[0].season,    actual_accolades[0].season    );
     assertEquals( expected_accolades[0].accolade,  actual_accolades[0].accolade  );

     assertEquals( expected_accolades[1].league_id, actual_accolades[1].league_id );
     assertEquals( expected_accolades[1].season,    actual_accolades[1].season    );
     assertEquals( expected_accolades[1].accolade,  actual_accolades[1].accolade  );

     free( actual_league_divisions );
     free( actual_league_stats );
     free( actual_accolades );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions[0] ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions[1] ) );
     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, league_stats_t_delete( db, &expected_stats[1] ) );
     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, league_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *remove_league__ShouldRemoveTheLeagueFromTheDatabase_GivenALeagueObject()
{
     league_s expected = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     assertEquals( SQLITE_OK, remove_league( db, &expected ) );

     league_s actual = { 0 };

     actual.league_id = expected.league_id;

     assertEquals( SQLITE_NOTFOUND, leagues_t_read( db, &actual ) );

     return NULL;
}

static char *remove_league__ShouldRemoveTheLeagueDivisionsFromTheDatabase_GivenALeagueWithLeagueDivisions()
{
     league_s          expected                     = { 0 };
     league_division_s expected_league_divisions[3] = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     expected_league_divisions[0].league_id   = 1;
     expected_league_divisions[0].division_id = 1;

     expected_league_divisions[1].league_id   = 1;
     expected_league_divisions[1].division_id = 2;

     expected_league_divisions[2] = league_division_sentinel;

     assertEquals( SQLITE_OK, league_divisions_t_create( db, &expected_league_divisions[0] ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &expected_league_divisions[1] ) );

     expected.divisions = expected_league_divisions;

     assertEquals( SQLITE_OK, remove_league( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, league_divisions_t_read_by_league( db, expected.league_id, &list ) );

     return NULL;
}

static char *remove_league__ShouldRemoveTheLeagueStatsFromTheDatabase_GivenALeagueWithLeagueStats()
{
     league_s       expected          = { 0 };
     league_stats_s expected_stats[3] = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     expected_stats[0].league_id    = 1;
     expected_stats[0].season       = 1;
     expected_stats[0].season_phase = sp_Regular;
     expected_stats[0].wins         = 522;
     expected_stats[0].losses       = 502;
     expected_stats[0].home_wins    = 268;
     expected_stats[0].home_losses  = 244;
     expected_stats[0].road_wins    = 254;
     expected_stats[0].road_losses  = 258;
     expected_stats[0].runs_scored  = 3482;
     expected_stats[0].runs_allowed = 3246;

     expected_stats[1].league_id    = 1;
     expected_stats[1].season       = 2;
     expected_stats[1].season_phase = sp_Regular;
     expected_stats[1].wins         = 532;
     expected_stats[1].losses       = 492;
     expected_stats[1].home_wins    = 288;
     expected_stats[1].home_losses  = 224;
     expected_stats[1].road_wins    = 244;
     expected_stats[1].road_losses  = 268;
     expected_stats[1].runs_scored  = 3552;
     expected_stats[1].runs_allowed = 3196;

     expected_stats[2] = league_stats_sentinel;

     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, league_stats_t_create( db, &expected_stats[1] ) );

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, remove_league( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, league_stats_t_read_by_league( db, expected.league_id, &list ) );

     return NULL;
}

static char *remove_league__ShouldRemoveTheAccoladesFromTheDatabase_GivenALeagueWithAccolades()
{
     league_s          expected              = { 0 };
     league_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.league_id = 1;
     strcpy( expected.name,      "LeagueName1" );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected ) );

     expected_accolades[0].league_id = 1;
     expected_accolades[0].season    = 1;
     expected_accolades[0].accolade  = lacc_World_Title;

     expected_accolades[1].league_id = 1;
     expected_accolades[1].season    = 2;
     expected_accolades[1].accolade  = lacc_Best_Record;

     expected_accolades[2] = league_accolade_sentinel;

     assertEquals( SQLITE_OK, league_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, league_accolades_t_create( db, &expected_accolades[1] ) );

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, remove_league( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, league_accolades_t_read_by_league( db, expected.league_id, &list ) );

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
     // get_league()
     run_test( get_league__ShouldReturnTheMatchingLeagueObject_GivenALeagueId,        check_sqlite_error );
     run_test( get_league__ShouldReturnTheMatchingLeagueWithDivisions_GivenALeagueId, check_sqlite_error );
     run_test( get_league__ShouldReturnTheMatchingLeagueWithStats_GivenALeagueId,     check_sqlite_error );
     run_test( get_league__ShouldReturnTheMatchingLeagueWithAccolades_GivenALeagueId, check_sqlite_error );

     // save_league()
     run_test( save_league__ShouldPersistTheLeagueInTheDatabase_GivenALeagueObject,                       check_sqlite_error );
     run_test( save_league__ShouldPersistTheLeagueDivisionsInTheDatabase_GivenALeagueWithLeagueDivisions, check_sqlite_error );
     run_test( save_league__ShouldPersistTheLeagueStatsInTheDatabase_GivenALeagueWithLeagueStats,         check_sqlite_error );
     run_test( save_league__ShouldPersistTheAccoladesInTheDatabase_GivenALeagueWithAccolades,             check_sqlite_error );
     run_test( save_league__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenALeague,               check_sqlite_error );

     // remove_league()
     run_test( remove_league__ShouldRemoveTheLeagueFromTheDatabase_GivenALeagueObject,                       check_sqlite_error );
     run_test( remove_league__ShouldRemoveTheLeagueDivisionsFromTheDatabase_GivenALeagueWithLeagueDivisions, check_sqlite_error );
     run_test( remove_league__ShouldRemoveTheLeagueStatsFromTheDatabase_GivenALeagueWithLeagueStats,         check_sqlite_error );
     run_test( remove_league__ShouldRemoveTheAccoladesFromTheDatabase_GivenALeagueWithAccolades,             check_sqlite_error );
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
