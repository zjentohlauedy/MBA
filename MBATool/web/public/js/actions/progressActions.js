define([], function() {

    var updateOrganization = function(data) {
        var defer = $.Deferred();

        $.ajax('/mba/resources/organizations/1', {
            type: 'POST',
            data: JSON.stringify(data),
            success: function(org) {
                defer.resolve(org);
            },
            error: function() {
                defer.reject();
            }
        });

        return defer.promise();
    };

    var ProgressActions = {
        nextStage: function(controller) {
            if (controller.stage >= controller.stages.length) {
                $.when(updateOrganization({stage: 0})).then(
                    function(org) {
                        controller.send('goToStage', 0);
                    },
                    function() {
                        alert('Error updating organization');
                    }
                );

                return;
            }

            if ((controller.stage + 1) < controller.stages.length) {
                $.when(updateOrganization({stage: controller.stage + 1})).then(
                    function(org) {
                        controller.send('goToStage', controller.stage + 1);
                    },
                    function() {
                        alert('Error updating organization');
                    }
                );

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
