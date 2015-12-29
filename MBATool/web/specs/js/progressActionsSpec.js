define(['objects/progress', 'actions/progressActions'], function(Progress, Actions) {

    describe('ProgressActions:', function() {

        describe('nextStage', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['transitionToRoute']);

                controller.stage  = Progress.stage;
                controller.stages = Progress.stages.slice(0); // clone the array so it is reset before each test

                controller.send = function(method, data) {
                    if (method == 'goToStage') { Actions.goToStage(controller, data); }
                }
            });

            it('should increment the stage', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                Actions.nextStage(controller);

                expect(controller.stage).toBe(1);
            });

            it('should reset the stage to 0 if it goes beyond the number of stages', function() {

                controller.stage = controller.stages.length;

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                Actions.nextStage(controller);

                expect(controller.stage).toBe(0);
            });

            it('should set the previous stage status to done', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                Actions.nextStage(controller);

                expect(controller.stages[0].status).toEqual('progress-done');
            });

            it('should set the current stage status to curr', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.nextStage(controller);

                expect(controller.stages[1].status).toEqual('progress-curr');
            });

            it('should reset all but current stage status to todo if stage goes beyond the number of stages', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                controller.stage = controller.stages.length;

                controller.stages.setEach('status', 'progress-done');

                Actions.nextStage(controller);

                for ( var i = 1; i < controller.stages.length; i++ ) {
                    expect(controller.stages[i].status).toEqual('progress-todo');
                };
            });

            it('should update the organization resource with the new stage value and reset draft values', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.nextStage(controller);

                expect($.ajax).toHaveBeenCalled();
                expect($.ajax.calls.mostRecent().args[0]).toEqual('/mba/resources/organizations/1');
                expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                expect($.ajax.calls.mostRecent().args[1].data).toEqual('{"draft_round":1,"pick_number":1,"stage":1}');
            });

            it('should transition to the route of the new stage', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                Actions.nextStage(controller);

                expect(controller.transitionToRoute).toHaveBeenCalledWith(controller.stages[controller.stage].route);
            });

            it('should transition to start season route if new stage is the last', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                controller.stage = controller.stages.length - 1;

                Actions.nextStage(controller);

                expect(controller.transitionToRoute).toHaveBeenCalledWith('start-season');
            });

            it('should show an alert if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error() });

                spyOn(window, 'alert').and.callThrough();

                Actions.nextStage(controller);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('goToStage', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['transitionToRoute']);

                controller.stage  = Progress.stage;
                controller.stages = Progress.stages.slice(0); // clone the array so it is reset before each test
            });

            it('should transition to the route of the given stage', function() {

                Actions.goToStage(controller, 3);

                expect(controller.transitionToRoute).toHaveBeenCalledWith(controller.stages[3].route);
            });

            it('should update the controller stage property to the given stage', function() {

                controller.stage = 0;

                Actions.goToStage(controller, 3);

                expect(controller.stage).toBe(3);
            });

            it('should set the current stage status to curr', function() {

                Actions.goToStage(controller, 3);

                expect(controller.stages[3].status).toEqual('progress-curr');
            });

            it('should set all previous stages status to done', function() {

                Actions.goToStage(controller, 3);

                expect(controller.stages[0].status).toEqual('progress-done');
                expect(controller.stages[1].status).toEqual('progress-done');
                expect(controller.stages[2].status).toEqual('progress-done');
            });

            it('should set all later stages status to todo', function() {

                Actions.goToStage(controller, 3);

                expect(controller.stages[4].status).toEqual('progress-todo');
                expect(controller.stages[5].status).toEqual('progress-todo');
            });

            it('should show an alert if the given stage is out of range', function() {

                spyOn(window, 'alert').and.callThrough();

                Actions.goToStage(controller, 12);

                expect(window.alert).toHaveBeenCalled();
            });
        });
    });
});
