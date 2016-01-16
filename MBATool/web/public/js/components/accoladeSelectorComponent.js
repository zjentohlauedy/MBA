define(['actions/accoladeSelectorActions', 'ember'], function(Actions, Ember) {

    var AccoladeSelectorComponent = Ember.Select.extend({

        change: function() { Ember.run.debounce(Actions, 'change', this, 100); }
    });

    return AccoladeSelectorComponent;
});
