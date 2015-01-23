define([], function() {

    var ProgressActions = {
        nextStage: function(controller) {
            if (controller.stage >= controller.stages.length) {
                controller.stages.setEach("status", "progress-todo");
                controller.stage = 0;

                controller.transitionToRoute(controller.stages[controller.stage].route);

                controller.stages[controller.stage].set("status", "progress-curr");

                return;
            }

            controller.stages[controller.stage].set("status", "progress-done");
            controller.stage++;

            if (controller.stage < controller.stages.length) {
                controller.transitionToRoute(controller.stages[controller.stage].route);

                controller.stages[controller.stage].set("status", "progress-curr");
            }
            else {
                controller.transitionToRoute("season-complete");
            }
        }
    };

    return ProgressActions;
});
