define(['actions/rosterCutActions', 'ember'], function(Actions, Ember) {

    var RosterCutController = Ember.ObjectController.extend({
        needs:          ["progress", "rookie-draft"],
        teams:          [],
        currentTeam:    Ember.Object.create(),
        rookies:        Ember.Object.create().setProperties({
            draftOrder: [],
            pitchers:   [],
            batters:    []
        }),
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
            selectTeam:           function(team)          { Actions.selectTeam(this, team);           },
            loadPlayers:          function(team, players) { Actions.loadPlayers(this, team, players); },
            toggleCutPitcher:     function(pitcher)       { Actions.toggleCutPitcher(this, pitcher);  },
            toggleCutBatter:      function(batter)        { Actions.toggleCutBatter(this, batter);    },
            updatePitchersStatus: function()              { Actions.updatePitchersStatus(this);       },
            updateBattersStatus:  function()              { Actions.updateBattersStatus(this);        },
            updateTeamStatus:     function()              { Actions.updateTeamStatus(this);           },
            updateStatus:         function()              { Actions.updateStatus(this);               },
            loadRookies:          function(rookies)       { Actions.loadRookies(this, rookies);       },
            finishStage:          function()              { Actions.finishStage(this);                }
        }
    });

    return RosterCutController;
});
