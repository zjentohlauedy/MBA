define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var AccoladesActions = {
        prepareData: function(controller, deferred) {
            $.ajax( '/mba/resources/accolades', {
                success: function(accolades) {
                    controller.set('availableAccolades', accolades.filter(function(element) { return ! element.automatic }));
                    deferred.resolve();
                },
                error: function() {
                    alert("Error loading accolades!")
                    deferred.reject();
                }
            });
        },
        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return AccoladesActions;
});
