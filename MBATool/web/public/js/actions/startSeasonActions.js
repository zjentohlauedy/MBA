define(['objects/globals', 'utils'], function(Globals, Utils) {

    var StartSeasonActions = {
        startNewSeason: function(controller) {
            $.ajax( "/mba/actions/start_season", {
                type: 'POST',
                success: function(data) {
                    Globals.season = data.season;
                    controller.send('loadTeams');
                },
                error: function() {
                    alert("Error starting new season!");
                }
            });
        },
        loadTeams: function(controller) {
            $.ajax( "/mba/resources/teams", {
                success: function(teams) {
                    controller.get('controllers.roster-cut').set( 'teams', Utils.decorateTeams(teams));
                    controller.send('finishStage');
                },
                error: function() {
                    alert("Error loading teams!")
                }
            });
        },
        finishStage: function(controller) {
            controller.get("controllers.progress").send('nextStage');
        }
    };

    return StartSeasonActions;
});
