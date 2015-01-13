define(['progress', 'ember'], function(Progress, Ember) {

    var ProgressController = Ember.ObjectController.extend({
        stage:  Progress.stage,
        stages: Progress.stages,
        actions: {
            nextStage: function() {
                if (this.stage >= this.stages.length) {
                    this.stages.setEach("status", "progress-todo");
                    this.stage = 0;

                    this.transitionToRoute(this.stages[this.stage].route);

                    this.stages[this.stage].set("status", "progress-curr");

                    return;
                }

                this.stages[this.stage].set("status", "progress-done");
                this.stage++;

                if (this.stage < this.stages.length) {
                    this.transitionToRoute(this.stages[this.stage].route);

                    this.stages[this.stage].set("status", "progress-curr");
                }
                else {
                    this.transitionToRoute("season-complete");
                }
            }
        }
    });

    return ProgressController;
});
