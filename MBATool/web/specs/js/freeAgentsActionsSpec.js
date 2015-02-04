define(['objects/constants', 'objects/globals', 'utils', 'actions/freeAgentsActions'], function(Constants, Globals, Utils, Actions) {

    describe('FreeAgentsActions', function() {

        describe('finishStage', function() {

            var controller;
            var progressController;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);

                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

                controller.get = function(name) {
                    if (name == 'controllers.progress') {
                        return progressController;
                    }

                    return null;
                }
            });

            it('should direct the progress controller to move to the next stage', function() {

                controller.stageComplete = true;

                Actions.finishStage(controller);

                expect(progressController.send).toHaveBeenCalledWith('nextStage');
            });

            it('should do nothing if the stage is not complete', function() {

                controller.stageComplete = false;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.finishStage(controller);

                expect($.ajax.calls.count()).toBe(0);
            });
        });
    });
});
