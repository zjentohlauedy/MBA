define(['actions/completeSeasonActions', 'ember'], function(Actions, Ember) {

    var CompleteSeasonController = Ember.ObjectController.extend({
        needs:         "progress",
        actions: {
            finishStage: function() { Actions.finishStage( this ); }
        }
    });

    return CompleteSeasonController;
});
