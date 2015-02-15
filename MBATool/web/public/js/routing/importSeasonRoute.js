define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var ImportSeasonRoute = Ember.Route.extend({
        expectedStage: 5,
        redirect: function() { Actions.importSeasonRouteRedirect(this); }
    });

    return ImportSeasonRoute;
});
