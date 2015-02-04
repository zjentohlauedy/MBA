define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var FreeAgentsActions = {
        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return FreeAgentsActions;
});
