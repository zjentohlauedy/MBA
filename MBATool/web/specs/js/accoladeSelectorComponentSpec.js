define(['actions/accoladeSelectorActions'], function(Actions) {

    describe('accoladeSelectorActions', function() {

        describe('change', function() {

            var controller;
            var component;

            beforeEach(function() {
                component = jasmine.createSpyObj('component', ['sendAction', 'get']);
                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);

                component.get.and.callFake(function(name) {
                    if (name == 'controller') {
                        return controller;
                    }
                    else if (name == 'changeAction') {
                        return 'changeAction-method';
                    }
                });
            });

            it('should send the method referenced in the changeAction attribute to the controller', function() {

                Actions.change(component);

                expect(controller.send).toHaveBeenCalledWith('changeAction-method');
            });

            it('should not send anything to the controller method if changeAction is not set', function() {

                component.get.and.callFake(function(name) {
                    if (name == 'controller') {
                        return controller;
                    }
                    else if (name == 'changeAction') {
                        return null;
                    }
                });

                Actions.change(component);

                expect(controller.send).not.toHaveBeenCalled();
            });
        });
    });
});
