define([], function() {

    var Constants = {
        TEAM_MAX_PITCHERS:          10,
        TEAM_MAX_BATTERS:           15,
        ROOKIE_DRAFT_MAX_PITCHERS:   4,
        ROOKIE_DRAFT_MAX_BATTERS:    6,
        ROSTER_CUT_TARGET_PITCHERS:  3,
        ROSTER_CUT_TARGET_BATTERS:   5,

        STATUS_URI:           '/mba/status',
        TEAMS_URI:            '/mba/resources/teams',
        PLAYERS_URI:          '/mba/resources/players',
        ROOKIE_DRAFT_URI:     '/mba/resources/drafts/rookie/season',
        FREE_AGENT_DRAFT_URI: '/mba/resources/drafts/free-agent/season',
        START_SEASON_URI:     '/mba/actions/start_season'
    };

    return Constants;
});
