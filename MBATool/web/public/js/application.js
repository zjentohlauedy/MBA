define([
    'routing/router',
    'routing/indexRoute',
    'routing/rosterCutRoute',
    'routing/rookieDraftRoute',
    'routing/freeAgentsRoute',
    'routing/exportSeasonRoute',
    'controllers/progressController',
    'controllers/startSeasonController',
    'controllers/rosterCutController',
    'controllers/rookieDraftController',
    'controllers/freeAgentsController',
    'controllers/exportSeasonController',
    'ember'
], function(
    Router,
    IndexRoute,
    RosterCutRoute,
    RookieDraftRoute,
    FreeAgentsRoute,
    ExportSeasonRoute,
    ProgressController,
    StartSeasonController,
    RosterCutController,
    RookieDraftController,
    FreeAgentsController,
    ExportSeasonController,
    Ember) {

    var App = Ember.Application.create({
        Router:                 Router,
        IndexRoute:             IndexRoute,
        RosterCutRoute:         RosterCutRoute,
        RookieDraftRoute:       RookieDraftRoute,
        FreeAgentsRoute:        FreeAgentsRoute,
        ExportSeasonRoute:      ExportSeasonRoute,
        ProgressController:     ProgressController,
        StartSeasonController:  StartSeasonController,
        RosterCutController:    RosterCutController,
        RookieDraftController:  RookieDraftController,
        FreeAgentsController:   FreeAgentsController,
        ExportSeasonController: ExportSeasonController
    });

    return App;
});
