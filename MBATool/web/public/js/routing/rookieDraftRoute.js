define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var RookieDraftRoute = Ember.Route.extend({
        redirect: function() { Actions.rookieDraftRouteRedirect(this); }
    });

    return RookieDraftRoute;
});
