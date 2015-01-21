define(['objects/progress', 'actions/progressActions', 'ember'], function(Progress, Actions, Ember) {

    var ProgressController = Ember.ObjectController.extend({
        stage:  Progress.stage,
        stages: Progress.stages,
        actions: {
            nextStage: function() { Actions.nextStage(this); }
        }
    });

    return ProgressController;
});
