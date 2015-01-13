define([
    'router', 'indexRoute', 'progressController', 'startSeasonController', 'rosterCutController', 'rookieDraftController', 'freeAgentsController', 'ember'
], function(Router, IndexRoute, ProgressController, StartSeasonController, RosterCutController, RookieDraftController, FreeAgentsController, Ember) {

    var App = {
        Router:                Router,
        IndexRoute:            IndexRoute,
        ProgressController:    ProgressController,
        StartSeasonController: StartSeasonController,
        RosterCutController:   RosterCutController,
        RookieDraftController: RookieDraftController,
        FreeAgentsController:  FreeAgentsController
    };

    return App;
});
