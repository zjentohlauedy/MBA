define([
    'routing/router',
    'routing/indexRoute',
    'controllers/progressController',
    'controllers/startSeasonController',
    'controllers/rosterCutController',
    'controllers/rookieDraftController',
    'controllers/freeAgentsController',
    'ember'
], function(Router, IndexRoute, ProgressController, StartSeasonController, RosterCutController, RookieDraftController, FreeAgentsController, Ember) {

    var App = Ember.Application.create({
        Router:                Router,
        IndexRoute:            IndexRoute,
        ProgressController:    ProgressController,
        StartSeasonController: StartSeasonController,
        RosterCutController:   RosterCutController,
        RookieDraftController: RookieDraftController,
        FreeAgentsController:  FreeAgentsController
    });

    return App;
});
