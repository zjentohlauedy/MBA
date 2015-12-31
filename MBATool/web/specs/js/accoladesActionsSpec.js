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

            it('should assign a unique id to each of the accolades', function() {

                var accolades = [{automatic: false},{automatic: false},{automatic: false}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(accolades);
                });

                Actions.prepareData(controller, deferred);

                expect(controller.set.calls.mostRecent().args[1][0].id).toEqual(1);
                expect(controller.set.calls.mostRecent().args[1][1].id).toEqual(2);
                expect(controller.set.calls.mostRecent().args[1][2].id).toEqual(3);
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

        describe('saveAccolades', function() {

            var controller;

            beforeEach(function() {
                controller               = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.errorMessages = jasmine.createSpyObj('errorMessages', ['addObject', 'clear']);
            });

            it('should disable the save button', function() {

                controller.canSave = true;
                controller.accoladeList = [];

                Actions.saveAccolades(controller);

                expect(controller.set).toHaveBeenCalledWith('canSave', false);
            });

            it('should clear the error messages on the controller', function() {

                controller.canSave = true;
                controller.accoladeList = [];

                Actions.saveAccolades(controller);

                expect(controller.errorMessages.clear).toHaveBeenCalled();
            });

            it('should do nothing if the save button is disabled', function() {

                controller.canSave = false;

                Actions.saveAccolades(controller);

                expect(controller.set).not.toHaveBeenCalled();
            });

            it('should post every accolade entered to the player accolades endpoint', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [{id: 1, value: 2, type: 'one'},{id: 2, value: 7, type: 'two'}]
                controller.accoladeList       = [{selectedAccolade: 1, selectedPlayer: 5},{selectedAccolade: 2, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.saveAccolades(controller);

                expect($.ajax).toHaveBeenCalled();

                expect($.ajax.calls.argsFor(0)[0]     ).toEqual('/mba/resources/players/5/accolades');
                expect($.ajax.calls.argsFor(0)[1].type).toEqual('POST');
                expect($.ajax.calls.argsFor(0)[1].contentType).toEqual('application/json');
                expect($.ajax.calls.argsFor(0)[1].data).toEqual('{"player_id":5,"season":3,"accolade":2,"type":"one"}');

                expect($.ajax.calls.argsFor(1)[0]     ).toEqual('/mba/resources/players/4/accolades');
                expect($.ajax.calls.argsFor(1)[1].type).toEqual('POST');
                expect($.ajax.calls.argsFor(1)[1].contentType).toEqual('application/json');
                expect($.ajax.calls.argsFor(1)[1].data).toEqual('{"player_id":4,"season":3,"accolade":7,"type":"two"}');
            });

            it('should call the resolveAccolades method on the controller after all ajax calls are successful', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [];
                controller.accoladeList       = [{selectedAccolade: 2, selectedPlayer: 5},{selectedAccolade: 7, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success() });

                Actions.saveAccolades(controller);

                expect(controller.send).toHaveBeenCalledWith('resolveAccolades');
            });

            it('should not call the resolveAccolades method if any ajax calls fail', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [];
                controller.accoladeList       = [{selectedAccolade: 2, selectedPlayer: 5},{selectedAccolade: 7, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (options.data.player_id == 5)
                        options.success();
                    else
                        options.error({responseText: '{"error":"some error message"}'});
                });

                Actions.saveAccolades(controller);

                expect(controller.send).not.toHaveBeenCalledWith('resolveAccolades');
            });

            it('should add messages to the error messages on the controller if any ajax calls fail', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [];
                controller.accoladeList       = [{selectedAccolade: 2, selectedPlayer: 5},{selectedAccolade: 7, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (options.data.player_id == 5)
                        options.success();
                    else
                        options.error({responseText: '{"error":"some error message"}'});
                });

                Actions.saveAccolades(controller);

                expect(controller.errorMessages.addObject).toHaveBeenCalledWith("some error message");
            });

            it('should add unknown error message to the error messages if the error response is not json', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [];
                controller.accoladeList       = [{selectedAccolade: 2, selectedPlayer: 5},{selectedAccolade: 7, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (options.data.player_id == 5)
                        options.success();
                    else
                        options.error({responseText: '<html></html>'});
                });

                Actions.saveAccolades(controller);

                expect(controller.errorMessages.addObject).toHaveBeenCalledWith("Unknown error, check console log.");
            });

            it('should log actual response on error if the response is not json', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [];
                controller.accoladeList       = [{selectedAccolade: 2, selectedPlayer: 5},{selectedAccolade: 7, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (options.data.player_id == 5)
                        options.success();
                    else
                        options.error({responseText: '<html></html>'});
                });

                spyOn(console, 'error').and.callFake(function() {});

                Actions.saveAccolades(controller);

                expect(console.error).toHaveBeenCalled();
                expect(console.error.calls.mostRecent().args[0]).toMatch('<html></html>');
            });

            it('should re-enable the save button if the ajax call fails', function() {

                Globals.season                = 3;
                controller.canSave            = true;
                controller.availableAccolades = [];
                controller.accoladeList       = [{selectedAccolade: 2, selectedPlayer: 5},{selectedAccolade: 7, selectedPlayer: 4}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (options.data.player_id == 5)
                        options.success();
                    else
                        options.error({responseText: '{"error":"some error message"}'});
                });

                Actions.saveAccolades(controller);

                expect(controller.set).toHaveBeenCalledWith('canSave', true);
            });
        });

        describe('resolveAccolades', function() {

            var controller;

            beforeEach(function() {
                controller               = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.errorMessages = jasmine.createSpyObj('errorMessages', ['addObject', 'clear']);
            });

            it('should post to the resolve accolades action endpoint', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.resolveAccolades(controller);

                expect($.ajax).toHaveBeenCalled();

                expect($.ajax.calls.argsFor(0)[0]     ).toEqual('/mba/actions/resolve_accolades');
                expect($.ajax.calls.argsFor(0)[1].type).toEqual('POST');
            });

            it('should call the display accolades controller method with the response from the action endpoint', function() {

                var response = [{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(response); });

                Actions.resolveAccolades(controller);

                expect(controller.send).toHaveBeenCalledWith('displayAccolades', response);
            });

            it('should not call the display accolades method if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error({responseText: '{"error":"some error message"}'}); });

                Actions.resolveAccolades(controller);

                expect(controller.send).not.toHaveBeenCalled();
            });

            it('should add message to the error messages on the controller if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error({responseText: '{"error":"some error message"}'}); });

                Actions.resolveAccolades(controller);

                expect(controller.errorMessages.addObject).toHaveBeenCalledWith("some error message");
            });

            it('should add unknown error message to the error messages if the error response is not json', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error({responseText: '<html></html>'}); });

                Actions.resolveAccolades(controller);

                expect(controller.errorMessages.addObject).toHaveBeenCalledWith("Unknown error, check console log.");
            });

            it('should log actual response if the error response is not json', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error({responseText: '<html></html>'}); });
                spyOn(console, 'error').and.callFake(function() {});

                Actions.resolveAccolades(controller);

                expect(console.error).toHaveBeenCalled();
                expect(console.error.calls.mostRecent().args[0]).toMatch('<html></html>');
            });

            it('should re-enable the save button if the ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error({responseText: '{"error":"some error message"}'}); });

                Actions.resolveAccolades(controller);

                expect(controller.set).toHaveBeenCalledWith('canSave', true);
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
