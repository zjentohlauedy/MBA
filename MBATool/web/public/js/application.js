define([
    'routing/router',
    'routing/indexRoute',
    'routing/rosterCutRoute',
    'routing/rookieDraftRoute',
    'routing/freeAgentsRoute',
    'controllers/progressController',
    'controllers/startSeasonController',
    'controllers/rosterCutController',
    'controllers/rookieDraftController',
    'controllers/freeAgentsController',
    'ember'
], function(
    Router,
    IndexRoute,
    RosterCutRoute,
    RookieDraftRoute,
    FreeAgentsRoute,
    ProgressController,
    StartSeasonController,
    RosterCutController,
    RookieDraftController,
    FreeAgentsController,
    Ember) {

    var App = Ember.Application.create({
        Router:                Router,
        IndexRoute:            IndexRoute,
        RosterCutRoute:        RosterCutRoute,
        RookieDraftRoute:      RookieDraftRoute,
        FreeAgentsRoute:       FreeAgentsRoute,
        ProgressController:    ProgressController,
        StartSeasonController: StartSeasonController,
        RosterCutController:   RosterCutController,
        RookieDraftController: RookieDraftController,
        FreeAgentsController:  FreeAgentsController
    });

    return App;
});
