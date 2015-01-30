define(['objects/globals', 'utils', 'actions/startSeasonActions'], function(Globals, Utils, Actions) {

    describe('StartSeasonActions:', function() {

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

            it('should call the finishStage action', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success({season: 5});
                });

                Actions.startNewSeason(controller);

                expect(controller.send).toHaveBeenCalledWith('finishStage');
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

        describe('finishStage', function() {

            var controller;
            var progressController;
            var rosterCutController;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);
                rosterCutController = jasmine.createSpyObj('rosterCutController', ['send', 'get', 'set']);

                controller.get = function(name) {
                    if (name == 'controllers.progress') {
                        return progressController;
                    }
                    else if (name == 'controllers.roster-cut') {
                        return rosterCutController;
                    }

                    return null;
                }
            });

            it('should direct the roster cut controller to prepare data and pass it a deferred object', function() {

                var fakeDeferred = {promise: function(){ return{then: function(){}}; }};

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(rosterCutController.send).toHaveBeenCalledWith('prepareData', fakeDeferred);
            });

            it('should direct the progress controller to move to the next stage when the roster cut controller is finished preparing data', function() {

                var fakeDeferred = {promise: function(){ return{then: function(callback){ callback(); }}; }};

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(progressController.send).toHaveBeenCalledWith('nextStage');
            });

            it('should not direct the progress controller to move to the next stage if there is an error preparing data', function() {

                var fakeDeferred = {promise: function(){ return{then: function(){}}; }};

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(progressController.send).not.toHaveBeenCalled();
            });
        });
    });
});
