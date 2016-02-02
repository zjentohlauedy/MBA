define(['actions/rosterCutActions', 'ember'], function(Actions, Ember) {

    var RosterCutController = Ember.ObjectController.extend({
        needs:          ["progress", "rookie-draft"],
        teams:          [],
        currentTeam:    Ember.Object.create(),
        currentPitcherSortField: "",
        currentBatterSortField:  "",
        pitchers: {
            isComplete: false,
            isError:    false
        },
        batters: {
            isComplete: false,
            isError:    false
        },
        stageComplete:  false,
        actions: {
            prepareData:          function(deferred)      { Actions.prepareData(this, deferred);      },
            selectTeam:           function(team)          { Actions.selectTeam(this, team);           },
            loadPlayers:          function(team, players) { Actions.loadPlayers(this, team, players); },
            toggleCutPitcher:     function(pitcher)       { Actions.toggleCutPitcher(this, pitcher);  },
            toggleCutBatter:      function(batter)        { Actions.toggleCutBatter(this, batter);    },
            updatePitchersStatus: function()              { Actions.updatePitchersStatus(this);       },
            updateBattersStatus:  function()              { Actions.updateBattersStatus(this);        },
            updateTeamStatus:     function()              { Actions.updateTeamStatus(this);           },
            updateStatus:         function()              { Actions.updateStatus(this);               },
            sortPitchers:         function(field)         { Actions.sortPitchers(this, field);        },
            sortBatters:          function(field)         { Actions.sortBatters(this, field);         },
            finishStage:          function()              { Actions.finishStage(this);                }
        }
    });

    return RosterCutController;
});
