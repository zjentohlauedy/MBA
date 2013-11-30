#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "org.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static char *getOrg_ShouldRetrieveLeaguesFromDatabase_GivenASeason()
{
     league_s expected1 = { 0 };
     league_s expected2 = { 0 };

     buildIntoLeague( &expected1, 1 );
     buildIntoLeague( &expected2, 2 );

     assertEquals( SQLITE_OK, leagues_t_create( db, &expected1 ) );
     assertEquals( SQLITE_OK, leagues_t_create( db, &expected2 ) );

     org_s *org = getOrg( db, 1 );

     assertNotNull( org          );
     assertNotNull( org->leagues );

     assertNotNull(                   org->leagues[0].league            );
     assertEqualsInt( 1,              org->leagues[0].league->league_id );
     assertEqualsStr( expected1.name, org->leagues[0].league->name      );

     assertNotNull(                   org->leagues[1].league            );
     assertEqualsInt( 2,              org->leagues[1].league->league_id );
     assertEqualsStr( expected2.name, org->leagues[1].league->name      );

     assertNull( org->leagues[2].league );

     freeOrg( org );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected1 ) );
     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected2 ) );

     return NULL;
}

static char *getOrg_ShouldRetrieveLeagueTeamsFromDatabase_GivenASeason()
{
     league_s      league         = { 0 };
     league_team_s league_team1   = { 0 };
     league_team_s league_team2   = { 0 };
     team_s        expected_team1 = { 0 };
     team_s        expected_team2 = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueTeam( &league_team1, 1, 1 );
     buildIntoLeagueTeam( &league_team2, 1, 2 );
     buildIntoTeam( &expected_team1, 1 );
     buildIntoTeam( &expected_team2, 2 );

     assertEquals( SQLITE_OK, leagues_t_create( db, &league ) );
     assertEquals( SQLITE_OK, league_teams_t_create( db, &league_team1 ) );
     assertEquals( SQLITE_OK, league_teams_t_create( db, &league_team2 ) );
     assertEquals( SQLITE_OK, teams_t_create( db, &expected_team1 ) );
     assertEquals( SQLITE_OK, teams_t_create( db, &expected_team2 ) );

     org_s *org = getOrg( db, 1 );

     assertNotNull( org );
     assertNotNull( org->leagues );
     assertNotNull( org->leagues[0].league );
     assertNotNull( org->leagues[0].league->teams );

     league_team_s *league_teams = org->leagues[0].league->teams;

     assertNotNull(                           league_teams[0].team          );
     assertEqualsInt( league.league_id,       league_teams[0].league_id     );
     assertEqualsInt( expected_team1.team_id, league_teams[0].team_id       );
     assertEqualsInt( expected_team1.team_id, league_teams[0].team->team_id );
     assertEqualsStr( expected_team1.name,    league_teams[0].team->name    );

     assertNotNull(                           league_teams[1].team          );
     assertEqualsInt( league.league_id,       league_teams[1].league_id     );
     assertEqualsInt( expected_team2.team_id, league_teams[1].team_id       );
     assertEqualsInt( expected_team2.team_id, league_teams[1].team->team_id );
     assertEqualsStr( expected_team2.name,    league_teams[1].team->name    );

     league_team_s sentinel = LEAGUE_TEAM_SENTINEL;

     assertNull(                          league_teams[2].team      );
     assertEqualsInt( sentinel.league_id, league_teams[2].league_id );
     assertEqualsInt( sentinel.team_id,   league_teams[2].team_id   );

     freeOrg( org );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_teams_t_delete( db, &league_team1 ) );
     assertEquals( SQLITE_OK, league_teams_t_delete( db, &league_team2 ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team1 ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team2 ) );

     return NULL;
}

static char *getOrg_ShouldRetrieveLeagueTeamsWithPlayersFromDatabase_GivenASeason()
{
     league_s      league           = { 0 };
     league_team_s league_team      = { 0 };
     team_s        team             = { 0 };
     team_player_s team_player1     = { 0 };
     team_player_s team_player2     = { 0 };
     player_s      expected_player1 = { 0 };
     player_s      expected_player2 = { 0 };
     int           season           =   1;

     buildIntoLeague( &league, 1 );
     buildIntoLeagueTeam( &league_team, 1, 1 );
     buildIntoTeam( &team, 1 );
     buildIntoTeamPlayer( &team_player1, 1, season, 1 );
     buildIntoTeamPlayer( &team_player2, 1, season, 2 );
     buildIntoPlayer( &expected_player1, 1 );
     buildIntoPlayer( &expected_player2, 2 );

     assertEquals( SQLITE_OK, leagues_t_create( db, &league ) );
     assertEquals( SQLITE_OK, league_teams_t_create( db, &league_team ) );
     assertEquals( SQLITE_OK, teams_t_create( db, &team ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &team_player2 ) );
     assertEquals( SQLITE_OK, players_t_create( db, &expected_player1 ) );
     assertEquals( SQLITE_OK, players_t_create( db, &expected_player2 ) );

     org_s *org = getOrg( db, season );

     assertNotNull( org );
     assertNotNull( org->leagues );
     assertNotNull( org->leagues[0].league );
     assertNotNull( org->leagues[0].league->teams );
     assertNotNull( org->leagues[0].league->teams[0].team );
     assertNotNull( org->leagues[0].league->teams[0].team->players );

     team_player_s *team_players = org->leagues[0].league->teams[0].team->players;

     assertNotNull(                                team_players[0].player             );
     assertEqualsInt( team.team_id,                team_players[0].team_id            );
     assertEqualsInt( season,                      team_players[0].season             );
     assertEqualsInt( expected_player1.player_id,  team_players[0].player_id          );
     assertEqualsInt( expected_player1.player_id,  team_players[0].player->player_id  );
     assertEqualsStr( expected_player1.first_name, team_players[0].player->first_name );
     assertEqualsStr( expected_player1.last_name,  team_players[0].player->last_name  );

     assertNotNull(                                team_players[1].player             );
     assertEqualsInt( team.team_id,                team_players[1].team_id            );
     assertEqualsInt( season,                      team_players[1].season             );
     assertEqualsInt( expected_player2.player_id,  team_players[1].player_id          );
     assertEqualsInt( expected_player2.player_id,  team_players[1].player->player_id  );
     assertEqualsStr( expected_player2.first_name, team_players[1].player->first_name );
     assertEqualsStr( expected_player2.last_name,  team_players[1].player->last_name  );

     team_player_s sentinel = TEAM_PLAYER_SENTINEL;

     assertNull(                          team_players[2].player    );
     assertEqualsInt( sentinel.team_id,   team_players[2].team_id   );
     assertEqualsInt( sentinel.season,    team_players[2].season    );
     assertEqualsInt( sentinel.player_id, team_players[2].player_id );

     freeOrg( org );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_teams_t_delete( db, &league_team ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &team ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &team_player2 ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player1 ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player2 ) );

     return NULL;
}

static char *getOrg_ShouldRetrieveDivisionsFromDatabase_GivenASeason()
{
     league_s          league             = { 0 };
     league_division_s league_division1   = { 0 };
     league_division_s league_division2   = { 0 };
     division_s        expected_division1 = { 0 };
     division_s        expected_division2 = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueDivision( &league_division1, 1, 1 );
     buildIntoLeagueDivision( &league_division2, 1, 2 );
     buildIntoDivision( &expected_division1, 1 );
     buildIntoDivision( &expected_division2, 2 );

     assertEquals( SQLITE_OK, leagues_t_create( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &league_division1 ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &league_division2 ) );
     assertEquals( SQLITE_OK, divisions_t_create( db, &expected_division1 ) );
     assertEquals( SQLITE_OK, divisions_t_create( db, &expected_division2 ) );

     org_s *org = getOrg( db, 1 );

     assertNotNull( org );
     assertNotNull( org->leagues );
     assertNotNull( org->leagues[0].league );
     assertNotNull( org->leagues[0].league->divisions );

     league_division_s *league_divisions = org->leagues[0].league->divisions;

     assertNotNull(                                   league_divisions[0].division              );
     assertEqualsInt( league.league_id,               league_divisions[0].league_id             );
     assertEqualsInt( expected_division1.division_id, league_divisions[0].division_id           );
     assertEqualsInt( expected_division1.division_id, league_divisions[0].division->division_id );
     assertEqualsStr( expected_division1.name,        league_divisions[0].division->name        );

     assertNotNull(                                   league_divisions[1].division              );
     assertEqualsInt( league.league_id,               league_divisions[1].league_id             );
     assertEqualsInt( expected_division2.division_id, league_divisions[1].division_id           );
     assertEqualsInt( expected_division2.division_id, league_divisions[1].division->division_id );
     assertEqualsStr( expected_division2.name,        league_divisions[1].division->name        );

     league_division_s sentinel = LEAGUE_DIVISION_SENTINEL;

     assertNull(                            league_divisions[2].division    );
     assertEqualsInt( sentinel.league_id,   league_divisions[2].league_id   );
     assertEqualsInt( sentinel.division_id, league_divisions[2].division_id );

     freeOrg( org );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &league_division1 ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &league_division2 ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected_division1 ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected_division2 ) );

     return NULL;
}

static char *getOrg_ShouldRetrieveDivisionsWithTeamsFromDatabase_GivenASeason()
{
     league_s          league          = { 0 };
     league_division_s league_division = { 0 };
     division_s        division        = { 0 };
     division_team_s   division_team1  = { 0 };
     division_team_s   division_team2  = { 0 };
     team_s            expected_team1  = { 0 };
     team_s            expected_team2  = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueDivision( &league_division, 1, 1 );
     buildIntoDivision( &division, 1 );
     buildIntoDivisionTeam( &division_team1, 1, 1 );
     buildIntoDivisionTeam( &division_team2, 1, 2 );
     buildIntoTeam( &expected_team1, 1 );
     buildIntoTeam( &expected_team2, 2 );

     assertEquals( SQLITE_OK, leagues_t_create( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &league_division ) );
     assertEquals( SQLITE_OK, divisions_t_create( db, &division ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &division_team1 ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &division_team2 ) );
     assertEquals( SQLITE_OK, teams_t_create( db, &expected_team1 ) );
     assertEquals( SQLITE_OK, teams_t_create( db, &expected_team2 ) );

     org_s *org = getOrg( db, 1 );

     assertNotNull( org );
     assertNotNull( org->leagues );
     assertNotNull( org->leagues[0].league );
     assertNotNull( org->leagues[0].league->divisions );
     assertNotNull( org->leagues[0].league->divisions[0].division );
     assertNotNull( org->leagues[0].league->divisions[0].division->teams );

     division_team_s *division_teams = org->leagues[0].league->divisions[0].division->teams;

     assertNotNull(                           division_teams[0].team          );
     assertEqualsInt( division.division_id,   division_teams[0].division_id   );
     assertEqualsInt( expected_team1.team_id, division_teams[0].team_id       );
     assertEqualsInt( expected_team1.team_id, division_teams[0].team->team_id );
     assertEqualsStr( expected_team1.name,    division_teams[0].team->name    );

     assertNotNull(                           division_teams[1].team          );
     assertEqualsInt( division.division_id,   division_teams[1].division_id   );
     assertEqualsInt( expected_team2.team_id, division_teams[1].team_id       );
     assertEqualsInt( expected_team2.team_id, division_teams[1].team->team_id );
     assertEqualsStr( expected_team2.name,    division_teams[1].team->name    );

     division_team_s sentinel = DIVISION_TEAM_SENTINEL;

     assertNull(                            division_teams[2].team        );
     assertEqualsInt( sentinel.division_id, division_teams[2].division_id );
     assertEqualsInt( sentinel.team_id,     division_teams[2].team_id     );

     freeOrg( org );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &league_division ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &division ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &division_team1 ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &division_team2 ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team1 ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team2 ) );

     return NULL;
}

static char *getOrg_ShouldRetrieveDivisionsWithTeamsAndPlayersFromDatabase_GivenASeason()
{
     league_s          league           = { 0 };
     league_division_s league_division  = { 0 };
     division_s        division         = { 0 };
     division_team_s   division_team    = { 0 };
     team_s            team             = { 0 };
     team_player_s     team_player1     = { 0 };
     team_player_s     team_player2     = { 0 };
     player_s          expected_player1 = { 0 };
     player_s          expected_player2 = { 0 };
     int               season           =   1;

     buildIntoLeague( &league, 1 );
     buildIntoLeagueDivision( &league_division, 1, 1 );
     buildIntoDivision( &division, 1 );
     buildIntoDivisionTeam( &division_team, 1, 1 );
     buildIntoTeam( &team, 1 );
     buildIntoTeamPlayer( &team_player1, 1, season, 1 );
     buildIntoTeamPlayer( &team_player2, 1, season, 2 );
     buildIntoPlayer( &expected_player1, 1 );
     buildIntoPlayer( &expected_player2, 2 );

     assertEquals( SQLITE_OK, leagues_t_create( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_create( db, &league_division ) );
     assertEquals( SQLITE_OK, divisions_t_create( db, &division ) );
     assertEquals( SQLITE_OK, division_teams_t_create( db, &division_team ) );
     assertEquals( SQLITE_OK, teams_t_create( db, &team ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, &team_player2 ) );
     assertEquals( SQLITE_OK, players_t_create( db, &expected_player1 ) );
     assertEquals( SQLITE_OK, players_t_create( db, &expected_player2 ) );

     org_s *org = getOrg( db, season );

     assertNotNull( org );
     assertNotNull( org->leagues );
     assertNotNull( org->leagues[0].league );
     assertNotNull( org->leagues[0].league->divisions );
     assertNotNull( org->leagues[0].league->divisions[0].division );
     assertNotNull( org->leagues[0].league->divisions[0].division->teams );
     assertNotNull( org->leagues[0].league->divisions[0].division->teams[0].team );
     assertNotNull( org->leagues[0].league->divisions[0].division->teams[0].team->players );

     team_player_s *team_players = org->leagues[0].league->divisions[0].division->teams[0].team->players;

     assertNotNull(                                team_players[0].player             );
     assertEqualsInt( team.team_id,                team_players[0].team_id            );
     assertEqualsInt( season,                      team_players[0].season             );
     assertEqualsInt( expected_player1.player_id,  team_players[0].player_id          );
     assertEqualsInt( expected_player1.player_id,  team_players[0].player->player_id  );
     assertEqualsStr( expected_player1.first_name, team_players[0].player->first_name );
     assertEqualsStr( expected_player1.last_name,  team_players[0].player->last_name  );

     assertNotNull(                                team_players[1].player             );
     assertEqualsInt( team.team_id,                team_players[1].team_id            );
     assertEqualsInt( season,                      team_players[1].season             );
     assertEqualsInt( expected_player2.player_id,  team_players[1].player_id          );
     assertEqualsInt( expected_player2.player_id,  team_players[1].player->player_id  );
     assertEqualsStr( expected_player2.first_name, team_players[1].player->first_name );
     assertEqualsStr( expected_player2.last_name,  team_players[1].player->last_name  );

     team_player_s sentinel = TEAM_PLAYER_SENTINEL;

     assertNull(                          team_players[2].player    );
     assertEqualsInt( sentinel.team_id,   team_players[2].team_id   );
     assertEqualsInt( sentinel.season,    team_players[2].season    );
     assertEqualsInt( sentinel.player_id, team_players[2].player_id );

     freeOrg( org );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &league_division ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &division ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &division_team ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &team ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &team_player2 ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player1 ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player2 ) );

     return NULL;
}

static char *saveOrg_ShouldSaveLeaguesToDatabase_GivenAOrgWithLeagues()
{
     org_s        org                 = { 0 };
     org_league_s org_leagues[3]      = { 0 };
     org_league_s org_league_sentinel = ORG_LEAGUE_SENTINEL;
     league_s     expected_league1    = { 0 };
     league_s     expected_league2    = { 0 };

     buildIntoLeague( &expected_league1, 1 );
     buildIntoLeague( &expected_league2, 2 );

     org_leagues[0].league = &expected_league1;
     org_leagues[1].league = &expected_league2;
     org_leagues[2]        =  org_league_sentinel;

     org.leagues = org_leagues;

     assertEquals( SQLITE_OK, saveOrg( db, &org ) );

     league_s actual_league1 = { 0 };
     league_s actual_league2 = { 0 };

     actual_league1.league_id = 1;
     actual_league2.league_id = 2;

     assertEquals( SQLITE_OK, leagues_t_read( db, &actual_league1 ) );
     assertEquals( SQLITE_OK, leagues_t_read( db, &actual_league2 ) );

     assertEqualsInt( expected_league1.league_id, actual_league1.league_id );
     assertEqualsStr( expected_league1.name,      actual_league1.name      );

     assertEqualsInt( expected_league2.league_id, actual_league2.league_id );
     assertEqualsStr( expected_league2.name,      actual_league2.name      );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected_league1 ) );
     assertEquals( SQLITE_OK, leagues_t_delete( db, &expected_league2 ) );

     return NULL;
}

static char *saveOrg_ShouldSaveTeamsToDatabase_GivenAOrgWithLeagueTeams()
{
     org_s         org                      = { 0 };
     org_league_s  org_leagues[2]           = { 0 };
     org_league_s  org_league_sentinel      = ORG_LEAGUE_SENTINEL;
     league_s      league                   = { 0 };
     league_team_s expected_league_teams[3] = { 0 };
     league_team_s league_team_sentinel     = LEAGUE_TEAM_SENTINEL;
     team_s        expected_team1           = { 0 };
     team_s        expected_team2           = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueTeam( &expected_league_teams[0], 1, 1 );
     buildIntoLeagueTeam( &expected_league_teams[1], 1, 2 );
     buildIntoTeam( &expected_team1, 1 );
     buildIntoTeam( &expected_team2, 2 );

     expected_league_teams[0].team = &expected_team1;
     expected_league_teams[1].team = &expected_team2;
     expected_league_teams[2]      =  league_team_sentinel;

     league.teams = expected_league_teams;

     org_leagues[0].league = &league;
     org_leagues[1]        =  org_league_sentinel;

     org.leagues = org_leagues;

     assertEquals( SQLITE_OK, saveOrg( db, &org ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_teams_t_read_by_league( db, league.league_id, &list ) );

     assertEquals( 2, list.count );

     league_team_s *actual_league_teams = list.data;

     assertNotNull( actual_league_teams );

     assertEquals( expected_league_teams[0].league_id, actual_league_teams[0].league_id );
     assertEquals( expected_league_teams[0].team_id,   actual_league_teams[0].team_id   );

     assertEquals( expected_league_teams[1].league_id, actual_league_teams[1].league_id );
     assertEquals( expected_league_teams[1].team_id,   actual_league_teams[1].team_id   );

     team_s actual_team1 = { 0 };
     team_s actual_team2 = { 0 };

     actual_team1.team_id = 1;
     actual_team2.team_id = 2;

     assertEquals( SQLITE_OK, teams_t_read( db, &actual_team1 ) );
     assertEquals( SQLITE_OK, teams_t_read( db, &actual_team2 ) );

     assertEqualsInt( expected_team1.team_id, actual_team1.team_id );
     assertEqualsStr( expected_team1.name,    actual_team1.name    );

     assertEqualsInt( expected_team2.team_id, actual_team2.team_id );
     assertEqualsStr( expected_team2.name,    actual_team2.name    );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_teams_t_delete( db, &expected_league_teams[0] ) );
     assertEquals( SQLITE_OK, league_teams_t_delete( db, &expected_league_teams[1] ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team1 ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team2 ) );

     return NULL;
}

static char *saveOrg_ShouldSavePlayersToDatabase_GivenAOrgWithLeagueTeamPlayers()
{
     org_s         org                      = { 0 };
     org_league_s  org_leagues[2]           = { 0 };
     org_league_s  org_league_sentinel      = ORG_LEAGUE_SENTINEL;
     league_s      league                   = { 0 };
     league_team_s league_teams[2]          = { 0 };
     league_team_s league_team_sentinel     = LEAGUE_TEAM_SENTINEL;
     team_s        team                     = { 0 };
     team_player_s expected_team_players[3] = { 0 };
     team_player_s team_player_sentinel     = TEAM_PLAYER_SENTINEL;
     player_s      expected_player1         = { 0 };
     player_s      expected_player2         = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueTeam( &league_teams[0], 1, 1 );
     buildIntoTeam( &team, 1 );
     buildIntoTeamPlayer( &expected_team_players[0], 1, 1, 1 );
     buildIntoTeamPlayer( &expected_team_players[1], 1, 1, 2 );
     buildIntoPlayer( &expected_player1, 1 );
     buildIntoPlayer( &expected_player2, 2 );

     expected_team_players[0].player = &expected_player1;
     expected_team_players[1].player = &expected_player2;
     expected_team_players[2]        = team_player_sentinel;

     team.players = expected_team_players;

     league_teams[0].team = &team;
     league_teams[1]      =  league_team_sentinel;

     league.teams = league_teams;

     org_leagues[0].league = &league;
     org_leagues[1]        =  org_league_sentinel;

     org.leagues = org_leagues;

     assertEquals( SQLITE_OK, saveOrg( db, &org ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_players_t_read_by_team( db, team.team_id, &list ) );

     assertEquals( 2, list.count );

     team_player_s *actual_team_players = list.data;

     assertNotNull( actual_team_players );

     assertEquals( expected_team_players[0].team_id,   actual_team_players[0].team_id   );
     assertEquals( expected_team_players[0].season,    actual_team_players[0].season    );
     assertEquals( expected_team_players[0].player_id, actual_team_players[0].player_id );

     assertEquals( expected_team_players[1].team_id,   actual_team_players[1].team_id   );
     assertEquals( expected_team_players[1].season,    actual_team_players[1].season    );
     assertEquals( expected_team_players[1].player_id, actual_team_players[1].player_id );

     player_s actual_player1 = { 0 };
     player_s actual_player2 = { 0 };

     actual_player1.player_id = 1;
     actual_player2.player_id = 2;

     assertEquals( SQLITE_OK, players_t_read( db, &actual_player1 ) );
     assertEquals( SQLITE_OK, players_t_read( db, &actual_player2 ) );

     assertEqualsInt( expected_player1.player_id,  actual_player1.player_id  );
     assertEqualsStr( expected_player1.first_name, actual_player1.first_name );
     assertEqualsStr( expected_player1.last_name,  actual_player1.last_name  );

     assertEqualsInt( expected_player2.player_id,  actual_player2.player_id  );
     assertEqualsStr( expected_player2.first_name, actual_player2.first_name );
     assertEqualsStr( expected_player2.last_name,  actual_player2.last_name  );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_teams_t_delete( db, &league_teams[0] ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &team ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[0] ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[1] ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player1 ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player2 ) );

     return NULL;
}

static char *saveOrg_ShouldSaveDivisionsToDatabase_GivenAOrgWithDivisions()
{
     org_s             org                          = { 0 };
     org_league_s      org_leagues[2]               = { 0 };
     org_league_s      org_league_sentinel          = ORG_LEAGUE_SENTINEL;
     league_s          league                       = { 0 };
     league_division_s expected_league_divisions[3] = { 0 };
     league_division_s league_division_sentinel     = LEAGUE_DIVISION_SENTINEL;
     division_s        expected_division1           = { 0 };
     division_s        expected_division2           = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueDivision( &expected_league_divisions[0], 1, 1 );
     buildIntoLeagueDivision( &expected_league_divisions[1], 1, 2 );
     buildIntoDivision( &expected_division1, 1 );
     buildIntoDivision( &expected_division2, 2 );

     expected_league_divisions[0].division = &expected_division1;
     expected_league_divisions[1].division = &expected_division2;
     expected_league_divisions[2]          =  league_division_sentinel;

     league.divisions = expected_league_divisions;

     org_leagues[0].league = &league;
     org_leagues[1]        =  org_league_sentinel;

     org.leagues = org_leagues;

     assertEquals( SQLITE_OK, saveOrg( db, &org ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, league_divisions_t_read_by_league( db, league.league_id, &list ) );

     assertEquals( 2, list.count );

     league_division_s *actual_league_divisions = list.data;

     assertNotNull( actual_league_divisions );

     assertEquals( expected_league_divisions[0].league_id,   actual_league_divisions[0].league_id   );
     assertEquals( expected_league_divisions[0].division_id, actual_league_divisions[0].division_id );

     assertEquals( expected_league_divisions[1].league_id,   actual_league_divisions[1].league_id   );
     assertEquals( expected_league_divisions[1].division_id, actual_league_divisions[1].division_id );

     division_s actual_division1 = { 0 };
     division_s actual_division2 = { 0 };

     actual_division1.division_id = 1;
     actual_division2.division_id = 2;

     assertEquals( SQLITE_OK, divisions_t_read( db, &actual_division1 ) );
     assertEquals( SQLITE_OK, divisions_t_read( db, &actual_division2 ) );

     assertEqualsInt( expected_division1.division_id, actual_division1.division_id );
     assertEqualsStr( expected_division1.name,        actual_division1.name        );

     assertEqualsInt( expected_division2.division_id, actual_division2.division_id );
     assertEqualsStr( expected_division2.name,        actual_division2.name        );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions[0] ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &expected_league_divisions[1] ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected_division1 ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &expected_division2 ) );

     return NULL;
}

static char *saveOrg_ShouldSaveDivisionTeamsToDatabase_GivenAOrgWithDivisionTeams()
{
     org_s             org                        = { 0 };
     org_league_s      org_leagues[2]             = { 0 };
     org_league_s      org_league_sentinel        = ORG_LEAGUE_SENTINEL;
     league_s          league                     = { 0 };
     league_division_s league_divisions[2]        = { 0 };
     league_division_s league_division_sentinel   = LEAGUE_DIVISION_SENTINEL;
     division_s        division                   = { 0 };
     division_team_s   expected_division_teams[3] = { 0 };
     division_team_s   division_team_sentinel     = DIVISION_TEAM_SENTINEL;
     team_s            expected_team1             = { 0 };
     team_s            expected_team2             = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueDivision( &league_divisions[0], 1, 1 );
     buildIntoDivision( &division, 1 );
     buildIntoDivisionTeam( &expected_division_teams[0], 1, 1 );
     buildIntoDivisionTeam( &expected_division_teams[1], 1, 2 );
     buildIntoTeam( &expected_team1, 1 );
     buildIntoTeam( &expected_team2, 2 );

     expected_division_teams[0].team = &expected_team1;
     expected_division_teams[1].team = &expected_team2;
     expected_division_teams[2]      = division_team_sentinel;

     division.teams = expected_division_teams;

     league_divisions[0].division = &division;
     league_divisions[1]          =  league_division_sentinel;

     league.divisions = league_divisions;

     org_leagues[0].league = &league;
     org_leagues[1]        =  org_league_sentinel;

     org.leagues = org_leagues;

     assertEquals( SQLITE_OK, saveOrg( db, &org ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, division_teams_t_read_by_division( db, division.division_id, &list ) );

     assertEquals( 2, list.count );

     division_team_s *actual_division_teams = list.data;

     assertNotNull( actual_division_teams );

     assertEquals( expected_division_teams[0].division_id, actual_division_teams[0].division_id );
     assertEquals( expected_division_teams[0].team_id,     actual_division_teams[0].team_id     );

     assertEquals( expected_division_teams[1].division_id, actual_division_teams[1].division_id );
     assertEquals( expected_division_teams[1].team_id,     actual_division_teams[1].team_id     );

     team_s actual_team1 = { 0 };
     team_s actual_team2 = { 0 };

     actual_team1.team_id = 1;
     actual_team2.team_id = 2;

     assertEquals( SQLITE_OK, teams_t_read( db, &actual_team1 ) );
     assertEquals( SQLITE_OK, teams_t_read( db, &actual_team2 ) );

     assertEqualsInt( expected_team1.team_id, actual_team1.team_id );
     assertEqualsStr( expected_team1.name,    actual_team1.name    );

     assertEqualsInt( expected_team2.team_id, actual_team2.team_id );
     assertEqualsStr( expected_team2.name,    actual_team2.name    );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &league_divisions[0] ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &division ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[0] ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &expected_division_teams[1] ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team1 ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &expected_team2 ) );

     return NULL;
}

static char *saveOrg_ShouldSavePlayersToDatabase_GivenAOrgWithDivisionTeamsWithPlayers()
{
     org_s             org                        = { 0 };
     org_league_s      org_leagues[2]             = { 0 };
     org_league_s      org_league_sentinel        = ORG_LEAGUE_SENTINEL;
     league_s          league                     = { 0 };
     league_division_s league_divisions[2]        = { 0 };
     league_division_s league_division_sentinel   = LEAGUE_DIVISION_SENTINEL;
     division_s        division                   = { 0 };
     division_team_s   division_teams[2]          = { 0 };
     division_team_s   division_team_sentinel     = DIVISION_TEAM_SENTINEL;
     team_s            team                       = { 0 };
     team_player_s     expected_team_players[3]   = { 0 };
     team_player_s     team_player_sentinel       = TEAM_PLAYER_SENTINEL;
     player_s          expected_player1           = { 0 };
     player_s          expected_player2           = { 0 };

     buildIntoLeague( &league, 1 );
     buildIntoLeagueDivision( &league_divisions[0], 1, 1 );
     buildIntoDivision( &division, 1 );
     buildIntoDivisionTeam( &division_teams[0], 1, 1 );
     buildIntoTeam( &team, 1 );
     buildIntoTeamPlayer( &expected_team_players[0], 1, 1, 1 );
     buildIntoTeamPlayer( &expected_team_players[1], 1, 1, 2 );
     buildIntoPlayer( &expected_player1, 1 );
     buildIntoPlayer( &expected_player2, 2 );

     expected_team_players[0].player = &expected_player1;
     expected_team_players[1].player = &expected_player2;
     expected_team_players[2]        = team_player_sentinel;

     team.players = expected_team_players;

     division_teams[0].team = &team;
     division_teams[1]      = division_team_sentinel;

     division.teams = division_teams;

     league_divisions[0].division = &division;
     league_divisions[1]          =  league_division_sentinel;

     league.divisions = league_divisions;

     org_leagues[0].league = &league;
     org_leagues[1]        =  org_league_sentinel;

     org.leagues = org_leagues;

     assertEquals( SQLITE_OK, saveOrg( db, &org ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_players_t_read_by_team( db, team.team_id, &list ) );

     assertEquals( 2, list.count );

     team_player_s *actual_team_players = list.data;

     assertNotNull( actual_team_players );

     assertEquals( expected_team_players[0].team_id,   actual_team_players[0].team_id   );
     assertEquals( expected_team_players[0].season,    actual_team_players[0].season    );
     assertEquals( expected_team_players[0].player_id, actual_team_players[0].player_id );

     assertEquals( expected_team_players[1].team_id,   actual_team_players[1].team_id   );
     assertEquals( expected_team_players[1].season,    actual_team_players[1].season    );
     assertEquals( expected_team_players[1].player_id, actual_team_players[1].player_id );

     player_s actual_player1 = { 0 };
     player_s actual_player2 = { 0 };

     actual_player1.player_id = 1;
     actual_player2.player_id = 2;

     assertEquals( SQLITE_OK, players_t_read( db, &actual_player1 ) );
     assertEquals( SQLITE_OK, players_t_read( db, &actual_player2 ) );

     assertEqualsInt( expected_player1.player_id,  actual_player1.player_id  );
     assertEqualsStr( expected_player1.first_name, actual_player1.first_name );
     assertEqualsStr( expected_player1.last_name,  actual_player1.last_name  );

     assertEqualsInt( expected_player2.player_id,  actual_player2.player_id  );
     assertEqualsStr( expected_player2.first_name, actual_player2.first_name );
     assertEqualsStr( expected_player2.last_name,  actual_player2.last_name  );

     assertEquals( SQLITE_OK, leagues_t_delete( db, &league ) );
     assertEquals( SQLITE_OK, league_divisions_t_delete( db, &league_divisions[0] ) );
     assertEquals( SQLITE_OK, divisions_t_delete( db, &division ) );
     assertEquals( SQLITE_OK, division_teams_t_delete( db, &division_teams[0] ) );
     assertEquals( SQLITE_OK, teams_t_delete( db, &team ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[0] ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, &expected_team_players[1] ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player1 ) );
     assertEquals( SQLITE_OK, players_t_delete( db, &expected_player2 ) );

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
     run_test( getOrg_ShouldRetrieveLeaguesFromDatabase_GivenASeason,                      check_sqlite_error );
     run_test( getOrg_ShouldRetrieveLeagueTeamsFromDatabase_GivenASeason,                  check_sqlite_error );
     run_test( getOrg_ShouldRetrieveLeagueTeamsWithPlayersFromDatabase_GivenASeason,       check_sqlite_error );
     run_test( getOrg_ShouldRetrieveDivisionsFromDatabase_GivenASeason,                    check_sqlite_error );
     run_test( getOrg_ShouldRetrieveDivisionsWithTeamsFromDatabase_GivenASeason,           check_sqlite_error );
     run_test( getOrg_ShouldRetrieveDivisionsWithTeamsAndPlayersFromDatabase_GivenASeason, check_sqlite_error );

     run_test( saveOrg_ShouldSaveLeaguesToDatabase_GivenAOrgWithLeagues,                  check_sqlite_error );
     run_test( saveOrg_ShouldSaveTeamsToDatabase_GivenAOrgWithLeagueTeams,                check_sqlite_error );
     run_test( saveOrg_ShouldSavePlayersToDatabase_GivenAOrgWithLeagueTeamPlayers,        check_sqlite_error );
     run_test( saveOrg_ShouldSaveDivisionsToDatabase_GivenAOrgWithDivisions,              check_sqlite_error );
     run_test( saveOrg_ShouldSaveDivisionTeamsToDatabase_GivenAOrgWithDivisionTeams,      check_sqlite_error );
     run_test( saveOrg_ShouldSavePlayersToDatabase_GivenAOrgWithDivisionTeamsWithPlayers, check_sqlite_error );
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
