define(['ember'], function(Ember) {

    var AccoladeSelectorActions = {

        change: function(component) {

            var checkstate = component.get('checkstate');

            if (checkstate) {
                component.get('controller').send(checkstate);
            }
        }
    };

    return AccoladeSelectorActions;
});
