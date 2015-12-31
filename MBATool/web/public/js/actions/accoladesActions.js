define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var AccoladesActions = {
        prepareData: function(controller, deferred) {
            $.ajax( '/mba/resources/accolades', {
                success: function(accolades) {
                    filteredAccolades = accolades.filter(function(element) { return ! element.automatic });

                    for (var i = 0; i < filteredAccolades.length; ++i) {
                        filteredAccolades[i].id = i + 1;
                    }

                    controller.set('availableAccolades', filteredAccolades);
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
