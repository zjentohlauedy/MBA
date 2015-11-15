define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var ImportSeasonActions = {
        setSeason: function(controller) {
            controller.set('season', sprintf("S%02d", Globals.season));
        },

        importSeason: function(controller) {
            if (!controller.get('importReady')) {
                return;
            }

            controller.set('importReady', false);

            $.ajax( Constants.IMPORT_SEASON_URI, {
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
                    controller.set('importReady', true);
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

    return ImportSeasonActions;
});
