define(['actions/routeActions', 'ember'], function(Actions, Ember) {

    var AccoladesRoute = Ember.Route.extend({
        expectedStage: 6,
        redirect: function() { Actions.accoladesRouteRedirect(this); }
    });

    return AccoladesRoute;
});
