define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var FreeAgentsActions = {
        prepareData: function(controller, deferred) {
            $.ajax( "/mba/resources/drafts/free-agent/season/" + (Globals.season - 1), {
                success: function(draft) {
                    controller.freeAgents.set( "draftOrder", draft );

                    $.ajax( "/mba/resources/players?freeagent=true&season=" + Globals.season, {
                        success: function(freeAgents) {
                            var promises = [];

                            for (var i = 0; i < freeAgents.length; i++) {
                                promises.push( Utils.loadPlayer(freeAgents[i], controller.freeAgents, Globals.season, false) );
                            }

                            $.when.apply(null, promises).done(function() {
                                controller.send('showFirstTeam');

                                deferred.resolve();
                            });
                        },
                        error: function() {
                            alert("Error retrieving free agents!");

                            deferred.reject();
                        }
                    });
                },
                error: function() {
                    alert("Error retrieving free agent draft!");

                    deferred.reject();
                }
            });
        },
        toggleFreeAgentTable: function(controller) {
            if (controller.showFreeAgentPitchers) {
                controller.set("showFreeAgentPitchers", false);
                controller.set("showFreeAgentBatters",  true );

                controller.set("toggleFreeAgentButtonLabel", "Show Pitchers");

                controller.freeAgents.pitchers.setEach("isSelected", false);
            }
            else {
                controller.set("showFreeAgentPitchers", true );
                controller.set("showFreeAgentBatters",  false);

                controller.set("toggleFreeAgentButtonLabel", "Show Batters");

                controller.freeAgents.batters.setEach("isSelected", false);
            }

            controller.send("setDraftStatus");
        },
        sortPitchers: function(controller, field) {
            if (field === controller.currentPitcherSortField) {
                controller.freeAgents.set("pitchers", controller.freeAgents.pitchers.reverseObjects());
                return;
            }

            controller.set("currentPitcherSortField", field);

            controller.freeAgents.set("pitchers", controller.freeAgents.pitchers.sortBy(field));
        },
        sortBatters: function(controller, field) {
            if (field === controller.currentBatterSortField) {
                controller.freeAgents.set("batters", controller.freeAgents.batters.reverseObjects());
                return;
            }

            controller.set("currentBatterSortField", field);

            controller.freeAgents.set("batters", controller.freeAgents.batters.sortBy(field));
        },
        selectFreeAgentPitcher: function(controller, pitcher) {
            if (pitcher.isSelected) {
                pitcher.set("isSelected", false);
                return;
            }

            controller.freeAgents.pitchers.setEach("isSelected", false);

            pitcher.set("isSelected", true);
        },
        selectFreeAgentBatter: function(controller, batter) {
            if (batter.isSelected) {
                batter.set("isSelected", false);
                return;
            }

            controller.freeAgents.batters.setEach("isSelected", false);

            batter.set("isSelected", true);
        },
        draftFreeAgent: function(controller) {
            if (!controller.canDraft) return;

            var freeAgent;
            if      (freeAgent = controller.freeAgents.pitchers.findBy("isSelected")) { controller.send("draftPitcher", freeAgent); }
            else if (freeAgent = controller.freeAgents.batters. findBy("isSelected")) { controller.send("draftBatter",  freeAgent); }
        },
        draftPitcher: function(controller, pitcher) {
            var url = Utils.findLink( controller.team.links, "players" ) + "/" + pitcher.player_id + "/season/" + Globals.season;

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
        draftBatter: function(controller, batter) {
            var url = Utils.findLink( controller.team.links, "players" ) + "/" + batter.player_id + "/season/" + Globals.season;

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
        showFirstTeam: function(controller) {
            controller.set("currentTeamIdx", 0);

            controller.send("loadTeam", controller.freeAgents.draftOrder[controller.currentTeamIdx] );
        },
        showNextTeam: function(controller) {
            var nextTeamIdx = controller.currentTeamIdx + 1;

            if (nextTeamIdx == controller.freeAgents.draftOrder.length) {
                controller.set("canDraft",      false);
                controller.set("stageComplete", true );
                return;
            }

            controller.set("currentTeamIdx", nextTeamIdx);

            controller.send("loadTeam", controller.freeAgents.draftOrder[controller.currentTeamIdx] );
        },
        loadTeam: function(controller, teamId) {
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
        loadPlayers: function(controller, team, players) {
            var promises = [];

            for (var i = 0; i < players.length; i++) {
                promises.push( Utils.loadPlayer(players[i], team, Globals.season, false) );
            }

            $.when.apply(null, promises).done(function() {
                controller.set("team", Ember.Object.create().setProperties(team));
                controller.send("setDraftStatus");
            });
        },
        setDraftStatus: function(controller) {
            if (controller.stageComplete) return;

            if (controller.showFreeAgentPitchers && (controller.team.pitchers.length >= Constants.TEAM_MAX_PITCHERS)) {
                controller.set("canDraft", false);
                return;
            }

            if (controller.showFreeAgentBatters && (controller.team.batters.length >= Constants.TEAM_MAX_BATTERS)) {
                controller.set("canDraft", false);
                return;
            }

            controller.set("canDraft", true);
        },
        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return FreeAgentsActions;
});
