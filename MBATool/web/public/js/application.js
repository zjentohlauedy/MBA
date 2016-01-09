define([
    'routing/router',
    'routing/indexRoute',
    'routing/rosterCutRoute',
    'routing/rookieDraftRoute',
    'routing/freeAgentsRoute',
    'routing/exportSeasonRoute',
    'routing/importSeasonRoute',
    'routing/accoladesRoute',
    'routing/completeSeasonRoute',

    'controllers/progressController',
    'controllers/startSeasonController',
    'controllers/rosterCutController',
    'controllers/rookieDraftController',
    'controllers/freeAgentsController',
    'controllers/exportSeasonController',
    'controllers/importSeasonController',
    'controllers/accoladesController',
    'controllers/completeSeasonController',

    'components/playerSelectorComponent',
    'ember'
], function(
    Router,
    IndexRoute,
    RosterCutRoute,
    RookieDraftRoute,
    FreeAgentsRoute,
    ExportSeasonRoute,
    ImportSeasonRoute,
    AccoladesRoute,
    CompleteSeasonRoute,

    ProgressController,
    StartSeasonController,
    RosterCutController,
    RookieDraftController,
    FreeAgentsController,
    ExportSeasonController,
    ImportSeasonController,
    AccoladesController,
    CompleteSeasonController,
    PlayerSelectorComponent,
    Ember) {

    var App = Ember.Application.create({
        Router:                   Router,
        IndexRoute:               IndexRoute,
        RosterCutRoute:           RosterCutRoute,
        RookieDraftRoute:         RookieDraftRoute,
        FreeAgentsRoute:          FreeAgentsRoute,
        ExportSeasonRoute:        ExportSeasonRoute,
        ImportSeasonRoute:        ImportSeasonRoute,
        AccoladesRoute:           AccoladesRoute,
        CompleteSeasonRoute:      CompleteSeasonRoute,

        ProgressController:       ProgressController,
        StartSeasonController:    StartSeasonController,
        RosterCutController:      RosterCutController,
        RookieDraftController:    RookieDraftController,
        FreeAgentsController:     FreeAgentsController,
        ExportSeasonController:   ExportSeasonController,
        ImportSeasonController:   ImportSeasonController,
        AccoladesController:      AccoladesController,
        CompleteSeasonController: CompleteSeasonController,
        PlayerSelectorComponent:  PlayerSelectorComponent
    });

    return App;
});
