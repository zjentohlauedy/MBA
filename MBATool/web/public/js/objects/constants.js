define([], function() {

    var Constants = {
        TEAM_MAX_PITCHERS:          11,
        TEAM_MAX_BATTERS:           14,
        ROOKIE_DRAFT_MAX_PITCHERS:   4,
        ROOKIE_DRAFT_MAX_BATTERS:    6,
        ROSTER_CUT_TARGET_PITCHERS:  3,
        ROSTER_CUT_TARGET_BATTERS:   5,

        PICKS_PER_ROUND:            32,

        STATUS_URI:            '/mba/status',
        ORGANIZATIONS_URI:     '/mba/resources/organizations',
        TEAMS_URI:             '/mba/resources/teams',
        PLAYERS_URI:           '/mba/resources/players',
        ROOKIE_DRAFT_URI:      '/mba/resources/drafts/rookie/season',
        FREE_AGENT_DRAFT_URI:  '/mba/resources/drafts/free-agent/season',
        START_SEASON_URI:      '/mba/actions/start_season',
        EXPORT_SEASON_URI:     '/mba/actions/export_season',
        IMPORT_SEASON_URI:     '/mba/actions/import_season',
        RESOLVE_ACCOLADES_URI: '/mba/actions/resolve_accolades'
    };

    return Constants;
});
