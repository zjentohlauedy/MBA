define([], function() {

    var ProgressActions = {
        nextStage: function(controller) {
            if (controller.stage >= controller.stages.length) {

                controller.send('goToStage', 0);
                return;
            }

            if ((controller.stage + 1) < controller.stages.length) {
                controller.send('goToStage', controller.stage + 1);
                return;
            }

            controller.transitionToRoute("season-complete");
        },
        goToStage: function(controller, stage) {
            if ( stage < 0  ||  stage > controller.stages.length ) {
                alert('Stage is out of range!');
                return;
            }

            controller.stage = stage;

            controller.transitionToRoute(controller.stages[controller.stage].route);

            for (var i = 0; i < controller.stages.length; i++) {
                if      (i < stage) { controller.stages[i].set("status", "progress-done"); }
                else if (i > stage) { controller.stages[i].set("status", "progress-todo"); }
                else                { controller.stages[i].set("status", "progress-curr"); }
            }
        }
    };

    return ProgressActions;
});
