define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var StartSeasonActions = {
        startNewSeason: function(controller) {
            $.ajax( Constants.START_SEASON_URI, {
                type: 'POST',
                success: function(data) {
                    Globals.season = data.season;
                    controller.send('finishStage');
                },
                error: function() {
                    alert("Error starting new season!");
                }
            });
        },
        finishStage: function(controller) {
            var deferred = $.Deferred();

            controller.get("controllers.roster-cut").send('prepareData', deferred);

            deferred.promise().then(function() {

                controller.get("controllers.progress").send('nextStage');
            });
        }
    };

    return StartSeasonActions;
});
