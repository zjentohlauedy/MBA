define(['objects/constants', 'objects/globals', 'utils', 'ember'], function(Constants, Globals, Utils, Ember) {

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
            toggleRookieTable: function() {
                if (this.showRookiePitchers) {
                    this.set("showRookiePitchers", false);
                    this.set("showRookieBatters",  true );

                    this.set("toggleRookieButtonLabel", "Show Pitchers");

                    this.rookies.pitchers.setEach("isSelected", false);
                }
                else {
                    this.set("showRookiePitchers", true );
                    this.set("showRookieBatters",  false);

                    this.set("toggleRookieButtonLabel", "Show Batters");

                    this.rookies.batters.setEach("isSelected", false);
                }

                this.send("setDraftStatus");
            },
            sortPitchers: function(field) {
                if (field === this.currentPitcherSortField) {
                    this.rookies.set("pitchers", this.rookies.pitchers.reverseObjects());
                    return;
                }

                this.set("currentPitcherSortField", field);

                this.rookies.set("pitchers", this.rookies.pitchers.sortBy(field, "rating"));
            },
            sortBatters: function(field) {
                if (field === this.currentBatterSortField) {
                    this.rookies.set("batters", this.rookies.batters.reverseObjects());
                    return;
                }

                this.set("currentBatterSortField", field);

                this.rookies.set("batters", this.rookies.batters.sortBy(field, "rating"));
            },
            selectRookiePitcher: function(pitcher) {
                if (pitcher.isSelected) {
                    pitcher.set("isSelected", false);
                    return;
                }

                this.rookies.pitchers.setEach("isSelected", false);

                pitcher.set("isSelected", true);
            },
            selectRookieBatter: function(batter) {
                if (batter.isSelected) {
                    batter.set("isSelected", false);
                    return;
                }

                this.rookies.batters.setEach("isSelected", false);

                batter.set("isSelected", true);
            },
            draftRookie: function() {
                if (!this.canDraft) return;

                var rookie;
                if      (rookie = this.rookies.pitchers.findBy("isSelected")) { this.send("draftPitcher", rookie); }
                else if (rookie = this.rookies.batters. findBy("isSelected")) { this.send("draftBatter",  rookie); }
            },
            draftPitcher: function(pitcher) {
                var controller = this;
                var url = Utils.findLink( this.team.links, "players" ) + "/" + pitcher.player_id + "/season/" + Globals.season;

                $.ajax( url, {
                    type: 'POST',
                    success: function() {
                        controller.rookies.pitchers.removeObject(pitcher);
                        controller.team.   pitchers.   addObject(pitcher);
                        controller.team.set("draftedRookiePitcher", true);
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
                        controller.rookies.batters.removeObject(batter);
                        controller.team   .batters.   addObject(batter);
                        controller.team.set("draftedRookieBatter", true);
                        controller.send("showNextTeam");
                    },
                    error: function() {
                        alert("Error drafting pitcher!");
                    }
                });
            },
            showFirstTeam: function() {
                this.set("currentTeamIdx", 0);

                this.send("loadTeam", this.rookies.draftOrder[this.currentTeamIdx] );
            },
            showNextTeam: function() {
                var nextTeamIdx = this.currentTeamIdx + 1;

                if (nextTeamIdx == this.rookies.draftOrder.length) {
                    this.set("canDraft",      false);
                    this.set("stageComplete", true );
                    return;
                }

                this.set("currentTeamIdx", nextTeamIdx);

                this.send("loadTeam", this.rookies.draftOrder[this.currentTeamIdx] );
            },
            loadTeam: function(teamId) {
                var controller = this;

                $.ajax( "/mba/resources/teams/" + teamId, {
                    success: function(team) {
                        team.pitchers             = [];
                        team.batters              = [];
                        team.draftedRookiePitcher = false;
                        team.draftedRookieBatter  = false;

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

                if (this.showRookiePitchers && (this.team.pitchers.length >= Constants.ROOKIE_DRAFT_MAX_PITCHERS)) {
                    this.set("canDraft", false);
                    return;
                }

                if (this.showRookieBatters && (this.team.batters.length >= Constants.ROOKIE_DRAFT_MAX_BATTERS)) {
                    this.set("canDraft", false);
                    return;
                }

                this.set("canDraft", true);
            },
            loadFreeAgents: function(freeAgents) {
                var promises = [];
                var controller = this;

                for (var i = 0; i < freeAgents.length; i++) {
                    promises.push( Utils.loadPlayer(freeAgents[i], controller.freeAgents, Globals.season, false) );
                }

                $.when.apply(null, promises).done(function() {
                    controller.get("controllers.free-agents").set('freeAgents', controller.freeAgents);
                    controller.get("controllers.free-agents").send('showFirstTeam');
                    controller.get("controllers.progress").send('nextStage');
                });
            },
            finishStage: function() {
                var controller = this;

                if (this.stageComplete) {
                    $.ajax( "/mba/resources/drafts/free-agent/season/" + (Globals.season - 1), {
                        success: function(draft) {
                            controller.freeAgents.set( "draftOrder", draft );

                            $.ajax( "/mba/resources/players?freeagent=true&season=" + Globals.season, {
                                success: function(freeAgents) {
                                    controller.send( "loadFreeAgents", freeAgents );
                                },
                                error: function() {
                                    alert("Error retrieving free agents!");
                                }
                            });
                        },
                        error: function() {
                            alert("Error retrieving draft!");
                        }
                    });
                }
            }
        }
    });

    return RookieDraftController;
});
