define(['objects/globals', 'utils'], function(Globals, Utils) {

    var StartSeasonActions = {
        startNewSeason: function(controller) {
            $.ajax( "/mba/actions/start_season", {
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
