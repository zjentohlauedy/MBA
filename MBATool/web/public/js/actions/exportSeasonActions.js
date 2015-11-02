define(['objects/globals', 'sprintf'], function(Globals, Sprintf) {

    var ExportSeasonActions = {
        setSeason: function(controller) {
            controller.set('season', sprintf("S%02d", Globals.season));
        },

        exportSeason: function(controller) {
            controller.set('exportReady', false);
        },

        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return ExportSeasonActions;
});
