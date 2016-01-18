define(['actions/importSeasonActions', 'ember'], function(Actions, Ember) {

    var ImportSeasonController = Ember.ObjectController.extend({
        needs:         "progress",
        season:        null,
        success:       false,
        error:         false,
        statusMessage: 'Not Imported.',
        importReady:   true,
        stageComplete: false,
        actions: {
            setSeason:    function() { Actions.setSeason(    this ); },
            importSeason: function() { Actions.importSeason( this ); },
            finishStage:  function() { Actions.finishStage(  this ); }
        }
    });

    return ImportSeasonController;
});
