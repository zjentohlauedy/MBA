define(['constants', 'globals', 'utils', 'ember'], function(Constants, Globals, Utils, Ember) {

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
            toggleFreeAgentTable: function() {
                if (this.showFreeAgentPitchers) {
                    this.set("showFreeAgentPitchers", false);
                    this.set("showFreeAgentBatters",  true );

                    this.set("toggleFreeAgentButtonLabel", "Show Pitchers");

                    this.freeAgents.pitchers.setEach("isSelected", false);
                }
                else {
                    this.set("showFreeAgentPitchers", true );
                    this.set("showFreeAgentBatters",  false);

                    this.set("toggleFreeAgentButtonLabel", "Show Batters");

                    this.freeAgents.batters.setEach("isSelected", false);
                }

                this.send("setDraftStatus");
            },
            sortPitchers: function(field) {
                if (field === this.currentPitcherSortField) {
                    this.freeAgents.set("pitchers", this.freeAgents.pitchers.reverseObjects());
                    return;
                }

                this.set("currentPitcherSortField", field);

                this.freeAgents.set("pitchers", this.freeAgents.pitchers.sortBy(field));
            },
            sortBatters: function(field) {
                if (field === this.currentBatterSortField) {
                    this.freeAgents.set("batters", this.freeAgents.batters.reverseObjects());
                    return;
                }

                this.set("currentBatterSortField", field);

                this.freeAgents.set("batters", this.freeAgents.batters.sortBy(field));
            },
            selectFreeAgentPitcher: function(pitcher) {
                if (pitcher.isSelected) {
                    pitcher.set("isSelected", false);
                    return;
                }

                this.freeAgents.pitchers.setEach("isSelected", false);

                pitcher.set("isSelected", true);
            },
            selectFreeAgentBatter: function(batter) {
                if (batter.isSelected) {
                    batter.set("isSelected", false);
                    return;
                }

                this.freeAgents.batters.setEach("isSelected", false);

                batter.set("isSelected", true);
            },
            draftFreeAgent: function() {
                if (!this.canDraft) return;

                var freeAgent;
                if      (freeAgent = this.freeAgents.pitchers.findBy("isSelected")) { this.send("draftPitcher", freeAgent); }
                else if (freeAgent = this.freeAgents.batters. findBy("isSelected")) { this.send("draftBatter",  freeAgent); }
            },
            draftPitcher: function(pitcher) {
                var controller = this;
                var url = Utils.findLink( this.team.links, "players" ) + "/" + pitcher.player_id + "/season/" + Globals.season;

                $.ajax( url, {
                    type: 'POST',
                    success: function() {
                        controller.freeAgents.pitchers.removeObject(pitcher);
                        controller.team.      pitchers.   addObject(pitcher);
                        controller.send("showNextTeam");
                    },
                    error: function() {
                        alert("Error drafting pitcher!");
                    }
                });
            },
            draftBatter: function(batter) {
                var controller = this;
                var url = Utils.findLink( this.team.links, "players" ) + "/" + batter.player_id + "/season/" + Globals.season;

                $.ajax( url, {
                    type: 'POST',
                    success: function() {
                        controller.freeAgents.batters.removeObject(batter);
                        controller.team      .batters.   addObject(batter);
                        controller.send("showNextTeam");
                    },
                    error: function() {
                        alert("Error drafting pitcher!");
                    }
                });
            },
            showFirstTeam: function() {
                this.set("currentTeamIdx", 0);

                this.send("loadTeam", this.freeAgents.draftOrder[this.currentTeamIdx] );
            },
            showNextTeam: function() {
                var nextTeamIdx = this.currentTeamIdx + 1;

                if (nextTeamIdx == this.freeAgents.draftOrder.length) {
                    this.set("canDraft",      false);
                    this.set("stageComplete", true );
                    return;
                }

                this.set("currentTeamIdx", nextTeamIdx);

                this.send("loadTeam", this.freeAgents.draftOrder[this.currentTeamIdx] );
            },
            loadTeam: function(teamId) {
                var controller = this;

                $.ajax( "/mba/resources/teams/" + teamId, {
                    success: function(team) {
                        team.pitchers = [];
                        team.batters  = [];

                        $.ajax( Utils.findLink( team.links, "players" ) + "?season=" + Globals.season, {
                            success: function(players) {
                                controller.send('loadPlayers', team, players );
                            },
                            error: function() {
                                alert("Error retrieving players!");
                            }
                        });
                    },
                    error: function() {
                        alert("Error retrieving team!");
                    }
                });
            },
            loadPlayers: function(team, players) {
                var promises = [];
                var controller = this;

                for (var i = 0; i < players.length; i++) {
                    promises.push( Utils.loadPlayer(players[i], team, Globals.season, false) );
                }

                $.when.apply(null, promises).done(function() {
                    controller.set("team", Ember.Object.create().setProperties(team));
                    controller.send("setDraftStatus");
                });
            },
            setDraftStatus: function() {
                if (this.stageComplete) return;

                if (this.showFreeAgentPitchers && (this.team.pitchers.length >= Constants.TEAM_MAX_PITCHERS)) {
                    this.set("canDraft", false);
                    return;
                }

                if (this.showFreeAgentBatters && (this.team.batters.length >= Constants.TEAM_MAX_BATTERS)) {
                    this.set("canDraft", false);
                    return;
                }

                this.set("canDraft", true);
            },
            finishStage: function() {
                if (this.stageComplete) {
                    this.get("controllers.progress").send('nextStage');
                }
            }
        }
    });

    return FreeAgentsController;
});
