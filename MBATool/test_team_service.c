#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "unit_test.h"


static team_player_s         team_player_sentinel         = TEAM_PLAYER_SENTINEL;
static team_stats_s          team_stats_sentinel          = TEAM_STATS_SENTINEL;
static team_pitching_stats_s team_pitching_stats_sentinel = TEAM_PITCHING_STATS_SENTINEL;
static team_batting_stats_s  team_batting_stats_sentinel  = TEAM_BATTING_STATS_SENTINEL;
static team_accolade_s       team_accolade_sentinel       = TEAM_ACCOLADE_SENTINEL;

static sqlite3 *db;
static char    *result;


static char *get_team__ShouldReturnTheMatchingTeamObject_GivenATeamId()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     team_s *actual = get_team( db, expected.team_id );

     assertNotNull( actual );

     assertEqualsInt( expected.team_id,         actual->team_id         );
     assertEqualsStr( expected.name,            actual->name            );
     assertEqualsStr( expected.location,        actual->location        );
     assertEqualsInt( expected.primary_color,   actual->primary_color   );
     assertEqualsInt( expected.secondary_color, actual->secondary_color );

     free( actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );

     return NULL;
}

static char *get_team__ShouldReturnTheMatchingTeamWithPlayers_GivenATeamId()
{
     team_s        expected              = { 0 };
     team_player_s expected_team_player1 = { 0 };
     team_player_s expected_team_player2 = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_team_player1.team_id   = 1;
     expected_team_player1.season    = 1;
     expected_team_player1.player_id = 1;

     expected_team_player2.team_id   = 1;
     expected_team_player2.season    = 2;
     expected_team_player2.player_id = 2;

     assertEquals( SQLITE_OK, team_players_t_create( db, &expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &expected_team_player2 ) );

     team_s *actual = get_team( db, expected.team_id );

     assertNotNull( actual );

     team_player_s *actual_team_players = actual->players;

     assertNotNull( actual_team_players );

     assertEquals( expected_team_player1.team_id,   actual_team_players[0].team_id   );
     assertEquals( expected_team_player1.season,    actual_team_players[0].season    );
     assertEquals( expected_team_player1.player_id, actual_team_players[0].player_id );

     assertEquals( expected_team_player2.team_id,   actual_team_players[1].team_id   );
     assertEquals( expected_team_player2.season,    actual_team_players[1].season    );
     assertEquals( expected_team_player2.player_id, actual_team_players[1].player_id );

     assertEquals( team_player_sentinel.team_id,   actual_team_players[2].team_id   );
     assertEquals( team_player_sentinel.season,    actual_team_players[2].season    );
     assertEquals( team_player_sentinel.player_id, actual_team_players[2].player_id );

     free_team( actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_player2 ) );

     return NULL;
}

static char *get_team__ShouldReturnTheMatchingTeamWithStats_GivenATeamId()
{
     team_s       expected        = { 0 };
     team_stats_s expected_stats1 = { 0 };
     team_stats_s expected_stats2 = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_stats1.team_id         = 1;
     expected_stats1.season          = 1;
     expected_stats1.season_phase    = sp_Regular;
     expected_stats1.wins            = 85;
     expected_stats1.losses          = 67;
     expected_stats1.home_wins       = 56;
     expected_stats1.home_losses     = 20;
     expected_stats1.road_wins       = 29;
     expected_stats1.road_losses     = 47;
     expected_stats1.division_wins   = 30;
     expected_stats1.division_losses = 26;
     expected_stats1.league_wins     = 52;
     expected_stats1.league_losses   = 36;
     expected_stats1.runs_scored     = 234;
     expected_stats1.runs_allowed    = 222;

     expected_stats2.team_id         = 1;
     expected_stats2.season          = 2;
     expected_stats2.season_phase    = sp_Regular;
     expected_stats2.wins            = 92;
     expected_stats2.losses          = 60;
     expected_stats2.home_wins       = 50;
     expected_stats2.home_losses     = 26;
     expected_stats2.road_wins       = 42;
     expected_stats2.road_losses     = 24;
     expected_stats2.division_wins   = 36;
     expected_stats2.division_losses = 20;
     expected_stats2.league_wins     = 60;
     expected_stats2.league_losses   = 28;
     expected_stats2.runs_scored     = 199;
     expected_stats2.runs_allowed    = 141;

     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected_stats1 ) );
     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected_stats2 ) );

     team_s *actual = get_team( db, expected.team_id );

     assertNotNull( actual );

     team_stats_s *actual_stats = actual->stats;

     assertNotNull( actual_stats );

     assertEquals( expected_stats1.team_id,         actual_stats[0].team_id         );
     assertEquals( expected_stats1.season,          actual_stats[0].season          );
     assertEquals( expected_stats1.season_phase,    actual_stats[0].season_phase    );
     assertEquals( expected_stats1.wins,            actual_stats[0].wins            );
     assertEquals( expected_stats1.losses,          actual_stats[0].losses          );
     assertEquals( expected_stats1.home_wins,       actual_stats[0].home_wins       );
     assertEquals( expected_stats1.home_losses,     actual_stats[0].home_losses     );
     assertEquals( expected_stats1.road_wins,       actual_stats[0].road_wins       );
     assertEquals( expected_stats1.road_losses,     actual_stats[0].road_losses     );
     assertEquals( expected_stats1.division_wins,   actual_stats[0].division_wins   );
     assertEquals( expected_stats1.division_losses, actual_stats[0].division_losses );
     assertEquals( expected_stats1.league_wins,     actual_stats[0].league_wins     );
     assertEquals( expected_stats1.league_losses,   actual_stats[0].league_losses   );
     assertEquals( expected_stats1.runs_scored,     actual_stats[0].runs_scored     );
     assertEquals( expected_stats1.runs_allowed,    actual_stats[0].runs_allowed    );

     assertEquals( expected_stats2.team_id,         actual_stats[1].team_id         );
     assertEquals( expected_stats2.season,          actual_stats[1].season          );
     assertEquals( expected_stats2.season_phase,    actual_stats[1].season_phase    );
     assertEquals( expected_stats2.wins,            actual_stats[1].wins            );
     assertEquals( expected_stats2.losses,          actual_stats[1].losses          );
     assertEquals( expected_stats2.home_wins,       actual_stats[1].home_wins       );
     assertEquals( expected_stats2.home_losses,     actual_stats[1].home_losses     );
     assertEquals( expected_stats2.road_wins,       actual_stats[1].road_wins       );
     assertEquals( expected_stats2.road_losses,     actual_stats[1].road_losses     );
     assertEquals( expected_stats2.division_wins,   actual_stats[1].division_wins   );
     assertEquals( expected_stats2.division_losses, actual_stats[1].division_losses );
     assertEquals( expected_stats2.league_wins,     actual_stats[1].league_wins     );
     assertEquals( expected_stats2.league_losses,   actual_stats[1].league_losses   );
     assertEquals( expected_stats2.runs_scored,     actual_stats[1].runs_scored     );
     assertEquals( expected_stats2.runs_allowed,    actual_stats[1].runs_allowed    );

     assertEquals( team_stats_sentinel.team_id,         actual_stats[2].team_id         );
     assertEquals( team_stats_sentinel.season,          actual_stats[2].season          );
     assertEquals( team_stats_sentinel.season_phase,    actual_stats[2].season_phase    );
     assertEquals( team_stats_sentinel.wins,            actual_stats[2].wins            );
     assertEquals( team_stats_sentinel.losses,          actual_stats[2].losses          );
     assertEquals( team_stats_sentinel.home_wins,       actual_stats[2].home_wins       );
     assertEquals( team_stats_sentinel.home_losses,     actual_stats[2].home_losses     );
     assertEquals( team_stats_sentinel.road_wins,       actual_stats[2].road_wins       );
     assertEquals( team_stats_sentinel.road_losses,     actual_stats[2].road_losses     );
     assertEquals( team_stats_sentinel.division_wins,   actual_stats[2].division_wins   );
     assertEquals( team_stats_sentinel.division_losses, actual_stats[2].division_losses );
     assertEquals( team_stats_sentinel.league_wins,     actual_stats[2].league_wins     );
     assertEquals( team_stats_sentinel.league_losses,   actual_stats[2].league_losses   );
     assertEquals( team_stats_sentinel.runs_scored,     actual_stats[2].runs_scored     );
     assertEquals( team_stats_sentinel.runs_allowed,    actual_stats[2].runs_allowed    );

     free_team( actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats1 ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats2 ) );

     return NULL;
}

static char *get_team__ShouldReturnTheMatchingTeamWithPitchingStats_GivenATeamId()
{
     team_s                expected                 = { 0 };
     team_pitching_stats_s expected_pitching_stats1 = { 0 };
     team_pitching_stats_s expected_pitching_stats2 = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_pitching_stats1.team_id      = 1;
     expected_pitching_stats1.season       = 1;
     expected_pitching_stats1.season_phase = sp_Regular;
     expected_pitching_stats1.wins         = 12;
     expected_pitching_stats1.losses       = 15;
     expected_pitching_stats1.games        = 54;
     expected_pitching_stats1.saves        = 11;
     expected_pitching_stats1.innings      = 321.2;
     expected_pitching_stats1.hits         = 165;
     expected_pitching_stats1.earned_runs  = 23;
     expected_pitching_stats1.home_runs    = 14;
     expected_pitching_stats1.walks        = 25;
     expected_pitching_stats1.strike_outs  = 234;

     expected_pitching_stats2.team_id      = 1;
     expected_pitching_stats2.season       = 2;
     expected_pitching_stats2.season_phase = sp_Regular;
     expected_pitching_stats2.wins         = 21;
     expected_pitching_stats2.losses       = 51;
     expected_pitching_stats2.games        = 45;
     expected_pitching_stats2.saves        = 99;
     expected_pitching_stats2.innings      = 123.1;
     expected_pitching_stats2.hits         = 156;
     expected_pitching_stats2.earned_runs  = 32;
     expected_pitching_stats2.home_runs    = 41;
     expected_pitching_stats2.walks        = 52;
     expected_pitching_stats2.strike_outs  = 321;

     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected_pitching_stats1 ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected_pitching_stats2 ) );

     team_s *actual = get_team( db, expected.team_id );

     assertNotNull( actual );

     team_pitching_stats_s *actual_pitching_stats = actual->pitching_stats;

     assertNotNull( actual_pitching_stats );

     assertEqualsInt( expected_pitching_stats1.team_id,      actual_pitching_stats[0].team_id      );
     assertEqualsInt( expected_pitching_stats1.season,       actual_pitching_stats[0].season       );
     assertEqualsInt( expected_pitching_stats1.season_phase, actual_pitching_stats[0].season_phase );
     assertEqualsInt( expected_pitching_stats1.wins,         actual_pitching_stats[0].wins         );
     assertEqualsInt( expected_pitching_stats1.losses,       actual_pitching_stats[0].losses       );
     assertEqualsInt( expected_pitching_stats1.games,        actual_pitching_stats[0].games        );
     assertEqualsInt( expected_pitching_stats1.saves,        actual_pitching_stats[0].saves        );
     assertEqualsDbl( expected_pitching_stats1.innings,      actual_pitching_stats[0].innings      );
     assertEqualsInt( expected_pitching_stats1.hits,         actual_pitching_stats[0].hits         );
     assertEqualsInt( expected_pitching_stats1.earned_runs,  actual_pitching_stats[0].earned_runs  );
     assertEqualsInt( expected_pitching_stats1.home_runs,    actual_pitching_stats[0].home_runs    );
     assertEqualsInt( expected_pitching_stats1.walks,        actual_pitching_stats[0].walks        );
     assertEqualsInt( expected_pitching_stats1.strike_outs,  actual_pitching_stats[0].strike_outs  );

     assertEqualsInt( expected_pitching_stats2.team_id,      actual_pitching_stats[1].team_id      );
     assertEqualsInt( expected_pitching_stats2.season,       actual_pitching_stats[1].season       );
     assertEqualsInt( expected_pitching_stats2.season_phase, actual_pitching_stats[1].season_phase );
     assertEqualsInt( expected_pitching_stats2.wins,         actual_pitching_stats[1].wins         );
     assertEqualsInt( expected_pitching_stats2.losses,       actual_pitching_stats[1].losses       );
     assertEqualsInt( expected_pitching_stats2.games,        actual_pitching_stats[1].games        );
     assertEqualsInt( expected_pitching_stats2.saves,        actual_pitching_stats[1].saves        );
     assertEqualsDbl( expected_pitching_stats2.innings,      actual_pitching_stats[1].innings      );
     assertEqualsInt( expected_pitching_stats2.hits,         actual_pitching_stats[1].hits         );
     assertEqualsInt( expected_pitching_stats2.earned_runs,  actual_pitching_stats[1].earned_runs  );
     assertEqualsInt( expected_pitching_stats2.home_runs,    actual_pitching_stats[1].home_runs    );
     assertEqualsInt( expected_pitching_stats2.walks,        actual_pitching_stats[1].walks        );
     assertEqualsInt( expected_pitching_stats2.strike_outs,  actual_pitching_stats[1].strike_outs  );

     assertEqualsInt( team_pitching_stats_sentinel.team_id,      actual_pitching_stats[2].team_id      );
     assertEqualsInt( team_pitching_stats_sentinel.season,       actual_pitching_stats[2].season       );
     assertEqualsInt( team_pitching_stats_sentinel.season_phase, actual_pitching_stats[2].season_phase );
     assertEqualsInt( team_pitching_stats_sentinel.wins,         actual_pitching_stats[2].wins         );
     assertEqualsInt( team_pitching_stats_sentinel.losses,       actual_pitching_stats[2].losses       );
     assertEqualsInt( team_pitching_stats_sentinel.games,        actual_pitching_stats[2].games        );
     assertEqualsInt( team_pitching_stats_sentinel.saves,        actual_pitching_stats[2].saves        );
     assertEqualsDbl( team_pitching_stats_sentinel.innings,      actual_pitching_stats[2].innings      );
     assertEqualsInt( team_pitching_stats_sentinel.hits,         actual_pitching_stats[2].hits         );
     assertEqualsInt( team_pitching_stats_sentinel.earned_runs,  actual_pitching_stats[2].earned_runs  );
     assertEqualsInt( team_pitching_stats_sentinel.home_runs,    actual_pitching_stats[2].home_runs    );
     assertEqualsInt( team_pitching_stats_sentinel.walks,        actual_pitching_stats[2].walks        );
     assertEqualsInt( team_pitching_stats_sentinel.strike_outs,  actual_pitching_stats[2].strike_outs  );

     free_team( actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected_pitching_stats1 ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected_pitching_stats2 ) );

     return NULL;
}

static char *get_team__ShouldReturnTheMatchingTeamWithBattingStats_GivenATeamId()
{
     team_s               expected                = { 0 };
     team_batting_stats_s expected_batting_stats1 = { 0 };
     team_batting_stats_s expected_batting_stats2 = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_batting_stats1.team_id        = 1;
     expected_batting_stats1.season         = 1;
     expected_batting_stats1.season_phase   = sp_Regular;
     expected_batting_stats1.games          = 134;
     expected_batting_stats1.at_bats        = 543;
     expected_batting_stats1.runs           = 101;
     expected_batting_stats1.hits           = 220;
     expected_batting_stats1.doubles        = 35;
     expected_batting_stats1.triples        = 16;
     expected_batting_stats1.home_runs      = 56;
     expected_batting_stats1.runs_batted_in = 115;
     expected_batting_stats1.walks          = 62;
     expected_batting_stats1.strike_outs    = 131;
     expected_batting_stats1.steals         = 45;
     expected_batting_stats1.errors         = 5;

     expected_batting_stats2.team_id        = 1;
     expected_batting_stats2.season         = 2;
     expected_batting_stats2.season_phase   = sp_Regular;
     expected_batting_stats2.games          = 143;
     expected_batting_stats2.at_bats        = 555;
     expected_batting_stats2.runs           = 99;
     expected_batting_stats2.hits           = 206;
     expected_batting_stats2.doubles        = 22;
     expected_batting_stats2.triples        = 11;
     expected_batting_stats2.home_runs      = 44;
     expected_batting_stats2.runs_batted_in = 109;
     expected_batting_stats2.walks          = 58;
     expected_batting_stats2.strike_outs    = 144;
     expected_batting_stats2.steals         = 37;
     expected_batting_stats2.errors         = 8;

     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected_batting_stats1 ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected_batting_stats2 ) );

     team_s *actual = get_team( db, expected.team_id );

     assertNotNull( actual );

     team_batting_stats_s *actual_batting_stats = actual->batting_stats;

     assertNotNull( actual_batting_stats );

     assertEquals( expected_batting_stats1.team_id,        actual_batting_stats[0].team_id        );
     assertEquals( expected_batting_stats1.season,         actual_batting_stats[0].season         );
     assertEquals( expected_batting_stats1.season_phase,   actual_batting_stats[0].season_phase   );
     assertEquals( expected_batting_stats1.games,          actual_batting_stats[0].games          );
     assertEquals( expected_batting_stats1.at_bats,        actual_batting_stats[0].at_bats        );
     assertEquals( expected_batting_stats1.runs,           actual_batting_stats[0].runs           );
     assertEquals( expected_batting_stats1.hits,           actual_batting_stats[0].hits           );
     assertEquals( expected_batting_stats1.doubles,        actual_batting_stats[0].doubles        );
     assertEquals( expected_batting_stats1.triples,        actual_batting_stats[0].triples        );
     assertEquals( expected_batting_stats1.home_runs,      actual_batting_stats[0].home_runs      );
     assertEquals( expected_batting_stats1.runs_batted_in, actual_batting_stats[0].runs_batted_in );
     assertEquals( expected_batting_stats1.walks,          actual_batting_stats[0].walks          );
     assertEquals( expected_batting_stats1.strike_outs,    actual_batting_stats[0].strike_outs    );
     assertEquals( expected_batting_stats1.steals,         actual_batting_stats[0].steals         );
     assertEquals( expected_batting_stats1.errors,         actual_batting_stats[0].errors         );

     assertEquals( expected_batting_stats2.team_id,        actual_batting_stats[1].team_id        );
     assertEquals( expected_batting_stats2.season,         actual_batting_stats[1].season         );
     assertEquals( expected_batting_stats2.season_phase,   actual_batting_stats[1].season_phase   );
     assertEquals( expected_batting_stats2.games,          actual_batting_stats[1].games          );
     assertEquals( expected_batting_stats2.at_bats,        actual_batting_stats[1].at_bats        );
     assertEquals( expected_batting_stats2.runs,           actual_batting_stats[1].runs           );
     assertEquals( expected_batting_stats2.hits,           actual_batting_stats[1].hits           );
     assertEquals( expected_batting_stats2.doubles,        actual_batting_stats[1].doubles        );
     assertEquals( expected_batting_stats2.triples,        actual_batting_stats[1].triples        );
     assertEquals( expected_batting_stats2.home_runs,      actual_batting_stats[1].home_runs      );
     assertEquals( expected_batting_stats2.runs_batted_in, actual_batting_stats[1].runs_batted_in );
     assertEquals( expected_batting_stats2.walks,          actual_batting_stats[1].walks          );
     assertEquals( expected_batting_stats2.strike_outs,    actual_batting_stats[1].strike_outs    );
     assertEquals( expected_batting_stats2.steals,         actual_batting_stats[1].steals         );
     assertEquals( expected_batting_stats2.errors,         actual_batting_stats[1].errors         );

     assertEquals( team_batting_stats_sentinel.team_id,        actual_batting_stats[2].team_id        );
     assertEquals( team_batting_stats_sentinel.season,         actual_batting_stats[2].season         );
     assertEquals( team_batting_stats_sentinel.season_phase,   actual_batting_stats[2].season_phase   );
     assertEquals( team_batting_stats_sentinel.games,          actual_batting_stats[2].games          );
     assertEquals( team_batting_stats_sentinel.at_bats,        actual_batting_stats[2].at_bats        );
     assertEquals( team_batting_stats_sentinel.runs,           actual_batting_stats[2].runs           );
     assertEquals( team_batting_stats_sentinel.hits,           actual_batting_stats[2].hits           );
     assertEquals( team_batting_stats_sentinel.doubles,        actual_batting_stats[2].doubles        );
     assertEquals( team_batting_stats_sentinel.triples,        actual_batting_stats[2].triples        );
     assertEquals( team_batting_stats_sentinel.home_runs,      actual_batting_stats[2].home_runs      );
     assertEquals( team_batting_stats_sentinel.runs_batted_in, actual_batting_stats[2].runs_batted_in );
     assertEquals( team_batting_stats_sentinel.walks,          actual_batting_stats[2].walks          );
     assertEquals( team_batting_stats_sentinel.strike_outs,    actual_batting_stats[2].strike_outs    );
     assertEquals( team_batting_stats_sentinel.steals,         actual_batting_stats[2].steals         );
     assertEquals( team_batting_stats_sentinel.errors,         actual_batting_stats[2].errors         );

     free_team( actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected_batting_stats1 ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected_batting_stats2 ) );

     return NULL;
}

static char *get_team__ShouldReturnTheMatchingTeamWithAccolades_GivenATeamId()
{
     team_s          expected           = { 0 };
     team_accolade_s expected_accolade1 = { 0 };
     team_accolade_s expected_accolade2 = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_accolade1.team_id  = 1;
     expected_accolade1.season   = 1;
     expected_accolade1.accolade = tacc_League_Title;

     expected_accolade2.team_id  = 1;
     expected_accolade2.season   = 2;
     expected_accolade2.accolade = tacc_Best_Record;

     assertEquals( SQLITE_OK, team_accolades_t_create( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, team_accolades_t_create( db, &expected_accolade2 ) );

     team_s *actual = get_team( db, expected.team_id );

     assertNotNull( actual );

     team_accolade_s *actual_accolades = actual->accolades;

     assertNotNull( actual_accolades );

     assertEquals( expected_accolade1.team_id,  actual_accolades[0].team_id  );
     assertEquals( expected_accolade1.season,   actual_accolades[0].season   );
     assertEquals( expected_accolade1.accolade, actual_accolades[0].accolade );

     assertEquals( expected_accolade2.team_id,  actual_accolades[1].team_id  );
     assertEquals( expected_accolade2.season,   actual_accolades[1].season   );
     assertEquals( expected_accolade2.accolade, actual_accolades[1].accolade );

     assertEquals( team_accolade_sentinel.team_id,  actual_accolades[2].team_id  );
     assertEquals( team_accolade_sentinel.season,   actual_accolades[2].season   );
     assertEquals( team_accolade_sentinel.accolade, actual_accolades[2].accolade );

     free_team( actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolade1 ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolade2 ) );

     return NULL;
}

static char *save_team__ShouldPersistTheTeamInTheDatabase_GivenATeamObject()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     team_s actual = { 0 };

     actual.team_id = expected.team_id;

     assertEquals( SQLITE_OK, teams_t_read( db, &actual ) );

     assertEqualsInt( expected.team_id,         actual.team_id         );
     assertEqualsStr( expected.name,            actual.name            );
     assertEqualsStr( expected.location,        actual.location        );
     assertEqualsInt( expected.primary_color,   actual.primary_color   );
     assertEqualsInt( expected.secondary_color, actual.secondary_color );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );

     return NULL;
}

static char *save_team__ShouldPersistTheTeamPlayersInTheDatabase_GivenATeamWithTeamPlayers()
{
     team_s        expected                 = { 0 };
     team_player_s expected_team_players[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     expected_team_players[0].team_id   = 1;
     expected_team_players[0].season    = 1;
     expected_team_players[0].player_id = 1;

     expected_team_players[1].team_id   = 1;
     expected_team_players[1].season    = 2;
     expected_team_players[1].player_id = 2;

     expected_team_players[2] = team_player_sentinel;

     expected.players = expected_team_players;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_players_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_player_s *actual_team_players = list.data;

     assertNotNull( actual_team_players );

     assertEquals( expected_team_players[0].team_id,   actual_team_players[0].team_id   );
     assertEquals( expected_team_players[0].season,    actual_team_players[0].season    );
     assertEquals( expected_team_players[0].player_id, actual_team_players[0].player_id );

     assertEquals( expected_team_players[1].team_id,   actual_team_players[1].team_id   );
     assertEquals( expected_team_players[1].season,    actual_team_players[1].season    );
     assertEquals( expected_team_players[1].player_id, actual_team_players[1].player_id );

     free( actual_team_players );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[0] ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[1] ) );

     return NULL;
}

static char *save_team__ShouldPersistTheTeamStatsInTheDatabase_GivenATeamWithTeamStats()
{
     team_s       expected          = { 0 };
     team_stats_s expected_stats[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     expected_stats[0].team_id         = 1;
     expected_stats[0].season          = 1;
     expected_stats[0].season_phase    = sp_Regular;
     expected_stats[0].wins            = 85;
     expected_stats[0].losses          = 67;
     expected_stats[0].home_wins       = 56;
     expected_stats[0].home_losses     = 20;
     expected_stats[0].road_wins       = 29;
     expected_stats[0].road_losses     = 47;
     expected_stats[0].division_wins   = 30;
     expected_stats[0].division_losses = 26;
     expected_stats[0].league_wins     = 52;
     expected_stats[0].league_losses   = 36;
     expected_stats[0].runs_scored     = 234;
     expected_stats[0].runs_allowed    = 222;

     expected_stats[1].team_id         = 1;
     expected_stats[1].season          = 2;
     expected_stats[1].season_phase    = sp_Regular;
     expected_stats[1].wins            = 92;
     expected_stats[1].losses          = 60;
     expected_stats[1].home_wins       = 50;
     expected_stats[1].home_losses     = 26;
     expected_stats[1].road_wins       = 42;
     expected_stats[1].road_losses     = 24;
     expected_stats[1].division_wins   = 36;
     expected_stats[1].division_losses = 20;
     expected_stats[1].league_wins     = 60;
     expected_stats[1].league_losses   = 28;
     expected_stats[1].runs_scored     = 199;
     expected_stats[1].runs_allowed    = 141;

     expected_stats[2] = team_stats_sentinel;

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_stats_s *actual_stats = list.data;

     assertNotNull( actual_stats );

     assertEquals( expected_stats[0].team_id,         actual_stats[0].team_id         );
     assertEquals( expected_stats[0].season,          actual_stats[0].season          );
     assertEquals( expected_stats[0].season_phase,    actual_stats[0].season_phase    );
     assertEquals( expected_stats[0].wins,            actual_stats[0].wins            );
     assertEquals( expected_stats[0].losses,          actual_stats[0].losses          );
     assertEquals( expected_stats[0].home_wins,       actual_stats[0].home_wins       );
     assertEquals( expected_stats[0].home_losses,     actual_stats[0].home_losses     );
     assertEquals( expected_stats[0].road_wins,       actual_stats[0].road_wins       );
     assertEquals( expected_stats[0].road_losses,     actual_stats[0].road_losses     );
     assertEquals( expected_stats[0].division_wins,   actual_stats[0].division_wins   );
     assertEquals( expected_stats[0].division_losses, actual_stats[0].division_losses );
     assertEquals( expected_stats[0].league_wins,     actual_stats[0].league_wins     );
     assertEquals( expected_stats[0].league_losses,   actual_stats[0].league_losses   );
     assertEquals( expected_stats[0].runs_scored,     actual_stats[0].runs_scored     );
     assertEquals( expected_stats[0].runs_allowed,    actual_stats[0].runs_allowed    );

     assertEquals( expected_stats[1].team_id,         actual_stats[1].team_id         );
     assertEquals( expected_stats[1].season,          actual_stats[1].season          );
     assertEquals( expected_stats[1].season_phase,    actual_stats[1].season_phase    );
     assertEquals( expected_stats[1].wins,            actual_stats[1].wins            );
     assertEquals( expected_stats[1].losses,          actual_stats[1].losses          );
     assertEquals( expected_stats[1].home_wins,       actual_stats[1].home_wins       );
     assertEquals( expected_stats[1].home_losses,     actual_stats[1].home_losses     );
     assertEquals( expected_stats[1].road_wins,       actual_stats[1].road_wins       );
     assertEquals( expected_stats[1].road_losses,     actual_stats[1].road_losses     );
     assertEquals( expected_stats[1].division_wins,   actual_stats[1].division_wins   );
     assertEquals( expected_stats[1].division_losses, actual_stats[1].division_losses );
     assertEquals( expected_stats[1].league_wins,     actual_stats[1].league_wins     );
     assertEquals( expected_stats[1].league_losses,   actual_stats[1].league_losses   );
     assertEquals( expected_stats[1].runs_scored,     actual_stats[1].runs_scored     );
     assertEquals( expected_stats[1].runs_allowed,    actual_stats[1].runs_allowed    );

     free( actual_stats );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats[1] ) );

     return NULL;
}

static char *save_team__ShouldPersistThePitchingStatsInTheDatabase_GivenATeamWithPitchingStats()
{
     team_s                expected                   = { 0 };
     team_pitching_stats_s expected_pitching_stats[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     expected_pitching_stats[0].team_id      = 1;
     expected_pitching_stats[0].season       = 1;
     expected_pitching_stats[0].season_phase = sp_Regular;
     expected_pitching_stats[0].wins         = 12;
     expected_pitching_stats[0].losses       = 15;
     expected_pitching_stats[0].games        = 54;
     expected_pitching_stats[0].saves        = 11;
     expected_pitching_stats[0].innings      = 321.2;
     expected_pitching_stats[0].hits         = 165;
     expected_pitching_stats[0].earned_runs  = 23;
     expected_pitching_stats[0].home_runs    = 14;
     expected_pitching_stats[0].walks        = 25;
     expected_pitching_stats[0].strike_outs  = 234;

     expected_pitching_stats[1].team_id      = 1;
     expected_pitching_stats[1].season       = 2;
     expected_pitching_stats[1].season_phase = sp_Regular;
     expected_pitching_stats[1].wins         = 21;
     expected_pitching_stats[1].losses       = 51;
     expected_pitching_stats[1].games        = 45;
     expected_pitching_stats[1].saves        = 99;
     expected_pitching_stats[1].innings      = 123.1;
     expected_pitching_stats[1].hits         = 156;
     expected_pitching_stats[1].earned_runs  = 32;
     expected_pitching_stats[1].home_runs    = 41;
     expected_pitching_stats[1].walks        = 52;
     expected_pitching_stats[1].strike_outs  = 321;

     expected_pitching_stats[2] = team_pitching_stats_sentinel;

     expected.pitching_stats = expected_pitching_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_pitching_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_pitching_stats_s *actual_pitching_stats = list.data;

     assertNotNull( actual_pitching_stats );

     assertEqualsInt( expected_pitching_stats[0].team_id,      actual_pitching_stats[0].team_id      );
     assertEqualsInt( expected_pitching_stats[0].season,       actual_pitching_stats[0].season       );
     assertEqualsInt( expected_pitching_stats[0].season_phase, actual_pitching_stats[0].season_phase );
     assertEqualsInt( expected_pitching_stats[0].wins,         actual_pitching_stats[0].wins         );
     assertEqualsInt( expected_pitching_stats[0].losses,       actual_pitching_stats[0].losses       );
     assertEqualsInt( expected_pitching_stats[0].games,        actual_pitching_stats[0].games        );
     assertEqualsInt( expected_pitching_stats[0].saves,        actual_pitching_stats[0].saves        );
     assertEqualsDbl( expected_pitching_stats[0].innings,      actual_pitching_stats[0].innings      );
     assertEqualsInt( expected_pitching_stats[0].hits,         actual_pitching_stats[0].hits         );
     assertEqualsInt( expected_pitching_stats[0].earned_runs,  actual_pitching_stats[0].earned_runs  );
     assertEqualsInt( expected_pitching_stats[0].home_runs,    actual_pitching_stats[0].home_runs    );
     assertEqualsInt( expected_pitching_stats[0].walks,        actual_pitching_stats[0].walks        );
     assertEqualsInt( expected_pitching_stats[0].strike_outs,  actual_pitching_stats[0].strike_outs  );

     assertEqualsInt( expected_pitching_stats[1].team_id,      actual_pitching_stats[1].team_id      );
     assertEqualsInt( expected_pitching_stats[1].season,       actual_pitching_stats[1].season       );
     assertEqualsInt( expected_pitching_stats[1].season_phase, actual_pitching_stats[1].season_phase );
     assertEqualsInt( expected_pitching_stats[1].wins,         actual_pitching_stats[1].wins         );
     assertEqualsInt( expected_pitching_stats[1].losses,       actual_pitching_stats[1].losses       );
     assertEqualsInt( expected_pitching_stats[1].games,        actual_pitching_stats[1].games        );
     assertEqualsInt( expected_pitching_stats[1].saves,        actual_pitching_stats[1].saves        );
     assertEqualsDbl( expected_pitching_stats[1].innings,      actual_pitching_stats[1].innings      );
     assertEqualsInt( expected_pitching_stats[1].hits,         actual_pitching_stats[1].hits         );
     assertEqualsInt( expected_pitching_stats[1].earned_runs,  actual_pitching_stats[1].earned_runs  );
     assertEqualsInt( expected_pitching_stats[1].home_runs,    actual_pitching_stats[1].home_runs    );
     assertEqualsInt( expected_pitching_stats[1].walks,        actual_pitching_stats[1].walks        );
     assertEqualsInt( expected_pitching_stats[1].strike_outs,  actual_pitching_stats[1].strike_outs  );

     free( actual_pitching_stats );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected_pitching_stats[0] ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected_pitching_stats[1] ) );

     return NULL;
}

static char *save_team__ShouldPersistTheBattingStatsInTheDatabase_GivenATeamWithBattingStats()
{
     team_s               expected                  = { 0 };
     team_batting_stats_s expected_batting_stats[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     expected_batting_stats[0].team_id        = 1;
     expected_batting_stats[0].season         = 1;
     expected_batting_stats[0].season_phase   = sp_Regular;
     expected_batting_stats[0].games          = 134;
     expected_batting_stats[0].at_bats        = 543;
     expected_batting_stats[0].runs           = 101;
     expected_batting_stats[0].hits           = 220;
     expected_batting_stats[0].doubles        = 35;
     expected_batting_stats[0].triples        = 16;
     expected_batting_stats[0].home_runs      = 56;
     expected_batting_stats[0].runs_batted_in = 115;
     expected_batting_stats[0].walks          = 62;
     expected_batting_stats[0].strike_outs    = 131;
     expected_batting_stats[0].steals         = 45;
     expected_batting_stats[0].errors         = 5;

     expected_batting_stats[1].team_id        = 1;
     expected_batting_stats[1].season         = 2;
     expected_batting_stats[1].season_phase   = sp_Regular;
     expected_batting_stats[1].games          = 143;
     expected_batting_stats[1].at_bats        = 555;
     expected_batting_stats[1].runs           = 99;
     expected_batting_stats[1].hits           = 206;
     expected_batting_stats[1].doubles        = 22;
     expected_batting_stats[1].triples        = 11;
     expected_batting_stats[1].home_runs      = 44;
     expected_batting_stats[1].runs_batted_in = 109;
     expected_batting_stats[1].walks          = 58;
     expected_batting_stats[1].strike_outs    = 144;
     expected_batting_stats[1].steals         = 37;
     expected_batting_stats[1].errors         = 8;

     expected_batting_stats[2] = team_batting_stats_sentinel;

     expected.batting_stats = expected_batting_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_batting_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_batting_stats_s *actual_batting_stats = list.data;

     assertNotNull( actual_batting_stats );

     assertEquals( expected_batting_stats[0].team_id,        actual_batting_stats[0].team_id        );
     assertEquals( expected_batting_stats[0].season,         actual_batting_stats[0].season         );
     assertEquals( expected_batting_stats[0].season_phase,   actual_batting_stats[0].season_phase   );
     assertEquals( expected_batting_stats[0].games,          actual_batting_stats[0].games          );
     assertEquals( expected_batting_stats[0].at_bats,        actual_batting_stats[0].at_bats        );
     assertEquals( expected_batting_stats[0].runs,           actual_batting_stats[0].runs           );
     assertEquals( expected_batting_stats[0].hits,           actual_batting_stats[0].hits           );
     assertEquals( expected_batting_stats[0].doubles,        actual_batting_stats[0].doubles        );
     assertEquals( expected_batting_stats[0].triples,        actual_batting_stats[0].triples        );
     assertEquals( expected_batting_stats[0].home_runs,      actual_batting_stats[0].home_runs      );
     assertEquals( expected_batting_stats[0].runs_batted_in, actual_batting_stats[0].runs_batted_in );
     assertEquals( expected_batting_stats[0].walks,          actual_batting_stats[0].walks          );
     assertEquals( expected_batting_stats[0].strike_outs,    actual_batting_stats[0].strike_outs    );
     assertEquals( expected_batting_stats[0].steals,         actual_batting_stats[0].steals         );
     assertEquals( expected_batting_stats[0].errors,         actual_batting_stats[0].errors         );

     assertEquals( expected_batting_stats[1].team_id,        actual_batting_stats[1].team_id        );
     assertEquals( expected_batting_stats[1].season,         actual_batting_stats[1].season         );
     assertEquals( expected_batting_stats[1].season_phase,   actual_batting_stats[1].season_phase   );
     assertEquals( expected_batting_stats[1].games,          actual_batting_stats[1].games          );
     assertEquals( expected_batting_stats[1].at_bats,        actual_batting_stats[1].at_bats        );
     assertEquals( expected_batting_stats[1].runs,           actual_batting_stats[1].runs           );
     assertEquals( expected_batting_stats[1].hits,           actual_batting_stats[1].hits           );
     assertEquals( expected_batting_stats[1].doubles,        actual_batting_stats[1].doubles        );
     assertEquals( expected_batting_stats[1].triples,        actual_batting_stats[1].triples        );
     assertEquals( expected_batting_stats[1].home_runs,      actual_batting_stats[1].home_runs      );
     assertEquals( expected_batting_stats[1].runs_batted_in, actual_batting_stats[1].runs_batted_in );
     assertEquals( expected_batting_stats[1].walks,          actual_batting_stats[1].walks          );
     assertEquals( expected_batting_stats[1].strike_outs,    actual_batting_stats[1].strike_outs    );
     assertEquals( expected_batting_stats[1].steals,         actual_batting_stats[1].steals         );
     assertEquals( expected_batting_stats[1].errors,         actual_batting_stats[1].errors         );

     free( actual_batting_stats );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected_batting_stats[0] ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected_batting_stats[1] ) );

     return NULL;
}

static char *save_team__ShouldPersistTheAccoladesInTheDatabase_GivenATeamWithAccolades()
{
     team_s          expected              = { 0 };
     team_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     expected_accolades[0].team_id  = 1;
     expected_accolades[0].season   = 1;
     expected_accolades[0].accolade = tacc_League_Title;

     expected_accolades[1].team_id  = 1;
     expected_accolades[1].season   = 2;
     expected_accolades[1].accolade = tacc_Best_Record;

     expected_accolades[2] = team_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_accolades_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_accolade_s *actual_accolades = list.data;

     assertNotNull( actual_accolades );

     assertEquals( expected_accolades[0].team_id,  actual_accolades[0].team_id  );
     assertEquals( expected_accolades[0].season,   actual_accolades[0].season   );
     assertEquals( expected_accolades[0].accolade, actual_accolades[0].accolade );

     assertEquals( expected_accolades[1].team_id,  actual_accolades[1].team_id  );
     assertEquals( expected_accolades[1].season,   actual_accolades[1].season   );
     assertEquals( expected_accolades[1].accolade, actual_accolades[1].accolade );

     free( actual_accolades );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *save_team__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenATeam()
{
     team_s                expected                   = { 0 };
     team_player_s         expected_team_players[3]   = { 0 };
     team_stats_s          expected_stats[3]          = { 0 };
     team_pitching_stats_s expected_pitching_stats[3] = { 0 };
     team_batting_stats_s  expected_batting_stats[3]  = { 0 };
     team_accolade_s       expected_accolades[3]      = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     expected_team_players[0].team_id   = 1;
     expected_team_players[0].season    = 1;
     expected_team_players[0].player_id = 1;

     expected_team_players[1].team_id   = 1;
     expected_team_players[1].season    = 2;
     expected_team_players[1].player_id = 2;

     expected_team_players[2] = team_player_sentinel;

     expected_stats[0].team_id         = 1;
     expected_stats[0].season          = 1;
     expected_stats[0].season_phase    = sp_Regular;
     expected_stats[0].wins            = 85;
     expected_stats[0].losses          = 67;
     expected_stats[0].home_wins       = 56;
     expected_stats[0].home_losses     = 20;
     expected_stats[0].road_wins       = 29;
     expected_stats[0].road_losses     = 47;
     expected_stats[0].division_wins   = 30;
     expected_stats[0].division_losses = 26;
     expected_stats[0].league_wins     = 52;
     expected_stats[0].league_losses   = 36;
     expected_stats[0].runs_scored     = 234;
     expected_stats[0].runs_allowed    = 222;

     expected_stats[1].team_id         = 1;
     expected_stats[1].season          = 2;
     expected_stats[1].season_phase    = sp_Regular;
     expected_stats[1].wins            = 92;
     expected_stats[1].losses          = 60;
     expected_stats[1].home_wins       = 50;
     expected_stats[1].home_losses     = 26;
     expected_stats[1].road_wins       = 42;
     expected_stats[1].road_losses     = 24;
     expected_stats[1].division_wins   = 36;
     expected_stats[1].division_losses = 20;
     expected_stats[1].league_wins     = 60;
     expected_stats[1].league_losses   = 28;
     expected_stats[1].runs_scored     = 199;
     expected_stats[1].runs_allowed    = 141;

     expected_stats[2] = team_stats_sentinel;

     expected_pitching_stats[0].team_id      = 1;
     expected_pitching_stats[0].season       = 1;
     expected_pitching_stats[0].season_phase = sp_Regular;
     expected_pitching_stats[0].wins         = 12;
     expected_pitching_stats[0].losses       = 15;
     expected_pitching_stats[0].games        = 54;
     expected_pitching_stats[0].saves        = 11;
     expected_pitching_stats[0].innings      = 321.2;
     expected_pitching_stats[0].hits         = 165;
     expected_pitching_stats[0].earned_runs  = 23;
     expected_pitching_stats[0].home_runs    = 14;
     expected_pitching_stats[0].walks        = 25;
     expected_pitching_stats[0].strike_outs  = 234;

     expected_pitching_stats[1].team_id      = 1;
     expected_pitching_stats[1].season       = 2;
     expected_pitching_stats[1].season_phase = sp_Regular;
     expected_pitching_stats[1].wins         = 21;
     expected_pitching_stats[1].losses       = 51;
     expected_pitching_stats[1].games        = 45;
     expected_pitching_stats[1].saves        = 99;
     expected_pitching_stats[1].innings      = 123.1;
     expected_pitching_stats[1].hits         = 156;
     expected_pitching_stats[1].earned_runs  = 32;
     expected_pitching_stats[1].home_runs    = 41;
     expected_pitching_stats[1].walks        = 52;
     expected_pitching_stats[1].strike_outs  = 321;

     expected_pitching_stats[2] = team_pitching_stats_sentinel;

     expected_batting_stats[0].team_id        = 1;
     expected_batting_stats[0].season         = 1;
     expected_batting_stats[0].season_phase   = sp_Regular;
     expected_batting_stats[0].games          = 134;
     expected_batting_stats[0].at_bats        = 543;
     expected_batting_stats[0].runs           = 101;
     expected_batting_stats[0].hits           = 220;
     expected_batting_stats[0].doubles        = 35;
     expected_batting_stats[0].triples        = 16;
     expected_batting_stats[0].home_runs      = 56;
     expected_batting_stats[0].runs_batted_in = 115;
     expected_batting_stats[0].walks          = 62;
     expected_batting_stats[0].strike_outs    = 131;
     expected_batting_stats[0].steals         = 45;
     expected_batting_stats[0].errors         = 5;

     expected_batting_stats[1].team_id        = 1;
     expected_batting_stats[1].season         = 2;
     expected_batting_stats[1].season_phase   = sp_Regular;
     expected_batting_stats[1].games          = 143;
     expected_batting_stats[1].at_bats        = 555;
     expected_batting_stats[1].runs           = 99;
     expected_batting_stats[1].hits           = 206;
     expected_batting_stats[1].doubles        = 22;
     expected_batting_stats[1].triples        = 11;
     expected_batting_stats[1].home_runs      = 44;
     expected_batting_stats[1].runs_batted_in = 109;
     expected_batting_stats[1].walks          = 58;
     expected_batting_stats[1].strike_outs    = 144;
     expected_batting_stats[1].steals         = 37;
     expected_batting_stats[1].errors         = 8;

     expected_batting_stats[2] = team_batting_stats_sentinel;

     expected_accolades[0].team_id  = 1;
     expected_accolades[0].season   = 1;
     expected_accolades[0].accolade = tacc_League_Title;

     expected_accolades[1].team_id  = 1;
     expected_accolades[1].season   = 2;
     expected_accolades[1].accolade = tacc_Best_Record;

     expected_accolades[2] = team_accolade_sentinel;

     expected.players        = expected_team_players;
     expected.stats          = expected_stats;
     expected.pitching_stats = expected_pitching_stats;
     expected.batting_stats  = expected_batting_stats;
     expected.accolades      = expected_accolades;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &expected_team_players[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected_pitching_stats[0] ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected_batting_stats[0] ) );
     assertEquals( SQLITE_OK, team_accolades_t_create( db, &expected_accolades[0] ) );

     strcpy( expected.name,     "TeamName2" );
     strcpy( expected.location, "Location2" );

     expected_stats[0].wins   = 83;
     expected_stats[0].losses = 69;

     expected_pitching_stats[0].wins   = 24;
     expected_pitching_stats[0].losses = 13;

     expected_batting_stats[0].at_bats = 538;
     expected_batting_stats[0].hits    = 216;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     team_s actual = { 0 };

     actual.team_id = expected.team_id;

     assertEquals( SQLITE_OK, teams_t_read( db, &actual ) );

     assertEqualsInt( expected.team_id,         actual.team_id         );
     assertEqualsStr( expected.name,            actual.name            );
     assertEqualsStr( expected.location,        actual.location        );
     assertEqualsInt( expected.primary_color,   actual.primary_color   );
     assertEqualsInt( expected.secondary_color, actual.secondary_color );

     data_list_s team_players_list = { 0 };

     assertEquals( SQLITE_OK, team_players_t_read_by_team( db, expected.team_id, &team_players_list ) );

     assertEquals( 2, team_players_list.count );

     team_player_s *actual_team_players = team_players_list.data;

     assertNotNull( actual_team_players );

     assertEquals( expected_team_players[0].team_id,   actual_team_players[0].team_id   );
     assertEquals( expected_team_players[0].season,    actual_team_players[0].season    );
     assertEquals( expected_team_players[0].player_id, actual_team_players[0].player_id );

     assertEquals( expected_team_players[1].team_id,   actual_team_players[1].team_id   );
     assertEquals( expected_team_players[1].season,    actual_team_players[1].season    );
     assertEquals( expected_team_players[1].player_id, actual_team_players[1].player_id );

     data_list_s stats_list = { 0 };

     assertEquals( SQLITE_OK, team_stats_t_read_by_team( db, expected.team_id, &stats_list ) );

     assertEquals( 2, stats_list.count );

     team_stats_s *actual_stats = stats_list.data;

     assertNotNull( actual_stats );

     assertEquals( expected_stats[0].team_id,         actual_stats[0].team_id         );
     assertEquals( expected_stats[0].season,          actual_stats[0].season          );
     assertEquals( expected_stats[0].season_phase,    actual_stats[0].season_phase    );
     assertEquals( expected_stats[0].wins,            actual_stats[0].wins            );
     assertEquals( expected_stats[0].losses,          actual_stats[0].losses          );
     assertEquals( expected_stats[0].home_wins,       actual_stats[0].home_wins       );
     assertEquals( expected_stats[0].home_losses,     actual_stats[0].home_losses     );
     assertEquals( expected_stats[0].road_wins,       actual_stats[0].road_wins       );
     assertEquals( expected_stats[0].road_losses,     actual_stats[0].road_losses     );
     assertEquals( expected_stats[0].division_wins,   actual_stats[0].division_wins   );
     assertEquals( expected_stats[0].division_losses, actual_stats[0].division_losses );
     assertEquals( expected_stats[0].league_wins,     actual_stats[0].league_wins     );
     assertEquals( expected_stats[0].league_losses,   actual_stats[0].league_losses   );
     assertEquals( expected_stats[0].runs_scored,     actual_stats[0].runs_scored     );
     assertEquals( expected_stats[0].runs_allowed,    actual_stats[0].runs_allowed    );

     assertEquals( expected_stats[1].team_id,         actual_stats[1].team_id         );
     assertEquals( expected_stats[1].season,          actual_stats[1].season          );
     assertEquals( expected_stats[1].season_phase,    actual_stats[1].season_phase    );
     assertEquals( expected_stats[1].wins,            actual_stats[1].wins            );
     assertEquals( expected_stats[1].losses,          actual_stats[1].losses          );
     assertEquals( expected_stats[1].home_wins,       actual_stats[1].home_wins       );
     assertEquals( expected_stats[1].home_losses,     actual_stats[1].home_losses     );
     assertEquals( expected_stats[1].road_wins,       actual_stats[1].road_wins       );
     assertEquals( expected_stats[1].road_losses,     actual_stats[1].road_losses     );
     assertEquals( expected_stats[1].division_wins,   actual_stats[1].division_wins   );
     assertEquals( expected_stats[1].division_losses, actual_stats[1].division_losses );
     assertEquals( expected_stats[1].league_wins,     actual_stats[1].league_wins     );
     assertEquals( expected_stats[1].league_losses,   actual_stats[1].league_losses   );
     assertEquals( expected_stats[1].runs_scored,     actual_stats[1].runs_scored     );
     assertEquals( expected_stats[1].runs_allowed,    actual_stats[1].runs_allowed    );

     data_list_s pitching_stats_list = { 0 };

     assertEquals( SQLITE_OK, team_pitching_stats_t_read_by_team( db, expected.team_id, &pitching_stats_list ) );

     assertEquals( 2, pitching_stats_list.count );

     team_pitching_stats_s *actual_pitching_stats = pitching_stats_list.data;

     assertNotNull( actual_pitching_stats );

     assertEqualsInt( expected_pitching_stats[0].team_id,      actual_pitching_stats[0].team_id      );
     assertEqualsInt( expected_pitching_stats[0].season,       actual_pitching_stats[0].season       );
     assertEqualsInt( expected_pitching_stats[0].season_phase, actual_pitching_stats[0].season_phase );
     assertEqualsInt( expected_pitching_stats[0].wins,         actual_pitching_stats[0].wins         );
     assertEqualsInt( expected_pitching_stats[0].losses,       actual_pitching_stats[0].losses       );
     assertEqualsInt( expected_pitching_stats[0].games,        actual_pitching_stats[0].games        );
     assertEqualsInt( expected_pitching_stats[0].saves,        actual_pitching_stats[0].saves        );
     assertEqualsDbl( expected_pitching_stats[0].innings,      actual_pitching_stats[0].innings      );
     assertEqualsInt( expected_pitching_stats[0].hits,         actual_pitching_stats[0].hits         );
     assertEqualsInt( expected_pitching_stats[0].earned_runs,  actual_pitching_stats[0].earned_runs  );
     assertEqualsInt( expected_pitching_stats[0].home_runs,    actual_pitching_stats[0].home_runs    );
     assertEqualsInt( expected_pitching_stats[0].walks,        actual_pitching_stats[0].walks        );
     assertEqualsInt( expected_pitching_stats[0].strike_outs,  actual_pitching_stats[0].strike_outs  );

     assertEqualsInt( expected_pitching_stats[1].team_id,      actual_pitching_stats[1].team_id      );
     assertEqualsInt( expected_pitching_stats[1].season,       actual_pitching_stats[1].season       );
     assertEqualsInt( expected_pitching_stats[1].season_phase, actual_pitching_stats[1].season_phase );
     assertEqualsInt( expected_pitching_stats[1].wins,         actual_pitching_stats[1].wins         );
     assertEqualsInt( expected_pitching_stats[1].losses,       actual_pitching_stats[1].losses       );
     assertEqualsInt( expected_pitching_stats[1].games,        actual_pitching_stats[1].games        );
     assertEqualsInt( expected_pitching_stats[1].saves,        actual_pitching_stats[1].saves        );
     assertEqualsDbl( expected_pitching_stats[1].innings,      actual_pitching_stats[1].innings      );
     assertEqualsInt( expected_pitching_stats[1].hits,         actual_pitching_stats[1].hits         );
     assertEqualsInt( expected_pitching_stats[1].earned_runs,  actual_pitching_stats[1].earned_runs  );
     assertEqualsInt( expected_pitching_stats[1].home_runs,    actual_pitching_stats[1].home_runs    );
     assertEqualsInt( expected_pitching_stats[1].walks,        actual_pitching_stats[1].walks        );
     assertEqualsInt( expected_pitching_stats[1].strike_outs,  actual_pitching_stats[1].strike_outs  );

     data_list_s batting_stats_list = { 0 };

     assertEquals( SQLITE_OK, team_batting_stats_t_read_by_team( db, expected.team_id, &batting_stats_list ) );

     assertEquals( 2, batting_stats_list.count );

     team_batting_stats_s *actual_batting_stats = batting_stats_list.data;

     assertNotNull( actual_batting_stats );

     assertEquals( expected_batting_stats[0].team_id,        actual_batting_stats[0].team_id        );
     assertEquals( expected_batting_stats[0].season,         actual_batting_stats[0].season         );
     assertEquals( expected_batting_stats[0].season_phase,   actual_batting_stats[0].season_phase   );
     assertEquals( expected_batting_stats[0].games,          actual_batting_stats[0].games          );
     assertEquals( expected_batting_stats[0].at_bats,        actual_batting_stats[0].at_bats        );
     assertEquals( expected_batting_stats[0].runs,           actual_batting_stats[0].runs           );
     assertEquals( expected_batting_stats[0].hits,           actual_batting_stats[0].hits           );
     assertEquals( expected_batting_stats[0].doubles,        actual_batting_stats[0].doubles        );
     assertEquals( expected_batting_stats[0].triples,        actual_batting_stats[0].triples        );
     assertEquals( expected_batting_stats[0].home_runs,      actual_batting_stats[0].home_runs      );
     assertEquals( expected_batting_stats[0].runs_batted_in, actual_batting_stats[0].runs_batted_in );
     assertEquals( expected_batting_stats[0].walks,          actual_batting_stats[0].walks          );
     assertEquals( expected_batting_stats[0].strike_outs,    actual_batting_stats[0].strike_outs    );
     assertEquals( expected_batting_stats[0].steals,         actual_batting_stats[0].steals         );
     assertEquals( expected_batting_stats[0].errors,         actual_batting_stats[0].errors         );

     assertEquals( expected_batting_stats[1].team_id,        actual_batting_stats[1].team_id        );
     assertEquals( expected_batting_stats[1].season,         actual_batting_stats[1].season         );
     assertEquals( expected_batting_stats[1].season_phase,   actual_batting_stats[1].season_phase   );
     assertEquals( expected_batting_stats[1].games,          actual_batting_stats[1].games          );
     assertEquals( expected_batting_stats[1].at_bats,        actual_batting_stats[1].at_bats        );
     assertEquals( expected_batting_stats[1].runs,           actual_batting_stats[1].runs           );
     assertEquals( expected_batting_stats[1].hits,           actual_batting_stats[1].hits           );
     assertEquals( expected_batting_stats[1].doubles,        actual_batting_stats[1].doubles        );
     assertEquals( expected_batting_stats[1].triples,        actual_batting_stats[1].triples        );
     assertEquals( expected_batting_stats[1].home_runs,      actual_batting_stats[1].home_runs      );
     assertEquals( expected_batting_stats[1].runs_batted_in, actual_batting_stats[1].runs_batted_in );
     assertEquals( expected_batting_stats[1].walks,          actual_batting_stats[1].walks          );
     assertEquals( expected_batting_stats[1].strike_outs,    actual_batting_stats[1].strike_outs    );
     assertEquals( expected_batting_stats[1].steals,         actual_batting_stats[1].steals         );
     assertEquals( expected_batting_stats[1].errors,         actual_batting_stats[1].errors         );

     data_list_s accolade_list = { 0 };

     assertEquals( SQLITE_OK, team_accolades_t_read_by_team( db, expected.team_id, &accolade_list ) );

     assertEquals( 2, accolade_list.count );

     team_accolade_s *actual_accolades = accolade_list.data;

     assertNotNull( actual_accolades );

     assertEquals( expected_accolades[0].team_id,  actual_accolades[0].team_id  );
     assertEquals( expected_accolades[0].season,   actual_accolades[0].season   );
     assertEquals( expected_accolades[0].accolade, actual_accolades[0].accolade );

     assertEquals( expected_accolades[1].team_id,  actual_accolades[1].team_id  );
     assertEquals( expected_accolades[1].season,   actual_accolades[1].season   );
     assertEquals( expected_accolades[1].accolade, actual_accolades[1].accolade );

     free( actual_team_players );
     free( actual_stats );
     free( actual_pitching_stats );
     free( actual_batting_stats );
     free( actual_accolades );

     assertEquals( SQLITE_OK, teams_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[0] ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[1] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats[1] ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected_pitching_stats[0] ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_delete( db, &expected_pitching_stats[1] ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected_batting_stats[0] ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_delete( db, &expected_batting_stats[1] ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolades[1] ) );

     return NULL;
}

static char *remove_team__ShouldRemoveTheTeamFromTheDatabase_GivenATeamObject()
{
     team_s expected = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     assertEquals( SQLITE_OK, remove_team( db, &expected ) );

     team_s actual = { 0 };

     actual.team_id = expected.team_id;

     assertEquals( SQLITE_NOTFOUND, teams_t_read( db, &actual ) );

     return NULL;
}

static char *remove_team__ShouldRemoveTheTeamPlayersFromTheDatabase_GivenATeamWithTeamPlayers()
{
     team_s        expected                 = { 0 };
     team_player_s expected_team_players[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_team_players[0].team_id   = 1;
     expected_team_players[0].season    = 1;
     expected_team_players[0].player_id = 1;

     expected_team_players[1].team_id   = 1;
     expected_team_players[1].season    = 2;
     expected_team_players[1].player_id = 2;

     expected_team_players[2] = team_player_sentinel;

     assertEquals( SQLITE_OK, team_players_t_create( db, &expected_team_players[0] ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &expected_team_players[1] ) );

     expected.players = expected_team_players;

     assertEquals( SQLITE_OK, remove_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, team_players_t_read_by_team( db, expected.team_id, &list ) );

     return NULL;
}

static char *remove_team__ShouldRemoveTheTeamStatsFromTheDatabase_GivenATeamWithTeamStats()
{
     team_s       expected          = { 0 };
     team_stats_s expected_stats[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_stats[0].team_id         = 1;
     expected_stats[0].season          = 1;
     expected_stats[0].season_phase    = sp_Regular;
     expected_stats[0].wins            = 85;
     expected_stats[0].losses          = 67;
     expected_stats[0].home_wins       = 56;
     expected_stats[0].home_losses     = 20;
     expected_stats[0].road_wins       = 29;
     expected_stats[0].road_losses     = 47;
     expected_stats[0].division_wins   = 30;
     expected_stats[0].division_losses = 26;
     expected_stats[0].league_wins     = 52;
     expected_stats[0].league_losses   = 36;
     expected_stats[0].runs_scored     = 234;
     expected_stats[0].runs_allowed    = 222;

     expected_stats[1].team_id         = 1;
     expected_stats[1].season          = 2;
     expected_stats[1].season_phase    = sp_Regular;
     expected_stats[1].wins            = 92;
     expected_stats[1].losses          = 60;
     expected_stats[1].home_wins       = 50;
     expected_stats[1].home_losses     = 26;
     expected_stats[1].road_wins       = 42;
     expected_stats[1].road_losses     = 24;
     expected_stats[1].division_wins   = 36;
     expected_stats[1].division_losses = 20;
     expected_stats[1].league_wins     = 60;
     expected_stats[1].league_losses   = 28;
     expected_stats[1].runs_scored     = 199;
     expected_stats[1].runs_allowed    = 141;

     expected_stats[2] = team_stats_sentinel;

     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected_stats[1] ) );

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, remove_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, team_stats_t_read_by_team( db, expected.team_id, &list ) );

     return NULL;
}

static char *remove_team__ShouldRemoveTheTeamPitchingStatsFromTheDatabase_GivenATeamWithTeamPitchingStats()
{
     team_s                expected                   = { 0 };
     team_pitching_stats_s expected_pitching_stats[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_pitching_stats[0].team_id      = 1;
     expected_pitching_stats[0].season       = 1;
     expected_pitching_stats[0].season_phase = sp_Regular;
     expected_pitching_stats[0].wins         = 12;
     expected_pitching_stats[0].losses       = 15;
     expected_pitching_stats[0].games        = 54;
     expected_pitching_stats[0].saves        = 11;
     expected_pitching_stats[0].innings      = 321.2;
     expected_pitching_stats[0].hits         = 165;
     expected_pitching_stats[0].earned_runs  = 23;
     expected_pitching_stats[0].home_runs    = 14;
     expected_pitching_stats[0].walks        = 25;
     expected_pitching_stats[0].strike_outs  = 234;

     expected_pitching_stats[1].team_id      = 1;
     expected_pitching_stats[1].season       = 2;
     expected_pitching_stats[1].season_phase = sp_Regular;
     expected_pitching_stats[1].wins         = 21;
     expected_pitching_stats[1].losses       = 51;
     expected_pitching_stats[1].games        = 45;
     expected_pitching_stats[1].saves        = 99;
     expected_pitching_stats[1].innings      = 123.1;
     expected_pitching_stats[1].hits         = 156;
     expected_pitching_stats[1].earned_runs  = 32;
     expected_pitching_stats[1].home_runs    = 41;
     expected_pitching_stats[1].walks        = 52;
     expected_pitching_stats[1].strike_outs  = 321;

     expected_pitching_stats[2] = team_pitching_stats_sentinel;

     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected_pitching_stats[0] ) );
     assertEquals( SQLITE_OK, team_pitching_stats_t_create( db, &expected_pitching_stats[1] ) );

     expected.pitching_stats = expected_pitching_stats;

     assertEquals( SQLITE_OK, remove_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, team_pitching_stats_t_read_by_team( db, expected.team_id, &list ) );

     return NULL;
}

static char *remove_team__ShouldRemoveTheTeamBattingStatsFromTheDatabase_GivenATeamWithTeamBattingStats()
{
     team_s               expected                  = { 0 };
     team_batting_stats_s expected_batting_stats[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_batting_stats[0].team_id        = 1;
     expected_batting_stats[0].season         = 1;
     expected_batting_stats[0].season_phase   = sp_Regular;
     expected_batting_stats[0].games          = 134;
     expected_batting_stats[0].at_bats        = 543;
     expected_batting_stats[0].runs           = 101;
     expected_batting_stats[0].hits           = 220;
     expected_batting_stats[0].doubles        = 35;
     expected_batting_stats[0].triples        = 16;
     expected_batting_stats[0].home_runs      = 56;
     expected_batting_stats[0].runs_batted_in = 115;
     expected_batting_stats[0].walks          = 62;
     expected_batting_stats[0].strike_outs    = 131;
     expected_batting_stats[0].steals         = 45;
     expected_batting_stats[0].errors         = 5;

     expected_batting_stats[1].team_id        = 1;
     expected_batting_stats[1].season         = 2;
     expected_batting_stats[1].season_phase   = sp_Regular;
     expected_batting_stats[1].games          = 143;
     expected_batting_stats[1].at_bats        = 555;
     expected_batting_stats[1].runs           = 99;
     expected_batting_stats[1].hits           = 206;
     expected_batting_stats[1].doubles        = 22;
     expected_batting_stats[1].triples        = 11;
     expected_batting_stats[1].home_runs      = 44;
     expected_batting_stats[1].runs_batted_in = 109;
     expected_batting_stats[1].walks          = 58;
     expected_batting_stats[1].strike_outs    = 144;
     expected_batting_stats[1].steals         = 37;
     expected_batting_stats[1].errors         = 8;

     expected_batting_stats[2] = team_batting_stats_sentinel;

     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected_batting_stats[0] ) );
     assertEquals( SQLITE_OK, team_batting_stats_t_create( db, &expected_batting_stats[1] ) );

     expected.batting_stats = expected_batting_stats;

     assertEquals( SQLITE_OK, remove_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, team_batting_stats_t_read_by_team( db, expected.team_id, &list ) );

     return NULL;
}

static char *remove_team__ShouldRemoveTheAccoladesFromTheDatabase_GivenATeamWithAccolades()
{
     team_s          expected              = { 0 };
     team_accolade_s expected_accolades[3] = { 0 };

     /**/    expected.team_id         = 1;
     strcpy( expected.name,            "TeamName1" );
     strcpy( expected.location,        "Location1" );
     /**/    expected.primary_color   = cl_Blue;
     /**/    expected.secondary_color = cl_Gold;

     assertEquals( SQLITE_OK, teams_t_create( db, &expected ) );

     expected_accolades[0].team_id  = 1;
     expected_accolades[0].season   = 1;
     expected_accolades[0].accolade = tacc_League_Title;

     expected_accolades[1].team_id  = 1;
     expected_accolades[1].season   = 2;
     expected_accolades[1].accolade = tacc_Best_Record;

     expected_accolades[2] = team_accolade_sentinel;

     assertEquals( SQLITE_OK, team_accolades_t_create( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, team_accolades_t_create( db, &expected_accolades[1] ) );

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, remove_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_NOTFOUND, team_accolades_t_read_by_team( db, expected.team_id, &list ) );

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
     // get_team()
     run_test( get_team__ShouldReturnTheMatchingTeamObject_GivenATeamId,            check_sqlite_error );
     run_test( get_team__ShouldReturnTheMatchingTeamWithPlayers_GivenATeamId,       check_sqlite_error );
     run_test( get_team__ShouldReturnTheMatchingTeamWithStats_GivenATeamId,         check_sqlite_error );
     run_test( get_team__ShouldReturnTheMatchingTeamWithPitchingStats_GivenATeamId, check_sqlite_error );
     run_test( get_team__ShouldReturnTheMatchingTeamWithBattingStats_GivenATeamId,  check_sqlite_error );
     run_test( get_team__ShouldReturnTheMatchingTeamWithAccolades_GivenATeamId,     check_sqlite_error );

     // save_team()
     run_test( save_team__ShouldPersistTheTeamInTheDatabase_GivenATeamObject,                     check_sqlite_error );
     run_test( save_team__ShouldPersistTheTeamPlayersInTheDatabase_GivenATeamWithTeamPlayers,     check_sqlite_error );
     run_test( save_team__ShouldPersistTheTeamStatsInTheDatabase_GivenATeamWithTeamStats,         check_sqlite_error );
     run_test( save_team__ShouldPersistThePitchingStatsInTheDatabase_GivenATeamWithPitchingStats, check_sqlite_error );
     run_test( save_team__ShouldPersistTheBattingStatsInTheDatabase_GivenATeamWithBattingStats,   check_sqlite_error );
     run_test( save_team__ShouldPersistTheAccoladesInTheDatabase_GivenATeamWithAccolades,         check_sqlite_error );
     run_test( save_team__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenATeam,           check_sqlite_error );

     // remove_team()
     run_test( remove_team__ShouldRemoveTheTeamFromTheDatabase_GivenATeamObject,                             check_sqlite_error );
     run_test( remove_team__ShouldRemoveTheTeamPlayersFromTheDatabase_GivenATeamWithTeamPlayers,             check_sqlite_error );
     run_test( remove_team__ShouldRemoveTheTeamStatsFromTheDatabase_GivenATeamWithTeamStats,                 check_sqlite_error );
     run_test( remove_team__ShouldRemoveTheTeamPitchingStatsFromTheDatabase_GivenATeamWithTeamPitchingStats, check_sqlite_error );
     run_test( remove_team__ShouldRemoveTheTeamBattingStatsFromTheDatabase_GivenATeamWithTeamBattingStats,   check_sqlite_error );
     run_test( remove_team__ShouldRemoveTheAccoladesFromTheDatabase_GivenATeamWithAccolades,                 check_sqlite_error );
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
