define(['actions/startSeasonActions', 'ember'], function(Actions, Ember) {

    var StartSeasonController = Ember.ObjectController.extend({
        needs:   ['progress', 'roster-cut'],
        actions: {
            startNewSeason: function() { Actions.startNewSeason(this); },
            loadTeams:      function() { Actions.loadTeams(this);      },
            finishStage:    function() { Actions.finishStage(this)     }
        }
    });

    return StartSeasonController;
});
