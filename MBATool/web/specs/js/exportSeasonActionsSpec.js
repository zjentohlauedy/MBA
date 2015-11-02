define(['objects/globals', 'actions/exportSeasonActions'], function(Globals, Actions) {

    describe('ExportSeasonActions', function() {

        describe('setSeason', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            });

            it('should set the season on the export season controller', function() {

                Actions.setSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('season', jasmine.any(String));
            });

            it('should convert the season to a string representation', function() {

                Globals.season = 2;

                Actions.setSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('season', 'S02');
            });

            it('should correctly convert a two digit season', function() {

                Globals.season = 14;

                Actions.setSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('season', 'S14');
            });
        });

        describe('exportSeason', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            });

            it('should set the export ready flag on the export season controller to false', function() {

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('exportReady', false);
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
