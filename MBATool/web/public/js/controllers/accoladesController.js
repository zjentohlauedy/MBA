define(['actions/accoladesActions', 'ember'], function(Actions, Ember) {

    var AccoladesController = Ember.ObjectController.extend({
        needs:              "progress",
        pageReady:          false,
        stageComplete:      true,
        availableAccolades: [],
        accoladeList:       [ { selectedAccolade: null, selectedPlayer: null } ],
        actions: {
            addAccolade: function() {
                var accolade = { selectedAccolade: null, selectedPlayer: null };

                this.accoladeList.addObject( accolade );
            },
            removeAccolade: function(accolade) {
                this.accoladeList.removeObject( accolade );
            },
            prepareData: function(deferred) { Actions.prepareData( this, deferred ); },
            saveAccolades: function() { alert("Accolades saved!"); },
            finishStage: function() { Actions.finishStage( this ); }
        }
    });

    return AccoladesController;
});
