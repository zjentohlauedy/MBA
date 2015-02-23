define(['objects/globals', 'actions/routeActions'], function(Globals, Actions) {

    describe('RouteActions', function() {

        describe('indexRouteRedirect', function() {

            var route;
            var progressController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.indexRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5});
                });

                Actions.indexRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should direct the progress controller to go to the stage in the organization resource', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.indexRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 3);
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.indexRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('seasonStartRouteRedirect', function() {

            var route;
            var progressController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

                route.expectedStage = 0;
                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.seasonStartRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5});
                });

                Actions.seasonStartRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should direct the progress controller to go to the stage in the organization resource', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.seasonStartRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 3);
            });

            it('should not direct the progress controller to go to any stage if the organization stage is for start season', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 0});
                });

                Actions.seasonStartRouteRedirect(route);

                expect(progressController.send).not.toHaveBeenCalled();
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.seasonStartRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('rosterCutRouteRedirect', function() {

            var route;
            var progressController;
            var rosterCutController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);
                rosterCutController = jasmine.createSpyObj('rosterCutController', ['send', 'get', 'set']);

                route.expectedStage = 1;
                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    } else if (name == 'roster-cut') {
                        return rosterCutController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.rosterCutRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 1});
                });

                Actions.rosterCutRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should direct the roster cut controller to prepare data', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 1});
                });

                Actions.rosterCutRouteRedirect(route);

                expect(rosterCutController.send).toHaveBeenCalledWith('prepareData', jasmine.any(Object));
            });

            it('should direct the progress controller to go to the stage in the organization resource if the stage is not roster cut', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.rosterCutRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 3);
            });

            it('should not direct the roster cut controller to prepare data if the stage is not roster cut', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.rosterCutRouteRedirect(route);

                expect(rosterCutController.send).not.toHaveBeenCalled();
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.rosterCutRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('rookieDraftRouteRedirect', function() {

            var route;
            var progressController;
            var rookieDraftController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);
                rookieDraftController = jasmine.createSpyObj('rookieDraftController', ['send', 'get', 'set']);

                route.expectedStage = 2;
                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    } else if (name == 'rookie-draft') {
                        return rookieDraftController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.rookieDraftRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 2});
                });

                Actions.rookieDraftRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should set the draft round, pick number and current team idx on the rookie draft controller', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 2, draft_round: 2, pick_number: 15});
                });

                Actions.rookieDraftRouteRedirect(route);

                expect(rookieDraftController.set).toHaveBeenCalledWith('draftRound',      2);
                expect(rookieDraftController.set).toHaveBeenCalledWith('pickNumber',     15);
                expect(rookieDraftController.set).toHaveBeenCalledWith('currentTeamIdx', 46);
            });

            it('should direct the rookie draft controller to prepare data', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 2});
                });

                Actions.rookieDraftRouteRedirect(route);

                expect(rookieDraftController.send).toHaveBeenCalledWith('prepareData', jasmine.any(Object));
            });

            it('should direct the progress controller to go to the stage in the organization resource if the stage is not rookie draft', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.rookieDraftRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 3);
            });

            it('should not direct the roster cut controller to prepare data if the stage is not rookie draft', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.rookieDraftRouteRedirect(route);

                expect(rookieDraftController.send).not.toHaveBeenCalled();
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.rookieDraftRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('freeAgentsRouteRedirect', function() {

            var route;
            var progressController;
            var freeAgentsController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);
                freeAgentsController = jasmine.createSpyObj('freeAgentsController', ['send', 'get', 'set']);

                route.expectedStage = 3;
                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    } else if (name == 'free-agents') {
                        return freeAgentsController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.freeAgentsRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.freeAgentsRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should set the draft round, pick number and current team idx on the rookie draft controller', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3, draft_round: 7, pick_number: 24});
                });

                Actions.freeAgentsRouteRedirect(route);

                expect(freeAgentsController.set).toHaveBeenCalledWith('draftRound',       7);
                expect(freeAgentsController.set).toHaveBeenCalledWith('pickNumber',      24);
                expect(freeAgentsController.set).toHaveBeenCalledWith('currentTeamIdx', 215);
            });

            it('should direct the rookie draft controller to prepare data', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.freeAgentsRouteRedirect(route);

                expect(freeAgentsController.send).toHaveBeenCalledWith('prepareData', jasmine.any(Object));
            });

            it('should direct the progress controller to go to the stage in the organization resource if the stage is not free agents', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 1});
                });

                Actions.freeAgentsRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 1);
            });

            it('should not direct the roster cut controller to prepare data if the stage is not free agents', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 1});
                });

                Actions.freeAgentsRouteRedirect(route);

                expect(freeAgentsController.send).not.toHaveBeenCalled();
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.freeAgentsRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('exportSeasonRouteRedirect', function() {

            var route;
            var progressController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

                route.expectedStage = 4;
                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.exportSeasonRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5});
                });

                Actions.exportSeasonRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should direct the progress controller to go to the stage in the organization resource', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.exportSeasonRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 3);
            });

            it('should not direct the progress controller to go to any stage if the organization stage is for export season', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 4});
                });

                Actions.exportSeasonRouteRedirect(route);

                expect(progressController.send).not.toHaveBeenCalled();
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.exportSeasonRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('importSeasonRouteRedirect', function() {

            var route;
            var progressController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

                route.expectedStage = 5;
                route.controllerFor.and.callFake(function(name) {
                    if (name == 'progress') {
                        return progressController;
                    }
                });
            });

            it('should send a request to the organizations resource', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.importSeasonRouteRedirect(route);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/organizations/1', jasmine.any(Object));
            });

            it('should set the global season to the season in the organization resource', function() {

                Globals.season = 1;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5});
                });

                Actions.importSeasonRouteRedirect(route);

                expect(Globals.season).toBe(5);
            });

            it('should direct the progress controller to go to the stage in the organization resource', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 3});
                });

                Actions.importSeasonRouteRedirect(route);

                expect(progressController.send).toHaveBeenCalledWith('goToStage', 3);
            });

            it('should not direct the progress controller to go to any stage if the organization stage is for import season', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5, stage: 5});
                });

                Actions.importSeasonRouteRedirect(route);

                expect(progressController.send).not.toHaveBeenCalled();
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.importSeasonRouteRedirect(route);

                expect(window.alert).toHaveBeenCalled();
            });
        });
    });
});
