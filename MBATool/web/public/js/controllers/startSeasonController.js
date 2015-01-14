define(['objects/globals', 'utils', 'ember'], function(Globals, Utils, Ember) {

    var StartSeasonController = Ember.ObjectController.extend({
        needs:   ['progress', 'roster-cut'],
        actions: {
            startNewSeason: function() {
                var controller = this;
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
            loadTeams: function() {
                var controller = this;
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
            finishStage: function() {
                this.get("controllers.progress").send('nextStage');
            }
        }
    });

    return StartSeasonController;
});
