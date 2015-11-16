define(['actions/accoladesActions', 'ember'], function(Actions, Ember) {

    var AccoladesController = Ember.ObjectController.extend({
        needs:         "progress",
        stageComplete: true,
        actions: {
            finishStage: function() { Actions.finishStage( this ); }
        }
    });

    return AccoladesController;
});
