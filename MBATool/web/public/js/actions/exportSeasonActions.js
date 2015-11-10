define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var ExportSeasonActions = {
        setSeason: function(controller) {
            controller.set('season', sprintf("S%02d", Globals.season));
        },

        exportSeason: function(controller) {
            if (!controller.get('exportReady')) {
                return;
            }

            controller.set('exportReady', false);

            $.ajax( Constants.EXPORT_SEASON_URI, {
                type: 'POST',
                success: function(data) {
                    controller.set('success',       true);
                    controller.set('error',         false);
                    controller.set('stageComplete', true);
                    controller.set('statusMessage', data.status);
                },
                error: function(data) {
                    controller.set('error',       true);
                    controller.set('success',     false);
                    controller.set('exportReady', true);
                    controller.set('statusMessage', data.error);
                }
            });
        },

        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return ExportSeasonActions;
});
