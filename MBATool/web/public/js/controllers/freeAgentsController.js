define(['actions/freeAgentsActions', 'ember'], function(Actions, Ember) {

    var FreeAgentsController = Ember.ObjectController.extend({
        needs:                      "progress",
        team:                       {},
        freeAgents:                 null,
        currentTeamIdx:             0,
        currentPitcherSortField:    "",
        currentBatterSortField:     "",
        toggleFreeAgentButtonLabel: "Show Batters",
        showFreeAgentPitchers:      true,
        showFreeAgentBatters:       false,
        canDraft:                   true,
        stageComplete:              false,
        actions: {
            toggleFreeAgentTable:   function()              { Actions.toggleFreeAgentTable(this);            },
            sortPitchers:           function(field)         { Actions.sortPitchers(this, field);             },
            sortBatters:            function(field)         { Actions.sortBatters(this, field);              },
            selectFreeAgentPitcher: function(pitcher)       { Actions.selectFreeAgentPitcher(this, pitcher); },
            selectFreeAgentBatter:  function(batter)        { Actions.selectFreeAgentBatter(this, batter);   },
            draftFreeAgent:         function()              { Actions.draftFreeAgent(this);                  },
            draftPitcher:           function(pitcher)       { Actions.draftPitcher(this, pitcher);           },
            draftBatter:            function(batter)        { Actions.draftBatter(this, batter);             },
            showFirstTeam:          function()              { Actions.showFirstTeam(this);                   },
            showNextTeam:           function()              { Actions.showNextTeam(this);                    },
            loadTeam:               function(teamId)        { Actions.loadTeam(this, teamId);                },
            loadPlayers:            function(team, players) { Actions.loadPlayers(this, team, players);      },
            setDraftStatus:         function()              { Actions.setDraftStatus(this);                  },
            finishStage:            function()              { Actions.finishStage(this);                     }
        }
    });

    return FreeAgentsController;
});
