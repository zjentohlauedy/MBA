#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "division.h"
#include "unit_test.h"


static division_team_s     division_team_sentinel     = DIVISION_TEAM_SENTINEL;
static division_stats_s    division_stats_sentinel    = DIVISION_STATS_SENTINEL;
static division_accolade_s division_accolade_sentinel = DIVISION_ACCOLADE_SENTINEL;

static sqlite3 *db;
static char    *result;


static char *get_division__ShouldReturnTheMatchingDivisionObject_GivenADivisionId()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );

     division_s *actual = get_division( db, expected.division_id );

     assertEqualsInt( expected.division_id, actual->division_id );
     assertEqualsStr( expected.name,        actual->name        );

     free_division( actual );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );

     return NULL;
}

static char *get_division__ShouldReturnTheMatchingDivisionWithTeams_GivenADivisionId()
{
     division_s      expected                = { 0 };
     division_team_s expected_division_team1 = { 0 };
     division_team_s expected_division_team2 = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );

     expected_division_team1.division_id = 1;
     expected_division_team1.team_id     = 2;

     expected_division_team2.division_id = 1;
     expected_division_team2.team_id     = 5;

     assertEquals( SQLITE_OK, division_teams_t_create( db, &expected_division_team1 ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &expected_division_team2 ) );

     division_s *actual = get_division( db, expected.division_id );

     division_team_s *actual_division_teams = actual->teams;

     assertNotNull( actual_division_teams );

     assertEquals( expected_division_team1.division_id, actual_division_teams[0].division_id );
     assertEquals( expected_division_team1.team_id,     actual_division_teams[0].team_id     );

     assertEquals( expected_division_team2.division_id, actual_division_teams[1].division_id );
     assertEquals( expected_division_team2.team_id,     actual_division_teams[1].team_id     );

     assertEquals( division_team_sentinel.division_id, actual_division_teams[2].division_id );
     assertEquals( division_team_sentinel.team_id,     actual_division_teams[2].team_id     );

     free_division( actual );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_team1) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_team2) );

     return NULL;
}

static char *get_division__ShouldReturnTheMatchingDivisionWithStats_GivenADivisionId()
{
     division_s       expected        = { 0 };
     division_stats_s expected_stats1 = { 0 };
     division_stats_s expected_stats2 = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );

     expected_stats1.division_id   = 1;
     expected_stats1.season        = 1;
     expected_stats1.season_phase  = sp_Regular;
     expected_stats1.wins          = 388;
     expected_stats1.losses        = 380;
     expected_stats1.home_wins     = 205;
     expected_stats1.home_losses   = 179;
     expected_stats1.road_wins     = 166;
     expected_stats1.road_losses   = 218;
     expected_stats1.league_wins   = 135;
     expected_stats1.league_losses = 121;
     expected_stats1.runs_scored   = 2642;
     expected_stats1.runs_allowed  = 2619;

     expected_stats2.division_id   = 1;
     expected_stats2.season        = 2;
     expected_stats2.season_phase  = sp_Regular;
     expected_stats2.wins          = 392;
     expected_stats2.losses        = 376;
     expected_stats2.home_wins     = 195;
     expected_stats2.home_losses   = 189;
     expected_stats2.road_wins     = 167;
     expected_stats2.road_losses   = 217;
     expected_stats2.league_wins   = 138;
     expected_stats2.league_losses = 118;
     expected_stats2.runs_scored   = 2698;
     expected_stats2.runs_allowed  = 2703;

     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected_stats1 ) );
     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected_stats2 ) );

     division_s *actual = get_division( db, expected.division_id );

     division_stats_s *actual_division_stats = actual->stats;

     assertNotNull( actual_division_stats );

     assertEquals( expected_stats1.division_id,   actual_division_stats[0].division_id     );
     assertEquals( expected_stats1.season,        actual_division_stats[0].season          );
     assertEquals( expected_stats1.season_phase,  actual_division_stats[0].season_phase    );
     assertEquals( expected_stats1.wins,          actual_division_stats[0].wins            );
     assertEquals( expected_stats1.losses,        actual_division_stats[0].losses          );
     assertEquals( expected_stats1.home_wins,     actual_division_stats[0].home_wins       );
     assertEquals( expected_stats1.home_losses,   actual_division_stats[0].home_losses     );
     assertEquals( expected_stats1.road_wins,     actual_division_stats[0].road_wins       );
     assertEquals( expected_stats1.road_losses,   actual_division_stats[0].road_losses     );
     assertEquals( expected_stats1.league_wins,   actual_division_stats[0].league_wins     );
     assertEquals( expected_stats1.league_losses, actual_division_stats[0].league_losses   );
     assertEquals( expected_stats1.runs_scored,   actual_division_stats[0].runs_scored     );
     assertEquals( expected_stats1.runs_allowed,  actual_division_stats[0].runs_allowed    );

     assertEquals( expected_stats2.division_id,   actual_division_stats[1].division_id     );
     assertEquals( expected_stats2.season,        actual_division_stats[1].season          );
     assertEquals( expected_stats2.season_phase,  actual_division_stats[1].season_phase    );
     assertEquals( expected_stats2.wins,          actual_division_stats[1].wins            );
     assertEquals( expected_stats2.losses,        actual_division_stats[1].losses          );
     assertEquals( expected_stats2.home_wins,     actual_division_stats[1].home_wins       );
     assertEquals( expected_stats2.home_losses,   actual_division_stats[1].home_losses     );
     assertEquals( expected_stats2.road_wins,     actual_division_stats[1].road_wins       );
     assertEquals( expected_stats2.road_losses,   actual_division_stats[1].road_losses     );
     assertEquals( expected_stats2.league_wins,   actual_division_stats[1].league_wins     );
     assertEquals( expected_stats2.league_losses, actual_division_stats[1].league_losses   );
     assertEquals( expected_stats2.runs_scored,   actual_division_stats[1].runs_scored     );
     assertEquals( expected_stats2.runs_allowed,  actual_division_stats[1].runs_allowed    );

     assertEquals( division_stats_sentinel.division_id,   actual_division_stats[2].division_id     );
     assertEquals( division_stats_sentinel.season,        actual_division_stats[2].season          );
     assertEquals( division_stats_sentinel.season_phase,  actual_division_stats[2].season_phase    );
     assertEquals( division_stats_sentinel.wins,          actual_division_stats[2].wins            );
     assertEquals( division_stats_sentinel.losses,        actual_division_stats[2].losses          );
     assertEquals( division_stats_sentinel.home_wins,     actual_division_stats[2].home_wins       );
     assertEquals( division_stats_sentinel.home_losses,   actual_division_stats[2].home_losses     );
     assertEquals( division_stats_sentinel.road_wins,     actual_division_stats[2].road_wins       );
     assertEquals( division_stats_sentinel.road_losses,   actual_division_stats[2].road_losses     );
     assertEquals( division_stats_sentinel.league_wins,   actual_division_stats[2].league_wins     );
     assertEquals( division_stats_sentinel.league_losses, actual_division_stats[2].league_losses   );
     assertEquals( division_stats_sentinel.runs_scored,   actual_division_stats[2].runs_scored     );
     assertEquals( division_stats_sentinel.runs_allowed,  actual_division_stats[2].runs_allowed    );

     free_division( actual );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats1 ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats2 ) );

     return NULL;
}

static char *get_division__ShouldReturnTheMatchingDivisionWithAccolades_GivenADivisionId()
{
     division_s          expected           = { 0 };
     division_accolade_s expected_accolade1 = { 0 };
     division_accolade_s expected_accolade2 = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );

     expected_accolade1.division_id = 1;
     expected_accolade1.season      = 1;
     expected_accolade1.accolade    = dacc_League_Title;

     expected_accolade2.division_id = 1;
     expected_accolade2.season      = 2;
     expected_accolade2.accolade    = dacc_Best_Record;

     assertEquals( SQLITE_OK, division_accolades_t_create( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, division_accolades_t_create( db, &expected_accolade2 ) );

     division_s *actual = get_division( db, expected.division_id );

     division_accolade_s *actual_division_accolades = actual->accolades;

     assertNotNull( actual_division_accolades );

     assertEquals( expected_accolade1.division_id, actual_division_accolades[0].division_id );
     assertEquals( expected_accolade1.season,      actual_division_accolades[0].season      );
     assertEquals( expected_accolade1.accolade,    actual_division_accolades[0].accolade    );

     assertEquals( expected_accolade2.division_id, actual_division_accolades[1].division_id );
     assertEquals( expected_accolade2.season,      actual_division_accolades[1].season      );
     assertEquals( expected_accolade2.accolade,    actual_division_accolades[1].accolade    );

     assertEquals( division_accolade_sentinel.division_id, actual_division_accolades[2].division_id );
     assertEquals( division_accolade_sentinel.season,      actual_division_accolades[2].season      );
     assertEquals( division_accolade_sentinel.accolade,    actual_division_accolades[2].accolade    );

     free_division( actual );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolade2 ) );

     return NULL;
}

static char *save_division__ShouldPersistTheDivisionInTheDatabase_GivenADivisionObject()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( SQLITE_OK, save_division( db, &expected ) );

     division_s actual = { 0 };

     actual.division_id = expected.division_id;

     assertEquals( SQLITE_OK, divisions_t_read( db, &actual ) );

     assertEqualsInt( expected.division_id, actual.division_id );
     assertEqualsStr( expected.name,        actual.name        );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );

     return NULL;
}

static char *save_division__ShouldPersistTheDivisionTeamsInTheDatabase_GivenADivisionWithDivisionTeams()
{
     division_s      expected                   = { 0 };
     division_team_s expected_division_teams[3] = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_division_teams[0].division_id = 1;
     expected_division_teams[0].team_id     = 2;

     expected_division_teams[1].division_id = 1;
     expected_division_teams[1].team_id     = 5;

     expected_division_teams[2] = division_team_sentinel;

     expected.teams = expected_division_teams;

     assertEquals( SQLITE_OK, save_division( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_teams_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( 2, list.count );

     division_team_s *actual_division_teams = list.data;

     assertNotNull( actual_division_teams );

     assertEquals( expected_division_teams[0].division_id, actual_division_teams[0].division_id );
     assertEquals( expected_division_teams[0].team_id,     actual_division_teams[0].team_id     );

     assertEquals( expected_division_teams[1].division_id, actual_division_teams[1].division_id );
     assertEquals( expected_division_teams[1].team_id,     actual_division_teams[1].team_id     );

     free( actual_division_teams );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[0] ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[1] ) );

     return NULL;
}

static char *save_division__ShouldPersistTheDivisionStatsInTheDatabase_GivenADivisionWithDivisionStats()
{
     division_s       expected          = { 0 };
     division_stats_s expected_stats[3] = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_stats[0].division_id   = 1;
     expected_stats[0].season        = 1;
     expected_stats[0].season_phase  = sp_Regular;
     expected_stats[0].wins          = 388;
     expected_stats[0].losses        = 380;
     expected_stats[0].home_wins     = 205;
     expected_stats[0].home_losses   = 179;
     expected_stats[0].road_wins     = 166;
     expected_stats[0].road_losses   = 218;
     expected_stats[0].league_wins   = 135;
     expected_stats[0].league_losses = 121;
     expected_stats[0].runs_scored   = 2642;
     expected_stats[0].runs_allowed  = 2619;

     expected_stats[1].division_id   = 1;
     expected_stats[1].season        = 2;
     expected_stats[1].season_phase  = sp_Regular;
     expected_stats[1].wins          = 392;
     expected_stats[1].losses        = 376;
     expected_stats[1].home_wins     = 195;
     expected_stats[1].home_losses   = 189;
     expected_stats[1].road_wins     = 167;
     expected_stats[1].road_losses   = 217;
     expected_stats[1].league_wins   = 138;
     expected_stats[1].league_losses = 118;
     expected_stats[1].runs_scored   = 2698;
     expected_stats[1].runs_allowed  = 2703;

     expected_stats[2] = division_stats_sentinel;

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, save_division( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_stats_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( 2, list.count );

     division_stats_s *actual_division_stats = list.data;

     assertNotNull( actual_division_stats );

     assertEquals( expected_stats[0].division_id,   actual_division_stats[0].division_id     );
     assertEquals( expected_stats[0].season,        actual_division_stats[0].season          );
     assertEquals( expected_stats[0].season_phase,  actual_division_stats[0].season_phase    );
     assertEquals( expected_stats[0].wins,          actual_division_stats[0].wins            );
     assertEquals( expected_stats[0].losses,        actual_division_stats[0].losses          );
     assertEquals( expected_stats[0].home_wins,     actual_division_stats[0].home_wins       );
     assertEquals( expected_stats[0].home_losses,   actual_division_stats[0].home_losses     );
     assertEquals( expected_stats[0].road_wins,     actual_division_stats[0].road_wins       );
     assertEquals( expected_stats[0].road_losses,   actual_division_stats[0].road_losses     );
     assertEquals( expected_stats[0].league_wins,   actual_division_stats[0].league_wins     );
     assertEquals( expected_stats[0].league_losses, actual_division_stats[0].league_losses   );
     assertEquals( expected_stats[0].runs_scored,   actual_division_stats[0].runs_scored     );
     assertEquals( expected_stats[0].runs_allowed,  actual_division_stats[0].runs_allowed    );

     assertEquals( expected_stats[1].division_id,   actual_division_stats[1].division_id     );
     assertEquals( expected_stats[1].season,        actual_division_stats[1].season          );
     assertEquals( expected_stats[1].season_phase,  actual_division_stats[1].season_phase    );
     assertEquals( expected_stats[1].wins,          actual_division_stats[1].wins            );
     assertEquals( expected_stats[1].losses,        actual_division_stats[1].losses          );
     assertEquals( expected_stats[1].home_wins,     actual_division_stats[1].home_wins       );
     assertEquals( expected_stats[1].home_losses,   actual_division_stats[1].home_losses     );
     assertEquals( expected_stats[1].road_wins,     actual_division_stats[1].road_wins       );
     assertEquals( expected_stats[1].road_losses,   actual_division_stats[1].road_losses     );
     assertEquals( expected_stats[1].league_wins,   actual_division_stats[1].league_wins     );
     assertEquals( expected_stats[1].league_losses, actual_division_stats[1].league_losses   );
     assertEquals( expected_stats[1].runs_scored,   actual_division_stats[1].runs_scored     );
     assertEquals( expected_stats[1].runs_allowed,  actual_division_stats[1].runs_allowed    );

     free( actual_division_stats );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats[1] ) );

     return NULL;
}

static char *save_division__ShouldPersistTheAccoladesInTheDatabase_GivenADivisionWithAccolades()
{
     division_s          expected              = { 0 };
     division_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_accolades[0].division_id = 1;
     expected_accolades[0].season      = 1;
     expected_accolades[0].accolade    = dacc_League_Title;

     expected_accolades[1].division_id = 1;
     expected_accolades[1].season      = 2;
     expected_accolades[1].accolade    = dacc_Best_Record;

     expected_accolades[2] = division_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_division( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_accolades_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( 2, list.count );

     division_accolade_s *actual_division_accolades = list.data;

     assertNotNull( actual_division_accolades );

     assertEquals( expected_accolades[0].division_id, actual_division_accolades[0].division_id );
     assertEquals( expected_accolades[0].season,      actual_division_accolades[0].season      );
     assertEquals( expected_accolades[0].accolade,    actual_division_accolades[0].accolade    );

     assertEquals( expected_accolades[1].division_id, actual_division_accolades[1].division_id );
     assertEquals( expected_accolades[1].season,      actual_division_accolades[1].season      );
     assertEquals( expected_accolades[1].accolade,    actual_division_accolades[1].accolade    );

     free( actual_division_accolades );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *save_division__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenADivision()
{
     division_s          expected                   = { 0 };
     division_team_s     expected_division_teams[3] = { 0 };
     division_stats_s    expected_stats[3]          = { 0 };
     division_accolade_s expected_accolades[3]      = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_division_teams[0].division_id = 1;
     expected_division_teams[0].team_id     = 2;

     expected_division_teams[1].division_id = 1;
     expected_division_teams[1].team_id     = 5;

     expected_division_teams[2] = division_team_sentinel;

     expected_stats[0].division_id   = 1;
     expected_stats[0].season        = 1;
     expected_stats[0].season_phase  = sp_Regular;
     expected_stats[0].wins          = 388;
     expected_stats[0].losses        = 380;
     expected_stats[0].home_wins     = 205;
     expected_stats[0].home_losses   = 179;
     expected_stats[0].road_wins     = 166;
     expected_stats[0].road_losses   = 218;
     expected_stats[0].league_wins   = 135;
     expected_stats[0].league_losses = 121;
     expected_stats[0].runs_scored   = 2642;
     expected_stats[0].runs_allowed  = 2619;

     expected_stats[1].division_id   = 1;
     expected_stats[1].season        = 2;
     expected_stats[1].season_phase  = sp_Regular;
     expected_stats[1].wins          = 392;
     expected_stats[1].losses        = 376;
     expected_stats[1].home_wins     = 195;
     expected_stats[1].home_losses   = 189;
     expected_stats[1].road_wins     = 167;
     expected_stats[1].road_losses   = 217;
     expected_stats[1].league_wins   = 138;
     expected_stats[1].league_losses = 118;
     expected_stats[1].runs_scored   = 2698;
     expected_stats[1].runs_allowed  = 2703;

     expected_stats[2] = division_stats_sentinel;

     expected_accolades[0].division_id = 1;
     expected_accolades[0].season      = 1;
     expected_accolades[0].accolade    = dacc_League_Title;

     expected_accolades[1].division_id = 1;
     expected_accolades[1].season      = 2;
     expected_accolades[1].accolade    = dacc_Best_Record;

     expected_accolades[2] = division_accolade_sentinel;

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &expected_division_teams[0] ) );
     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, division_accolades_t_create( db, &expected_accolades[0] ) );

     strcpy( expected.name, "DivisionName2" );

     expected_stats[0].wins          = 390;
     expected_stats[0].losses        = 378;

     expected.teams     = expected_division_teams;
     expected.stats     = expected_stats;
     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_division( db, &expected ) );

     division_s actual = { 0 };

     actual.division_id = expected.division_id;

     assertEquals( SQLITE_OK, divisions_t_read( db, &actual ) );

     assertEqualsInt( expected.division_id, actual.division_id );
     assertEqualsStr( expected.name,        actual.name        );

     data_list_s division_teams_list = { 0 };

     assertEquals( SQLITE_OK, division_teams_t_read_by_division( db, expected.division_id, &division_teams_list ) );

     assertEquals( 2, division_teams_list.count );

     division_team_s *actual_division_teams = division_teams_list.data;

     assertNotNull( actual_division_teams );

     assertEquals( expected_division_teams[0].division_id, actual_division_teams[0].division_id );
     assertEquals( expected_division_teams[0].team_id,     actual_division_teams[0].team_id     );

     assertEquals( expected_division_teams[1].division_id, actual_division_teams[1].division_id );
     assertEquals( expected_division_teams[1].team_id,     actual_division_teams[1].team_id     );

     data_list_s division_stats_list = { 0 };

     assertEquals( SQLITE_OK, division_stats_t_read_by_division( db, expected.division_id, &division_stats_list ) );

     assertEquals( 2, division_stats_list.count );

     division_stats_s *actual_division_stats = division_stats_list.data;

     assertNotNull( actual_division_stats );

     assertEquals( expected_stats[0].division_id,   actual_division_stats[0].division_id     );
     assertEquals( expected_stats[0].season,        actual_division_stats[0].season          );
     assertEquals( expected_stats[0].season_phase,  actual_division_stats[0].season_phase    );
     assertEquals( expected_stats[0].wins,          actual_division_stats[0].wins            );
     assertEquals( expected_stats[0].losses,        actual_division_stats[0].losses          );
     assertEquals( expected_stats[0].home_wins,     actual_division_stats[0].home_wins       );
     assertEquals( expected_stats[0].home_losses,   actual_division_stats[0].home_losses     );
     assertEquals( expected_stats[0].road_wins,     actual_division_stats[0].road_wins       );
     assertEquals( expected_stats[0].road_losses,   actual_division_stats[0].road_losses     );
     assertEquals( expected_stats[0].league_wins,   actual_division_stats[0].league_wins     );
     assertEquals( expected_stats[0].league_losses, actual_division_stats[0].league_losses   );
     assertEquals( expected_stats[0].runs_scored,   actual_division_stats[0].runs_scored     );
     assertEquals( expected_stats[0].runs_allowed,  actual_division_stats[0].runs_allowed    );

     assertEquals( expected_stats[1].division_id,   actual_division_stats[1].division_id     );
     assertEquals( expected_stats[1].season,        actual_division_stats[1].season          );
     assertEquals( expected_stats[1].season_phase,  actual_division_stats[1].season_phase    );
     assertEquals( expected_stats[1].wins,          actual_division_stats[1].wins            );
     assertEquals( expected_stats[1].losses,        actual_division_stats[1].losses          );
     assertEquals( expected_stats[1].home_wins,     actual_division_stats[1].home_wins       );
     assertEquals( expected_stats[1].home_losses,   actual_division_stats[1].home_losses     );
     assertEquals( expected_stats[1].road_wins,     actual_division_stats[1].road_wins       );
     assertEquals( expected_stats[1].road_losses,   actual_division_stats[1].road_losses     );
     assertEquals( expected_stats[1].league_wins,   actual_division_stats[1].league_wins     );
     assertEquals( expected_stats[1].league_losses, actual_division_stats[1].league_losses   );
     assertEquals( expected_stats[1].runs_scored,   actual_division_stats[1].runs_scored     );
     assertEquals( expected_stats[1].runs_allowed,  actual_division_stats[1].runs_allowed    );

     data_list_s division_accolades_list = { 0 };

     assertEquals( SQLITE_OK, division_accolades_t_read_by_division( db, expected.division_id, &division_accolades_list ) );

     assertEquals( 2, division_accolades_list.count );

     division_accolade_s *actual_division_accolades = division_accolades_list.data;

     assertNotNull( actual_division_accolades );

     assertEquals( expected_accolades[0].division_id, actual_division_accolades[0].division_id );
     assertEquals( expected_accolades[0].season,      actual_division_accolades[0].season      );
     assertEquals( expected_accolades[0].accolade,    actual_division_accolades[0].accolade    );

     assertEquals( expected_accolades[1].division_id, actual_division_accolades[1].division_id );
     assertEquals( expected_accolades[1].season,      actual_division_accolades[1].season      );
     assertEquals( expected_accolades[1].accolade,    actual_division_accolades[1].accolade    );

     free( actual_division_teams     );
     free( actual_division_stats     );
     free( actual_division_accolades );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[0] ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[1] ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats[1] ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *remove_division__ShouldRemoveTheDivisionFromTheDatabase_GivenADivisionObject()
{
     division_s expected = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );

     assertEquals( SQLITE_OK, remove_division( db, &expected ) );

     division_s actual = { 0 };

     actual.division_id = expected.division_id;

     assertEquals( SQLITE_NOTFOUND, divisions_t_read( db, &actual ) );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );

     return NULL;
}

static char *remove_division__ShouldRemoveTheDivisionTeamsFromTheDatabase_GivenADivisionWithDivisionTeams()
{
     division_s      expected                   = { 0 };
     division_team_s expected_division_teams[3] = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_division_teams[0].division_id = 1;
     expected_division_teams[0].team_id     = 2;

     expected_division_teams[1].division_id = 1;
     expected_division_teams[1].team_id     = 5;

     expected_division_teams[2] = division_team_sentinel;

     expected.teams = expected_division_teams;

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &expected_division_teams[0] ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &expected_division_teams[1] ) );

     assertEquals( SQLITE_OK, remove_division( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, division_teams_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[0] ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[1] ) );

     return NULL;
}

static char *remove_division__ShouldRemoveTheDivisionStatsFromTheDatabase_GivenADivisionWithDivisionStats()
{
     division_s       expected          = { 0 };
     division_stats_s expected_stats[3] = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_stats[0].division_id   = 1;
     expected_stats[0].season        = 1;
     expected_stats[0].season_phase  = sp_Regular;
     expected_stats[0].wins          = 388;
     expected_stats[0].losses        = 380;
     expected_stats[0].home_wins     = 205;
     expected_stats[0].home_losses   = 179;
     expected_stats[0].road_wins     = 166;
     expected_stats[0].road_losses   = 218;
     expected_stats[0].league_wins   = 135;
     expected_stats[0].league_losses = 121;
     expected_stats[0].runs_scored   = 2642;
     expected_stats[0].runs_allowed  = 2619;

     expected_stats[1].division_id   = 1;
     expected_stats[1].season        = 2;
     expected_stats[1].season_phase  = sp_Regular;
     expected_stats[1].wins          = 392;
     expected_stats[1].losses        = 376;
     expected_stats[1].home_wins     = 195;
     expected_stats[1].home_losses   = 189;
     expected_stats[1].road_wins     = 167;
     expected_stats[1].road_losses   = 217;
     expected_stats[1].league_wins   = 138;
     expected_stats[1].league_losses = 118;
     expected_stats[1].runs_scored   = 2698;
     expected_stats[1].runs_allowed  = 2703;

     expected_stats[2] = division_stats_sentinel;

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, division_stats_t_create( db, &expected_stats[1] ) );

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, remove_division( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, division_stats_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, division_stats_t_delete( db, &expected_stats[1] ) );

     return NULL;

}

static char *remove_division__ShouldRemoveTheAccoladesFromTheDatabase_GivenADivisionWithAccolades()
{
     division_s          expected              = { 0 };
     division_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.division_id = 1;
     strcpy( expected.name,        "DivisionName1" );

     expected_accolades[0].division_id = 1;
     expected_accolades[0].season      = 1;
     expected_accolades[0].accolade    = dacc_League_Title;

     expected_accolades[1].division_id = 1;
     expected_accolades[1].season      = 2;
     expected_accolades[1].accolade    = dacc_Best_Record;

     expected_accolades[2] = division_accolade_sentinel;

     assertEquals( SQLITE_OK, divisions_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, division_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, division_accolades_t_create( db, &expected_accolades[1] ) );

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, remove_division( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, division_accolades_t_read_by_division( db, expected.division_id, &list ) );

     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, division_accolades_t_delete( db, &expected_accolades[1] ) );

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
     // get_division()
     run_test( get_division__ShouldReturnTheMatchingDivisionObject_GivenADivisionId,        check_sqlite_error );
     run_test( get_division__ShouldReturnTheMatchingDivisionWithTeams_GivenADivisionId,     check_sqlite_error );
     run_test( get_division__ShouldReturnTheMatchingDivisionWithStats_GivenADivisionId,     check_sqlite_error );
     run_test( get_division__ShouldReturnTheMatchingDivisionWithAccolades_GivenADivisionId, check_sqlite_error );

     // save_division()
     run_test( save_division__ShouldPersistTheDivisionInTheDatabase_GivenADivisionObject,                 check_sqlite_error );
     run_test( save_division__ShouldPersistTheDivisionTeamsInTheDatabase_GivenADivisionWithDivisionTeams, check_sqlite_error );
     run_test( save_division__ShouldPersistTheDivisionStatsInTheDatabase_GivenADivisionWithDivisionStats, check_sqlite_error );
     run_test( save_division__ShouldPersistTheAccoladesInTheDatabase_GivenADivisionWithAccolades,         check_sqlite_error );
     run_test( save_division__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenADivision,           check_sqlite_error );

     // remove_division()
     run_test( remove_division__ShouldRemoveTheDivisionFromTheDatabase_GivenADivisionObject,                 check_sqlite_error );
     run_test( remove_division__ShouldRemoveTheDivisionTeamsFromTheDatabase_GivenADivisionWithDivisionTeams, check_sqlite_error );
     run_test( remove_division__ShouldRemoveTheDivisionStatsFromTheDatabase_GivenADivisionWithDivisionStats, check_sqlite_error );
     run_test( remove_division__ShouldRemoveTheAccoladesFromTheDatabase_GivenADivisionWithAccolades,         check_sqlite_error );
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
