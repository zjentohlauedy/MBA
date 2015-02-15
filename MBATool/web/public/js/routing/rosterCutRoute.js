define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var RosterCutRoute = Ember.Route.extend({
        expectedStage: 1,
        redirect: function() { Actions.rosterCutRouteRedirect(this); }
    });

    return RosterCutRoute;
});
