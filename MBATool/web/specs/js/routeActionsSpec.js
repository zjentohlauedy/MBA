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

        describe('rosterCutRouteRedirect', function() {

            var route;
            var rosterCutController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                rosterCutController = jasmine.createSpyObj('rosterCutController', ['send', 'get', 'set']);

                route.controllerFor.and.callFake(function(name) {
                    if (name == 'roster-cut') {
                        return rosterCutController;
                    }
                });
            });

            it('should direct the roster cut controller to prepare data', function() {

                Actions.rosterCutRouteRedirect(route);

                expect(rosterCutController.send).toHaveBeenCalledWith('prepareData', jasmine.any(Object));
            });
        });

        describe('rookieDraftRouteRedirect', function() {

            var route;
            var rookieDraftController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                rookieDraftController = jasmine.createSpyObj('rookieDraftController', ['send', 'get', 'set']);

                route.controllerFor.and.callFake(function(name) {
                    if (name == 'rookie-draft') {
                        return rookieDraftController;
                    }
                });
            });

            it('should direct the rookie draft controller to prepare data', function() {

                Actions.rookieDraftRouteRedirect(route);

                expect(rookieDraftController.send).toHaveBeenCalledWith('prepareData', jasmine.any(Object));
            });
        });

        describe('freeAgentsRouteRedirect', function() {

            var route;
            var freeAgentsController;

            beforeEach(function() {

                route = jasmine.createSpyObj('route', ['controllerFor']);
                freeAgentsController = jasmine.createSpyObj('freeAgentsController', ['send', 'get', 'set']);

                route.controllerFor.and.callFake(function(name) {
                    if (name == 'free-agents') {
                        return freeAgentsController;
                    }
                });
            });

            it('should direct the rookie draft controller to prepare data', function() {

                Actions.freeAgentsRouteRedirect(route);

                expect(freeAgentsController.send).toHaveBeenCalledWith('prepareData', jasmine.any(Object));
            });
        });
    });
});
