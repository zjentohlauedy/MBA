define(['objects/constants', 'objects/globals', 'utils', 'actions/rookieDraftActions'], function(Constants, Globals, Utils, Actions) {

    describe('RookieDraftActions', function() {

        describe('finishStage', function() {

            var controller;
            var progressController;
            //var freeAgentsController;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);
                //freeAgentsController = jasmine.createSpyObj('freeAgentsController', ['send', 'get', 'set']);

                controller.get = function(name) {
                    if (name == 'controllers.progress') {
                        return progressController;
                    }
/*
                    else if (name == 'controllers.free-agents') {
                        return freeAgentsController;
                    }
*/
                    return null;
                }
            });
/*
            it('should direct the free agents controller to prepare data and pass it a deferred object', function() {

                var fakeDeferred = {promise: function(){ return{then: function(){}}; }};
                controller.stageComplete = true;

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(freeAgentsController.send).toHaveBeenCalledWith('prepareData', fakeDeferred);
            });
*/
            it('should direct the progress controller to move to the next stage when the free agents controller is finished preparing data', function() {

                var fakeDeferred = {promise: function(){ return{then: function(callback){ callback(); }}; }};
                controller.stageComplete = true;

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(progressController.send).toHaveBeenCalledWith('nextStage');
            });
/*
            it('should not direct the progress controller to move to the next stage if there is an error preparing data', function() {

                var fakeDeferred = {promise: function(){ return{then: function(){}}; }};
                controller.stageComplete = true;

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(progressController.send).not.toHaveBeenCalled();
            });
*/
            it('should do nothing if the stage is not complete', function() {

                controller.stageComplete = false;

                spyOn(controller, 'get').and.callThrough();

                Actions.finishStage(controller);

                expect(controller.get.calls.count()).toBe(0);
            });
        });
    });
});
