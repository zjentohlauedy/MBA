define(['ember'], function(Ember) {

    var AccoladeSelectorActions = {

        change: function(component) {

            var changeAction = component.get('changeAction');

            if (changeAction) {
                component.get('controller').send(changeAction);
            }
        }
    };

    return AccoladeSelectorActions;
});
