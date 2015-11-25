define(['actions/accoladesActions', 'ember'], function(Actions, Ember) {

    var AccoladesController = Ember.ObjectController.extend({
        needs:              "progress",
        pageReady:          false,
        stageComplete:      true,
        availableAccolades: [ { name: "MBA Batter of the Year",            value:  1 },
                              { name: "World League Batter of the Year",   value:  2 },
                              { name: "Global League Batter of the Year",  value:  3 },
                              { name: "Atlantic Batter of the Year",       value:  4 },
                              { name: "North Batter of the Year",          value:  5 },
                              { name: "South Batter of the Year",          value:  6 },
                              { name: "Pacific Batter of the Year",        value:  7 },
                              { name: "MBA Pitcher of the Year",           value:  8 },
                              { name: "World League Pitcher of the Year",  value:  9 },
                              { name: "Global League Pitcher of the Year", value: 10 },
                              { name: "Atlantic Pitcher of the Year",      value: 11 },
                              { name: "North Pitcher of the Year",         value: 12 },
                              { name: "South Pitcher of the Year",         value: 13 },
                              { name: "Pacific Pitcher of the Year",       value: 14 } ],
        accoladeList:       [ { selectedAccolade: null, selectedPlayer: null } ],
        actions: {
            addAccolade: function() {
                var accolade = { selectedAccolade: null, selectedPlayer: null };

                this.accoladeList.addObject( accolade );
            },
            removeAccolade: function(accolade) {
                this.accoladeList.removeObject( accolade );
            },
            saveAccolades: function() { alert("Accolades saved!"); },
            finishStage: function() { Actions.finishStage( this ); }
        }
    });

    return AccoladesController;
});
