define(['actions/accoladesActions', 'ember'], function(Actions, Ember) {

    var AccoladesController = Ember.ObjectController.extend({
        needs:              "progress",
        canSave:            true,
        stageComplete:      false,
        availableAccolades: [],
        accoladeList:       [ { selectedAccolade: null, selectedPlayer: null } ],
        errorMessages:      [],
        actions: {
            addAccolade: function() {
                var accolade = { selectedAccolade: null, selectedPlayer: null };

                this.accoladeList.addObject( accolade );
            },
            removeAccolade: function(accolade) {
                this.accoladeList.removeObject( accolade );
            },
            prepareData: function(deferred) { Actions.prepareData( this, deferred ); },
            saveAccolades: function() { Actions.saveAccolades( this ); },
            resolveAccolades: function() { Actions.resolveAccolades( this ); },
            displayAccolades: function() { alert('displaying accolades!'); },
            finishStage: function() { Actions.finishStage( this ); }
        }
    });

    return AccoladesController;
});
