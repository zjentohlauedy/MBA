define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var FreeAgentsRoute = Ember.Route.extend({
        expectedStage: 3,
        redirect: function() { Actions.freeAgentsRouteRedirect(this); }
    });

    return FreeAgentsRoute;
});
