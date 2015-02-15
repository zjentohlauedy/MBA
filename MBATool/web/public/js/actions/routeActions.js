define(['objects/globals'], function(Globals) {

    var loadOrganization = function(org_id) {
        var deferred = $.Deferred();

        $.ajax('/mba/resources/organizations/' + org_id, {
            success: function(org) {
                deferred.resolve(org)
            },
            error: function() {
                deferred.reject();
            }
        });

        return deferred.promise();
    };

    var RouteActions = {
        indexRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    route.controllerFor('progress').send('goToStage', org.stage);
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        seasonStartRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    if (org.stage !== route.expectedStage) {
                        route.controllerFor('progress').send('goToStage', org.stage);
                    }
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        rosterCutRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    if (org.stage === route.expectedStage) {
                        var defer = $.Deferred();

                        route.controllerFor('roster-cut').send('prepareData', defer);
                    }
                    else {
                        route.controllerFor('progress').send('goToStage', org.stage);
                    }
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        rookieDraftRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    if (org.stage === route.expectedStage) {
                        var defer = $.Deferred();

                        route.controllerFor('rookie-draft').send('prepareData', defer);
                    }
                    else {
                        route.controllerFor('progress').send('goToStage', org.stage);
                    }
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        freeAgentsRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    if (org.stage === route.expectedStage) {
                        var defer = $.Deferred();

                        route.controllerFor('free-agents').send('prepareData', defer);
                    }
                    else {
                        route.controllerFor('progress').send('goToStage', org.stage);
                    }
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        exportSeasonRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    if (org.stage !== route.expectedStage) {
                        route.controllerFor('progress').send('goToStage', org.stage);
                    }
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        importSeasonRouteRedirect: function(route) {
            $.when(loadOrganization(1)).then(
                function(org) {
                    Globals.season = org.season;

                    if (org.stage !== route.expectedStage) {
                        route.controllerFor('progress').send('goToStage', org.stage);
                    }
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        }
    };

    return RouteActions;
});
