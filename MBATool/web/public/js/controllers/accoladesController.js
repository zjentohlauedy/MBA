define(['actions/accoladesActions', 'ember'], function(Actions, Ember) {

    var AccoladesController = Ember.ObjectController.extend({
        needs:              "progress",
        pageReady:          false,
        stageComplete:      true,
        availableAccolades: [ { name: "MBA Batter of the Year",            id:  1, type: "batting",  value: 1 },
                              { name: "World League Batter of the Year",   id:  2, type: "batting",  value: 3 },
                              { name: "Global League Batter of the Year",  id:  3, type: "batting",  value: 2 },
                              { name: "Atlantic Batter of the Year",       id:  4, type: "batting",  value: 4 },
                              { name: "North Batter of the Year",          id:  5, type: "batting",  value: 5 },
                              { name: "South Batter of the Year",          id:  6, type: "batting",  value: 7 },
                              { name: "Pacific Batter of the Year",        id:  7, type: "batting",  value: 6 },
                              { name: "MBA Pitcher of the Year",           id:  8, type: "pitching", value: 1 },
                              { name: "World League Pitcher of the Year",  id:  9, type: "pitching", value: 3 },
                              { name: "Global League Pitcher of the Year", id: 10, type: "pitching", value: 2 },
                              { name: "Atlantic Pitcher of the Year",      id: 11, type: "pitching", value: 4 },
                              { name: "North Pitcher of the Year",         id: 12, type: "pitching", value: 5 },
                              { name: "South Pitcher of the Year",         id: 13, type: "pitching", value: 7 },
                              { name: "Pacific Pitcher of the Year",       id: 14, type: "pitching", value: 6 } ],
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
