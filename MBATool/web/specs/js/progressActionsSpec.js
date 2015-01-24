define(['objects/progress', 'actions/progressActions'], function(Progress, Actions) {

    describe('ProgressActions:', function() {

        describe('nextStage', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['transitionToRoute']);

                controller.stage  = Progress.stage;
                controller.stages = Progress.stages.slice(0); // clone the array so it is reset before each test
            });

            it('should increment the stage', function() {

                Actions.nextStage(controller);

                expect(controller.stage).toBe(1);
            });

            it('should reset the stage to 0 if it goes beyond the number of stages', function() {

                controller.stage = controller.stages.length;

                Actions.nextStage(controller);

                expect(controller.stage).toBe(0);
            });

            it('should set the previous stage status to done', function() {

                Actions.nextStage(controller);

                expect(controller.stages[0].status).toEqual('progress-done');
            });

            it('should set the current stage status to curr', function() {

                Actions.nextStage(controller);

                expect(controller.stages[1].status).toEqual('progress-curr');
            });

            it('should reset all but current stage status to todo if stage goes beyond the number of stages', function() {

                controller.stage = controller.stages.length;

                controller.stages.setEach('status', 'progress-done');

                Actions.nextStage(controller);

                for ( var i = 1; i < controller.stages.length; i++ ) {
                    expect(controller.stages[i].status).toEqual('progress-todo');
                };
            });

            it('should transition to the route of the new stage', function() {

                Actions.nextStage(controller);

                expect(controller.transitionToRoute).toHaveBeenCalledWith(controller.stages[controller.stage].route);
            });

            it('should transition to season complete route if new stage is the last', function() {

                controller.stage = controller.stages.length - 1;

                Actions.nextStage(controller);

                expect(controller.transitionToRoute).toHaveBeenCalledWith('season-complete');
            });
        });
    });
});
