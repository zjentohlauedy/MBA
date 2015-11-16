define(['objects/constants', 'objects/globals'], function(Constants, Globals) {

    var loadOrganization = function(org_id) {
        var deferred = $.Deferred();

        $.ajax(Constants.ORGANIZATIONS_URI + '/' + org_id, {
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

                        var rookieDraftController = route.controllerFor('rookie-draft');

                        var currentTeamIdx = ((org.draft_round - 1) * Constants.PICKS_PER_ROUND) + (org.pick_number - 1);

                        rookieDraftController.set('draftRound',     org.draft_round);
                        rookieDraftController.set('pickNumber',     org.pick_number);
                        rookieDraftController.set('currentTeamIdx', currentTeamIdx);

                        rookieDraftController.send('prepareData', defer);
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

                        var freeAgentsController = route.controllerFor('free-agents');

                        var currentTeamIdx = ((org.draft_round - 1) * Constants.PICKS_PER_ROUND) + (org.pick_number - 1);

                        freeAgentsController.set('draftRound',     org.draft_round);
                        freeAgentsController.set('pickNumber',     org.pick_number);
                        freeAgentsController.set('currentTeamIdx', currentTeamIdx);

                        freeAgentsController.send('prepareData', defer);
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

                    var exportSeasonController = route.controllerFor('export-season');

                    exportSeasonController.send('setSeason');
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

                    var importSeasonController = route.controllerFor('import-season');

                    importSeasonController.send('setSeason');
                },
                function() {
                    alert('Error loading the organization!');
                }
            );
        },
        accoladesRouteRedirect: function(route) {
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
        completeSeasonRouteRedirect: function(route) {
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
