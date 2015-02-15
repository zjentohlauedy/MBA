define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var RookieDraftRoute = Ember.Route.extend({
        expectedStage: 2,
        redirect: function() { Actions.rookieDraftRouteRedirect(this); }
    });

    return RookieDraftRoute;
});
