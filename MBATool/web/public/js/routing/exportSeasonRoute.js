define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var ExportSeasonRoute = Ember.Route.extend({
        expectedStage: 4,
        redirect: function() { Actions.exportSeasonRouteRedirect(this); }
    });

    return ExportSeasonRoute;
});
