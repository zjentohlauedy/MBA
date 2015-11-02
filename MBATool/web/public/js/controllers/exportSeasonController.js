define(['actions/exportSeasonActions', 'ember'], function(Actions, Ember) {

    var ExportSeasonController = Ember.ObjectController.extend({
        needs:         "progress",
        season:        null,
        exportReady:   true,
        stageComplete: true,
        actions: {
            setSeason:    function() { Actions.setSeason(    this ); },
            exportSeason: function() { Actions.exportSeason( this ); },
            finishStage:  function() { Actions.finishStage(  this ); }
        }
    });

    return ExportSeasonController;
});