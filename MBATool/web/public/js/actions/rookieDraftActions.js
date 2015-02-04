define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var RookieDraftActions = {
        finishStage: function(controller) {
            if (controller.stageComplete) {
                var deferred = $.Deferred();

                controller.get("controllers.free-agents").send('prepareData', deferred);

                deferred.promise().then(function() {

                    controller.get("controllers.progress").send('nextStage');
                });
            }
        }
    };

    return RookieDraftActions;
});
