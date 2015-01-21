define(['actions/rookieDraftActions', 'ember'], function(Actions, Ember) {

    var RookieDraftController = Ember.ObjectController.extend({
        needs:                   ["progress", "free-agents"],
        team:                    {},
        rookies:                 null,
        freeAgents:              Ember.Object.create().setProperties({
            draftOrder: [],
            pitchers:   [],
            batters:    []
        }),
        currentTeamIdx:          0,
        currentPitcherSortField: "",
        currentBatterSortField:  "",
        toggleRookieButtonLabel: "Show Batters",
        showRookiePitchers:      true,
        showRookieBatters:       false,
        canDraft:                true,
        stageComplete:           false,
        actions: {
            toggleRookieTable:   function()              { Actions.toggleRookieTable(this);            },
            sortPitchers:        function(field)         { Actions.sortPitchers(this, field);          },
            sortBatters:         function(field)         { Actions.sortBatters(this, field);           },
            selectRookiePitcher: function(pitcher)       { Actions.selectRookiePitcher(this, pitcher); },
            selectRookieBatter:  function(batter)        { Actions.selectRookieBatter(this, batter);   },
            draftRookie:         function()              { Actions.draftRookie(this);                  },
            draftPitcher:        function(pitcher)       { Actions.draftPitcher(this, pitcher);        },
            draftBatter:         function(batter)        { Actions.draftBatter(this, batter);          },
            showFirstTeam:       function()              { Actions.showFirstTeam(this);                },
            showNextTeam:        function()              { Actions.showNextTeam(this);                 },
            loadTeam:            function(teamId)        { Actions.loadTeam(this, teamId);             },
            loadPlayers:         function(team, players) { Actions.loadPlayers(this, team, players);   },
            setDraftStatus:      function()              { Actions.setDraftStatus(this);               },
            loadFreeAgents:      function(freeAgents)    { Actions.loadFreeAgents(this, freeAgents);   },
            finishStage:         function()              { Actions.finishStage(this);                  }
        }
    });

    return RookieDraftController;
});
