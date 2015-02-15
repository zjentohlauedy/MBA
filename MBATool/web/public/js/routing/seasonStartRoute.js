define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var SeasonStartRoute = Ember.Route.extend({
        expectedStage: 0,
        redirect: function() { Actions.seasonStartRouteRedirect(this); }
    });

    return SeasonStartRoute;
});
