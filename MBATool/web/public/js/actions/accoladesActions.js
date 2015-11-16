define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var AccoladesActions = {
        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return AccoladesActions;
});
