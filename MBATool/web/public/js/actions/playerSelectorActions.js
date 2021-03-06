define(['ember'], function(Ember) {

    var PlayerSelectorActions = {

        change: function(component) {

            if (component.get('changeAction')) {
                component.sendAction('changeAction');
            }
        },
        keyUp: function(component, event) {

            if (component.value === component.lastValue) { return; }

            component.lastValue = component.value;

            Ember.run.debounce(component, component.sendAction, 'action', component.get('instance'), component.value, 500);
        }
    };

    return PlayerSelectorActions;
});
