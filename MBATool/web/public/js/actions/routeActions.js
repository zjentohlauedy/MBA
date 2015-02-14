define(['objects/globals'], function(Globals) {

    var RouteActions = {
        indexRouteRedirect: function(route) {
            $.ajax('/mba/resources/organizations/1', {
                success: function(org) {
                    Globals.season = org.season;

                    route.controllerFor('progress').send('goToStage', org.stage);
                },
                error: function() {
                    alert('Error loading the organization!');
                }
            });
        },
        rosterCutRouteRedirect: function(route) {
            var defer = $.Deferred();

            route.controllerFor('roster-cut').send('prepareData', defer);
        },
        rookieDraftRouteRedirect: function(route) {
            var defer = $.Deferred();

            route.controllerFor('rookie-draft').send('prepareData', defer);
        },
        freeAgentsRouteRedirect: function(route) {
            var defer = $.Deferred();

            route.controllerFor('free-agents').send('prepareData', defer);
        }
    };

    return RouteActions;
});
