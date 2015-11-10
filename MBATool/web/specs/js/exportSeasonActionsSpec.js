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

                controller.get = function(name) {
                    if (name == 'exportReady') {
                        return true;
                    }
                }
            });

            it('should set the export ready flag on the export season controller to false', function() {

                spyOn($, 'ajax').and.callFake(function () {});

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('exportReady', false);
            });

            it('should post to the export season endpoint', function() {

                spyOn($, 'ajax').and.callFake(function () {});

                Actions.exportSeason(controller);

                expect($.ajax.calls.mostRecent().args[0]     ).toEqual('/mba/actions/export_season');
                expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
            });

            it('should set the stage complete flag to true', function() {

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.success({});
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('stageComplete', true);
            });

            it('should set the success flag to true', function() {

                var response = { status: '' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.success(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('success', true);
            });

            it('should set the error flag to false', function() {

                var response = { status: '' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.success(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('error', false);
            });

            it('should set the status message to the value of the status field in the ajax response', function() {

                var response = { status: 'This is a message.' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.success(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('statusMessage', response.status);
            });

            it('should set the error flag to true if the ajax call fails', function() {

                var response = { error: 'This is a message.' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.error(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('error', true);
            });

            it('should set the success flag to false if the ajax call fails', function() {

                var response = { error: 'This is a message.' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.error(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('success', false);
            });

            it('should set the export ready flag to true if the ajax call fails', function() {

                var response = { error: 'This is a message.' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.error(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('exportReady', true);
            });

            it('should set the status message to the value of the error in the ajax response if the ajax call fails', function() {

                var response = { error: 'This is a message.' };

                spyOn($, 'ajax').and.callFake(function (rel, options) {
                    options.error(response);
                });

                Actions.exportSeason(controller);

                expect(controller.set).toHaveBeenCalledWith('statusMessage', response.error);
            });

            it('should do nothing if exportReady is false', function() {

                spyOn($, 'ajax').and.callFake(function () {});

                controller.get = function(name) {
                    if (name == 'exportReady') {
                        return false;
                    }
                }

                Actions.exportSeason(controller);

                expect(controller.set).not.toHaveBeenCalled();
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
