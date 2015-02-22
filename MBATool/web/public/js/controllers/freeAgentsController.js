define(['objects/constants', 'objects/globals', 'actions/commonDraftActions', 'actions/freeAgentsActions', 'ember'],
       function(Constants, Globals, CommonActions, AgentActions, Ember) {

    var FreeAgentsController = Ember.ObjectController.extend({
        needs:                   "progress",
        team:                    {},
        draftOrder:              null,
        draftRound:              1,
        pickNumber:              1,
        availablePlayers:        Ember.Object.create().setProperties({
            pitchers:   [],
            batters:    []
        }),
        maxPitchers:             Constants.TEAM_MAX_PITCHERS,
        maxBatters:              Constants.TEAM_MAX_BATTERS,
        currentTeamIdx:          0,
        currentPitcherSortField: "",
        currentBatterSortField:  "",
        toggleButtonLabel:       "Show Batters",
        showAvailablePitchers:   true,
        showAvailableBatters:    false,
        canDraft:                true,
        stageComplete:           false,
        draftResource:           function() { return Constants.FREE_AGENT_DRAFT_URI + '/' + (Globals.season - 1);   },
        playersResource:         function() { return Constants.PLAYERS_URI + "?freeagent=true&season=" + Globals.season; },
        actions: {
            prepareData:                 function(deferred)        { CommonActions.prepareData(this, deferred);                  },
            toggleAvailablePlayersTable: function()                { CommonActions.toggleAvailablePlayersTable(this);            },
            sortPitchers:                function(field)           { CommonActions.sortPitchers(this, field);                    },
            sortBatters:                 function(field)           { CommonActions.sortBatters(this, field);                     },
            selectAvailablePlayer:       function(players, player) { CommonActions.selectAvailablePlayer(this, players, player); },
            draftSelectedPlayer:         function()                { CommonActions.draftSelectedPlayer(this);                    },
            showNextTeam:                function()                { CommonActions.showNextTeam(this);                           },
            setDraftStatus:              function()                { CommonActions.setDraftStatus(this);                         },
            finishStage:                 function()                { AgentActions.finishStage(this);                             }
        }
    });

    return FreeAgentsController;
});
