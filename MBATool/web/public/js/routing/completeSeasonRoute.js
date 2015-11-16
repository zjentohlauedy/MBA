define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var CompleteSeasonRoute = Ember.Route.extend({
        expectedStage: 7,
        redirect: function() { Actions.completeSeasonRouteRedirect(this); }
    });

    return CompleteSeasonRoute;
});
