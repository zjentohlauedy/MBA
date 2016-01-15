define(['actions/accoladeSelectorActions', 'ember'], function(Actions, Ember) {

    var AccoladeSelectorComponent = Ember.Select.extend({

        change: function() { Actions.change( this ); }
    });

    return AccoladeSelectorComponent;
});
