define(['objects/globals', 'utils', 'actions/startSeasonActions'], function(Globals, Utils, Actions) {

    describe('startNewSeason', function() {

        var controller;

        beforeEach(function() {

            controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
        });

        it('should post to the start season endpoint', function() {

            spyOn($, 'ajax').and.callFake(function () {});

            Actions.startNewSeason(controller);

            expect($.ajax.calls.mostRecent().args[0]     ).toEqual('/mba/actions/start_season');
            expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
        });

        it('should set the global season value', function() {

            spyOn($, 'ajax').and.callFake(function (rel, options) {
                options.success({ season: 5 });
            });

            Globals.season = 0;

            Actions.startNewSeason(controller);

            expect(Globals.season).toBe(5);
        });

        it('should load the teams', function() {

            spyOn($, 'ajax').and.callFake(function (rel, options) {
                options.success({ season: 5 });
            });

            Actions.startNewSeason(controller);

            expect(controller.send).toHaveBeenCalledWith('loadTeams');
        });

        it('should show an alert if the ajax call fails', function() {

            spyOn(window, 'alert').and.callThrough();

            spyOn($, 'ajax').and.callFake(function (rel, options) {
                options.error();
            });

            Actions.startNewSeason(controller);

            expect(window.alert).toHaveBeenCalled();
        });
    });

    describe('loadTeams', function() {

        var controller;
        var rosterCutController;

        beforeEach(function() {

            controller          = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            rosterCutController = jasmine.createSpyObj('rosterCutController', ['send', 'get', 'set']);

            controller.get = function(name) {
                if (name == 'controllers.roster-cut') {
                    return rosterCutController;
                }

                return null;
            }
        });

        it('should send a request to the teams resource', function() {

            spyOn($, 'ajax').and.callFake(function() {});

            Actions.loadTeams(controller);

            expect($.ajax).toHaveBeenCalledWith('/mba/resources/teams', jasmine.any(Object));
        });

        it('should set the teams on the roster-cut controller', function() {

            var teams = [{},{},{}];

            spyOn($, 'ajax').and.callFake(function(rel, options) {
                options.success(teams);
            });

            spyOn(Utils, 'decorateTeams').and.callFake(function(teams) { return teams });

            Actions.loadTeams(controller);

            expect(rosterCutController.set).toHaveBeenCalledWith('teams', teams);
        });

        it('should decorate the teams retrieved from the teams resource', function() {

            var teams = [{},{},{}];

            spyOn($, 'ajax').and.callFake(function(rel, options) {
                options.success(teams);
            });

            spyOn(Utils, 'decorateTeams').and.callThrough();

            Actions.loadTeams(controller);

            expect(Utils.decorateTeams).toHaveBeenCalledWith(teams);
        });

        it('should call the finishStage action', function() {

            var teams = [{},{},{}];

            spyOn($, 'ajax').and.callFake(function(rel, options) {
                options.success(teams);
            });

            Actions.loadTeams(controller);

            expect(controller.send).toHaveBeenCalledWith('finishStage');
        });

        it('should show an alert if the ajax call faile', function() {

            spyOn(window, 'alert').and.callThrough();

            spyOn($, 'ajax').and.callFake(function(rel, options) {
                options.error();
            });

            Actions.loadTeams(controller);

            expect(window.alert).toHaveBeenCalled();
        });
    });

    describe('finishStage', function() {

        it('should call the nextStage action on the progress controller', function() {

            var controller         = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            var progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

            controller.get = function(name) {
                if (name == 'controllers.progress') {
                    return progressController;
                }

                return null;
            }

            Actions.finishStage(controller);

            expect(progressController.send).toHaveBeenCalledWith('nextStage');
        });
    });
});
