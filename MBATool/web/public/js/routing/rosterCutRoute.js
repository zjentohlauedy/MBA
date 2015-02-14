define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var RosterCutRoute = Ember.Route.extend({
        redirect: function() { Actions.rosterCutRouteRedirect(this); }
    });

    return RosterCutRoute;
});
