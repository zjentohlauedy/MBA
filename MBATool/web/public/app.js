window.App = Ember.Application.create();

var App = window.App

App._constants = {
/* TEMP
    TEAM_MAX_PITCHERS:          10,
    TEAM_MAX_BATTERS:           15,
    ROSTER_CUT_TARGET_PITCHERS:  3,
    ROSTER_CUT_TARGET_BATTERS:   5
*/
    TEAM_MAX_PITCHERS:          15,
    TEAM_MAX_BATTERS:           20,
    ROSTER_CUT_TARGET_PITCHERS: 10,
    ROSTER_CUT_TARGET_BATTERS:  15
};

App._utils = {
    decorateTeams: function(teams) {

        for ( var i = 0; i < teams.length; i++ ) {
            teams[i].pitchers             = [];
            teams[i].batters              = [];
            teams[i].isSelected           = false;
            teams[i].isComplete           = false;
            teams[i].isError              = false;
            teams[i].draftedRookiePitcher = false;
            teams[i].draftedRookieBatter  = false;
        }

        return teams.map(function(entry){
            return Ember.Object.create().setProperties(entry);
        });
    },
    decoratePitcher: function(player, stats) {

        player.isCut      = false;
        player.isSelected = false;
        player.rating     = player.speed + player.control + player.fatigue;

        if (stats) {
            innings = stats.innings + (stats.outs / 10);
            at_bats = (stats.innings * 3) + stats.outs + stats.walks + stats.hits;
            era     = (innings == 0) ? 0 : stats.earned_runs / (innings / 9);
            vsba    = (at_bats == 0) ? 0 : stats.hits / at_bats;
            ipg     = (stats.games == 0) ? 0 : innings / stats.games;
            whip    = (innings == 0) ? 0 : (stats.walks + stats.hits) / innings;
            sop9    = (innings == 0) ? 0 : stats.strike_outs / (innings / 9);
            eff     = (stats.innings - stats.hits) + (stats.strike_outs - stats.hits);

            player.record  = stats.wins + " - " + stats.losses;
            player.innings = innings.toFixed(1);
            player.era     = era.toFixed(2);
            player.vsba    = vsba.toFixed(3).replace(/^0\./,".");
            player.ipg     = ipg.toFixed(2);
            player.whip    = whip.toFixed(3);
            player.sop9    = sop9.toFixed(2);
            player.eff     = (eff > 0) ? "+" + eff : eff;
        }

        return Ember.Object.create().setProperties(player);
    },
    decorateBatter: function(player, stats) {

        player.isCut      = false;
        player.isSelected = false;
        player.rating     = player.power + player.hit_n_run + player.running;

        if (stats) {
            avg      = (stats.at_bats == 0) ? 0 : stats.hits / stats.at_bats;
            slugging = (stats.at_bats == 0) ? 0 : (stats.hits + (2 * stats.doubles) + (3 * stats.triples) + (4 *stats.home_runs)) / stats.at_bats;
            oba      = (stats.at_bats == 0) ? 0 : (stats.hits + stats.walks) / stats.at_bats;
            rpg      = (stats.games   == 0) ? 0 : (stats.runs + stats.runs_batted_in - stats.home_runs) / stats.games;

            player.games    = stats.games;
            player.avg      = avg.toFixed(3).replace(/^0\./,".");
            player.homers   = stats.home_runs;
            player.steals   = stats.steals;
            player.slugging = slugging.toFixed(3).replace(/^0\./,".");
            player.oba      = oba.toFixed(3).replace(/^0\./,".");
            player.rpg      = rpg.toFixed(2);
        }

        return Ember.Object.create().setProperties(player);
    },
    findLink: function(links, rel) {

        for (var i = 0; i < links.length; i++) {
            if (links[i].rel === rel) {
                return links[i].href;
            }
        }

        return null;
    }
}

App._progress = {
    stage: 0,
    stages: [
        { number: "1", name: "Start New Season", status: "progress-curr", route: "start-season"  },
        { number: "2", name: "Roster Cut",       status: "progress-todo", route: "roster-cut"    },
        { number: "3", name: "Rookie Draft",     status: "progress-todo", route: "rookie-draft"  },
        { number: "4", name: "Free Agent Draft", status: "progress-todo", route: "free-agents"   },
        { number: "5", name: "Export Season",    status: "progress-todo", route: "export-season" },
        { number: "6", name: "Import Season",    status: "progress-todo", route: "import-season" }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    })
};

App._teams = [];

App._rookies = Ember.Object.create().setProperties({
    draftOrder: [],
    pitchers:   [],
    batters:    []
});

App._freeAgents = Ember.Object.create().setProperties({
    draftOrder: [],
    pitchers:   [],
    batters:    []
});

App.Router.map(function() {
    this.route("start-season",    { path: "/start"  });
    this.route("roster-cut",      { path: "/cut"    });
    this.route("rookie-draft",    { path: "/rookie" });
    this.route("free-agents",     { path: "/agents" });
    this.route("export-season",   { path: "/export" });
    this.route("import-season",   { path: "/import" });
    this.route("season-complete", { path: "/done"   });
});

App.IndexRoute = Ember.Route.extend({
  redirect: function() {
    this.transitionTo(App._progress.stages[App._progress.stage].route);
  }
});

App.ProgressController = Ember.ObjectController.extend({
    stage:  App._progress.stage,
    stages: App._progress.stages,
    actions: {
        nextStage: function() {
            if (this.stage >= this.stages.length) {
                this.stages.setEach("status", "progress-todo");
                this.stage = 0;

                this.transitionToRoute(this.stages[this.stage].route);

                this.stages[this.stage].set("status", "progress-curr");

                return;
            }

            this.stages[this.stage].set("status", "progress-done");
            this.stage++;

            if (this.stage < this.stages.length) {
                this.transitionToRoute(this.stages[this.stage].route);

                this.stages[this.stage].set("status", "progress-curr");
            }
            else {
                this.transitionToRoute("season-complete");
            }
        }
    }
});

App.StartSeasonController = Ember.ObjectController.extend({
    needs:   'progress',
    actions: {
        startNewSeason: function() {
            var controller = this;
            $.ajax( "/mba/actions/start_season", {
                type: 'POST',
                success: function(data) {
                    App._season = data.Season;
                    controller.send('loadTeams');
                },
                error: function() {
                    alert("Error starting new season!");
                }
            });
        },
        loadTeams: function() {
            var controller = this;
            $.ajax( "/mba/resources/teams", {
                success: function(teams) {
                    App._teams.setObjects(App._utils.decorateTeams(teams));
                    controller.send('finishStage');
                },
                error: function() {
                    alert("Error loading teams!")
                }
            });
        },
        finishStage: function() {
            this.get("controllers.progress").send('nextStage');
        }
    }
});


var loadPlayer = function(player, team, needStats) {
    var deferred = $.Deferred();

    $.ajax( App._utils.findLink( player.links, "self" ), {
        success: function(playerDetails) {

            if (needStats) {
                $.ajax( App._utils.findLink( playerDetails.links, "stats" ) + "?season=" + (App._season - 1) + "&phase=1", {
                    success: function(playerStats) {

                        if (playerDetails.player_type === "Pitcher") {
                            team.pitchers.addObject(App._utils.decoratePitcher(playerDetails, playerStats[0]));
                        }

                        if (playerDetails.player_type === "Batter") {
                            team.batters. addObject(App._utils.decorateBatter( playerDetails, playerStats[0]));
                        }

                        deferred.resolve();
                    },
                    error: function() {
                        alert("Error loading player stats!");
                        deferred.reject();
                    }
                });

                return;
            }

            if (playerDetails.player_type === "Pitcher") {
                team.pitchers.addObject(App._utils.decoratePitcher(playerDetails, null));
            }

            if (playerDetails.player_type === "Batter") {
                team.batters. addObject(App._utils.decorateBatter( playerDetails, null));
            }

            deferred.resolve();
        },
        error: function() {
            alert("Error loading player!");
            deferred.reject();
        }
    });

    return deferred.promise();
}


App.RosterCutController = Ember.ObjectController.extend({
    needs:          ["progress", "rookie-draft"],
    teams:          App._teams,
    currentTeam:    Ember.Object.create(),
    pitchers: {
        isComplete: false,
        isError:    false
    },
    batters: {
        isComplete: false,
        isError:    false
    },
// TEMP
//    stageComplete:  false,
    stageComplete:  true,
    actions: {
        selectTeam: function(team) {
            if (team.isSelected) return;

            team.set("isSelected", true);
            this.currentTeam.set("isSelected", false);
            this.set( "currentTeam", team );


            if ( team.pitchers.length == 0 ) {
                var controller = this;

                $.ajax( "/mba/resources/teams/" + team.team_id + "/players?season=" + App._season + "&phase=1", {
                    success: function(players) {
                        controller.send('loadPlayers', team, players );
                    },
                    error: function() {
                        alert("Error loading players!")
                    }
                });
            }
            else
            {
                this.send("updatePitchersStatus");
                this.send("updateBattersStatus");
            }
        },
        loadPlayers: function(team, players) {
            var promises = [];
            var controller = this;

            for (var i = 0; i < players.length; i++) {
                promises.push( loadPlayer(players[i], team, true) );
            }

            $.when.apply(null, promises).done(function() {
                controller.send("updatePitchersStatus");
                controller.send("updateBattersStatus");
                controller.send("updateTeamStatus");
            });
        },
        toggleCutPitcher: function(pitcher) {
            var controller = this;
            var url = App._utils.findLink( this.currentTeam.links, "players" ) + "/" + pitcher.player_id + "/season/" + App._season;

            $.ajax( url, {
                type: pitcher.isCut ? 'POST' : 'DELETE',
                success: function() {
                    pitcher.set("isCut", !pitcher.isCut);

                    controller.send("updatePitchersStatus");
                    controller.send("updateTeamStatus");
                },
                error: function() {
                    alert("Error modifying pitcher!");
                }
            });
        },
        toggleCutBatter: function(batter) {
            var controller = this;
            var url = App._utils.findLink( this.currentTeam.links, "players" ) + "/" + batter.player_id + "/season/" + App._season;

            $.ajax( url, {
                type: batter.isCut ? 'POST' : 'DELETE',
                success: function() {
                    batter.set("isCut", !batter.isCut);

                    controller.send("updateBattersStatus");
                    controller.send("updateTeamStatus");
                },
                error: function() {
                    alert("Error modifying batter!");
                }
            });
        },
        updatePitchersStatus: function() {
            var count = this.currentTeam.pitchers.reduce(function(value, pitcher){ return (pitcher.isCut) ? value : value + 1; }, 0);

            if      (count > App._constants.ROSTER_CUT_TARGET_PITCHERS) { this.set("pitchers.isComplete", false); this.set("pitchers.isError", false); }
            else if (count < App._constants.ROSTER_CUT_TARGET_PITCHERS) { this.set("pitchers.isComplete", false); this.set("pitchers.isError", true ); }
            else                                                        { this.set("pitchers.isComplete", true ); this.set("pitchers.isError", false); }
        },
        updateBattersStatus: function() {
            var count = this.currentTeam.batters.reduce(function(value, batter){ return (batter.isCut) ? value : value + 1; }, 0);

            if      (count > App._constants.ROSTER_CUT_TARGET_BATTERS) { this.set("batters.isComplete", false); this.set("batters.isError", false); }
            else if (count < App._constants.ROSTER_CUT_TARGET_BATTERS) { this.set("batters.isComplete", false); this.set("batters.isError", true ); }
            else                                                       { this.set("batters.isComplete", true ); this.set("batters.isError", false); }
        },
        updateTeamStatus: function() {
            if   (this.pitchers.isComplete && this.batters.isComplete) this.currentTeam.set("isComplete", true );
            else                                                       this.currentTeam.set("isComplete", false);

            if   (this.pitchers.isError || this.batters.isError) this.currentTeam.set("isError", true );
            else                                                 this.currentTeam.set("isError", false);

            this.send("updateStatus");
        },
        updateStatus: function() {
            var count = this.teams.reduce(function(value, team){ return (team.isComplete) ? value : value + 1; }, 0);

            if   (count > 0) this.set("stageComplete", false);
            else             this.set("stageComplete", true );
        },
        loadRookies: function(rookies) {
            var promises = [];
            var controller = this;

            for (var i = 0; i < rookies.length; i++) {
                promises.push( loadPlayer(rookies[i], App._rookies, false) );
            }

            $.when.apply(null, promises).done(function() {
                App._rookies.set( "pitchers", App._rookies.pitchers.sortBy("rating").reverseObjects());
                App._rookies.set( "batters",  App._rookies.batters .sortBy("rating").reverseObjects());

                controller.get("controllers.rookie-draft").send('showFirstTeam');
                controller.get("controllers.progress").send('nextStage');
            });
        },
        finishStage: function() {
            var controller = this;

            if (this.stageComplete) {
                $.ajax( "/mba/resources/drafts/rookie/season/" + (App._season - 1), {
                    success: function(draft) {
                        App._rookies.set( "draftOrder", draft );

                        $.ajax( "/mba/resources/players?rookie=true", {
                            success: function(rookies) {
                                controller.send( "loadRookies", rookies );
                            },
                            error: function() {
                                alert("Error retrieving rookies!");
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

App.RookieDraftController = Ember.ObjectController.extend({
    needs:                   ["progress", "free-agents"],
    team:                    {},
    rookies:                 App._rookies,
    currentTeamIdx:          0,
    currentPitcherSortField: "",
    currentBatterSortField:  "",
    toggleRookieButtonLabel: "Show Batters",
    showRookiePitchers:      true,
    showRookieBatters:       false,
    canDraft:                true,
// TEMP
//    stageComplete:           false,
    stageComplete:           true,
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
            var url = App._utils.findLink( this.team.links, "players" ) + "/" + pitcher.player_id + "/season/" + App._season;

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
            var url = App._utils.findLink( this.team.links, "players" ) + "/" + batter.player_id + "/season/" + App._season;

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

            this.send("loadTeam", App._rookies.draftOrder[this.currentTeamIdx] );
        },
        showNextTeam: function() {
            var nextTeamIdx = this.currentTeamIdx + 1;

            if (nextTeamIdx == App._rookies.draftOrder.length) {
                this.set("canDraft",      false);
                this.set("stageComplete", true );
                return;
            }

            this.set("currentTeamIdx", nextTeamIdx);

            this.send("loadTeam", App._rookies.draftOrder[this.currentTeamIdx] );
        },
        loadTeam: function(teamId) {
            var controller = this;

            $.ajax( "/mba/resources/teams/" + teamId, {
                success: function(team) {
                    team.pitchers             = [];
                    team.batters              = [];
                    team.draftedRookiePitcher = false;
                    team.draftedRookieBatter  = false;

                    $.ajax( App._utils.findLink( team.links, "players" ) + "?season=" + App._season, {
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
                promises.push( loadPlayer(players[i], team, false) );
            }

            $.when.apply(null, promises).done(function() {
                controller.set("team", Ember.Object.create().setProperties(team));
                controller.send("setDraftStatus");
            });
        },
        setDraftStatus: function() {
            if (this.stageComplete) return;

            if (this.showRookiePitchers && this.team.draftedRookiePitcher) {
                this.set("canDraft", false);
                return;
            }

            if (this.showRookieBatters && this.team.draftedRookieBatter) {
                this.set("canDraft", false);
                return;
            }

            this.set("canDraft", true);
        },
        loadFreeAgents: function(freeAgents) {
            var promises = [];
            var controller = this;

            for (var i = 0; i < freeAgents.length; i++) {
                promises.push( loadPlayer(freeAgents[i], App._freeAgents, false) );
            }

            $.when.apply(null, promises).done(function() {
                controller.get("controllers.free-agents").send('showFirstTeam');
                controller.get("controllers.progress").send('nextStage');
            });
        },
        finishStage: function() {
            var controller = this;

            if (this.stageComplete) {
                $.ajax( "/mba/resources/drafts/free-agent/season/" + (App._season - 1), {
                    success: function(draft) {
                        App._freeAgents.set( "draftOrder", draft );

                        $.ajax( "/mba/resources/players?freeagent=true", {
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

App.FreeAgentsController = Ember.ObjectController.extend({
    needs:                      "progress",
    team:                       {},
    freeAgents:                 App._freeAgents,
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
            var url = App._utils.findLink( this.team.links, "players" ) + "/" + pitcher.player_id + "/season/" + App._season;

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
            var url = App._utils.findLink( this.team.links, "players" ) + "/" + batter.player_id + "/season/" + App._season;

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

            this.send("loadTeam", App._rookies.draftOrder[this.currentTeamIdx] );
        },
        showNextTeam: function() {
            var nextTeamIdx = this.currentTeamIdx + 1;

            if (nextTeamIdx == App._freeAgents.draftOrder.length) {
                this.set("canDraft",      false);
                this.set("stageComplete", true );
                return;
            }

            this.set("currentTeamIdx", nextTeamIdx);

            this.send("loadTeam", App._freeAgents.draftOrder[this.currentTeamIdx] );
        },
        loadTeam: function(teamId) {
            var controller = this;

            $.ajax( "/mba/resources/teams/" + teamId, {
                success: function(team) {
                    team.pitchers = [];
                    team.batters  = [];

                    $.ajax( App._utils.findLink( team.links, "players" ) + "?season=" + App._season, {
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
                promises.push( loadPlayer(players[i], team, false) );
            }

            $.when.apply(null, promises).done(function() {
                controller.set("team", Ember.Object.create().setProperties(team));
                controller.send("setDraftStatus");
            });
        },
        setDraftStatus: function() {
            if (this.stageComplete) return;

            if (this.showFreeAgentPitchers && (this.team.pitchers.length >= App._constants.TEAM_MAX_PITCHERS)) {
                this.set("canDraft", false);
                return;
            }

            if (this.showFreeAgentBatters && (this.team.batters.length >= App._constants.TEAM_MAX_BATTERS)) {
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

App.NextButtonController = Ember.ObjectController.extend({
    needs: ['progress']
});

App.NextButtonView = Ember.View.extend({
    templateName: "next-button",
    name: "Next",
    click: function(event) {
        this.get("controller.controllers.progress").send('nextStage');
    }
});

