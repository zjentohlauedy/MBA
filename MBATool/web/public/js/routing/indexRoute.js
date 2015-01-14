define(['objects/progress', 'ember'], function(Progress, Ember) {

    var IndexRoute = Ember.Route.extend({
        redirect: function() {
            this.transitionTo(Progress.stages[Progress.stage].route);
        }
    });

    return IndexRoute;
});
