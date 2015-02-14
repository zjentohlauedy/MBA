define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var FreeAgentsRoute = Ember.Route.extend({
        redirect: function() { Actions.freeAgentsRouteRedirect(this); }
    });

    return FreeAgentsRoute;
});
