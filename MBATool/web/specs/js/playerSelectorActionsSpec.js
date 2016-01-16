define(['actions/playerSelectorActions'], function(Actions) {

    describe('playerSelectorActions', function() {

        describe('change', function() {

            var component;

            beforeEach(function() {
                component = jasmine.createSpyObj('component', ['sendAction', 'get']);

                component.get.and.callFake(function(name) { return function() {}; });
            });

            it('should send the action from the changeAction attribute', function() {

                Actions.change(component);

                expect(component.sendAction).toHaveBeenCalledWith('changeAction');
            });

            it('should not send the action if the changeAction attribute is not set', function() {

                component.get.and.callFake(function(name) { return null; });

                Actions.change(component);

                expect(component.sendAction).not.toHaveBeenCalled();
            });
        });

        describe('keyUp', function() {

            var component;
            var instance = Ember.Object.create();

            beforeEach(function() {
                component = jasmine.createSpyObj('component', ['sendAction', 'get']);

                component.lastValue = null;
                component.value     = 'a';

                component.get.and.callFake(function(name) {
                    if (name =='instance') {
                        return instance
                    }
                    return null;
                });

                spyOn(Ember.run, 'debounce').and.callFake(function() {
                    funcArgs = [];

                    for (var i = 0; i < arguments.length; ++i) {
                        funcArgs.push(arguments[i]);
                    }

                    if (funcArgs.length > 3) {
                        funcArgs[1].apply(funcArgs[0], funcArgs.slice(2,-1));
                    }
                    else {
                        funcArgs[1]();
                    }
                });
            });

            it('should call the component send action method', function() {
                Actions.keyUp(component, {});

                expect(component.sendAction).toHaveBeenCalled();
            });

            it('should pass the value "action", the value of the instance property and the component value to the action', function() {
                Actions.keyUp(component, {});

                expect(component.sendAction).toHaveBeenCalledWith('action', instance, component.value);
            });

            it('should update the last value to the current value', function() {
                component.lastValue = '123';
                component.value     = '1234';

                Actions.keyUp(component, {});

                expect(component.lastValue).toEqual('1234');
            });

            it('should not call send action if the value has not changed', function() {
                component.lastValue = '123';
                component.value     = '123';

                Actions.keyUp(component, {});

                expect(component.sendAction).not.toHaveBeenCalled();
            });

            it('should call the send action method through debounce with a 500ms delay', function() {
                Actions.keyUp(component, {});

                expect(Ember.run.debounce).toHaveBeenCalled();
                expect(Ember.run.debounce.calls.mostRecent().args.slice(-1)[0]).toEqual(500);
            });

            it('should only call the send action method through debounce', function() {
                Ember.run.debounce.and.callFake(function(){});

                Actions.keyUp(component, {});

                expect(component.sendAction).not.toHaveBeenCalled();
            });
        });
    });
});
