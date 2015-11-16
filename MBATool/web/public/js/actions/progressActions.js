define(['objects/constants'], function(Constants) {

    var updateOrganization = function(data) {
        var defer = $.Deferred();

        $.ajax(Constants.ORGANIZATIONS_URI + '/1', {
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
            var orgFields = { draft_round: 1, pick_number: 1 };

            var nextStage = controller.stage + 1;

            if (controller.stage >= controller.stages.length || nextStage >= controller.stages.length) {
                orgFields.stage = 0;

                $.when(updateOrganization(orgFields)).then(
                    function(org) {
                        controller.send('goToStage', 0);
                    },
                    function() {
                        alert('Error updating organization');
                    }
                );

                return;
            }

            orgFields.stage = nextStage;

            $.when(updateOrganization(orgFields)).then(
                function(org) {
                    controller.send('goToStage', nextStage);
                },
                function() {
                    alert('Error updating organization');
                }
            );
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
