define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var CompleteSeasonActions = {
        finishStage: function(controller) {
            controller.get("controllers.progress").send('nextStage');
        }
    };

    return CompleteSeasonActions;
});
