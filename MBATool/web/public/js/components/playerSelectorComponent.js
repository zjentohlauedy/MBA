define(['actions/playerSelectorActions', 'ember'], function(Actions, Ember) {

    var PlayerSelectorComponent = Ember.TextField.extend({

        attributeBindings: ['instance'],
        lastValue:           null,

        keyUp:  function(event) { Actions.keyUp( this, event); },
        change: function(event) { Actions.change(this       ); }
    });

    return PlayerSelectorComponent;
});
