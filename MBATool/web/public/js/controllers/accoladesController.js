define(['actions/accoladesActions', 'components/accoladeSelectorComponent', 'ember'], function(Actions, AccoladeSelectorComponent, Ember) {

    var AccoladesController = Ember.ObjectController.extend({
        needs:              "progress",
        canSave:            false,
        stageComplete:      false,
        availableAccolades: [],
        playerAccolades:    [],
        accoladeList:       [],
        errorMessages:      [],
        AccoladeSelectorComponent:   AccoladeSelectorComponent,
        actions: {
            addAccolade:      function(                ) { Actions.addAccolade(      this                ); },
            removeAccolade:   function(accolade        ) { Actions.removeAccolade(   this,     accolade  ); },
            prepareData:      function(deferred        ) { Actions.prepareData(      this,     deferred  ); },
            saveAccolades:    function(                ) { Actions.saveAccolades(    this                ); },
            updateSaveStatus: function(                ) { Actions.updateSaveStatus( this                ); },
            resolveAccolades: function(                ) { Actions.resolveAccolades( this                ); },
            displayAccolades: function(accolades       ) { Actions.displayAccolades( this,     accolades ); },
            searchPlayers:    function(accolade, value ) { Actions.searchPlayers(    accolade, value     ); },
            selectPlayer:     function(accolade, player) { Actions.selectPlayer(     accolade, player    ); },
            finishStage:      function(                ) { Actions.finishStage(      this                ); }
        }
    });

    return AccoladesController;
});
