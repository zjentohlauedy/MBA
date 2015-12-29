define(['objects/globals', 'actions/accoladesActions'], function(Globals, Actions) {

    describe('AccoladesActions', function() {

        describe('prepareData', function() {

            var controller;
            var deferred;

            beforeEach(function() {
                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);

                deferred = jasmine.createSpyObj('deferred', ['resolve','reject']);
            });

            it('should send a request to the accolades resource', function() {
                spyOn($, 'ajax').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/accolades', jasmine.any(Object));
            });

            it('should set the available accolades on the controller', function() {

                var accolades = [{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(accolades);
                });

                Actions.prepareData(controller, deferred);

                expect(controller.set).toHaveBeenCalledWith('availableAccolades', accolades);
            });

            it('should filter out the automatic accolades before setting the available accolades', function() {

                var accolades = [{automatic: true},{automatic: true},{automatic: false}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(accolades);
                });

                Actions.prepareData(controller, deferred);

                expect(controller.set.calls.mostRecent().args[1].length).toEqual(1);
            });

            it('should resolve the given promise when the work is complete', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.resolve).toHaveBeenCalled();
            });

            it('should reject the promise if the accolades ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the ajax call failed', function() {

                spyOn(window, 'alert').and.callThrough();

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });
        });

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
