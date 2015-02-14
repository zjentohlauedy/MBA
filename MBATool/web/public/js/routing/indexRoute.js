define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var IndexRoute = Ember.Route.extend({
        redirect: function() { Actions.indexRouteRedirect(this); }
    });

    return IndexRoute;
});
