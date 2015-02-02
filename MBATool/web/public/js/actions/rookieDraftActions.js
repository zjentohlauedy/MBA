define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var loadRookieDetails = function(controller, deferred, rookies) {
        var promises = [];

        rookies.forEach(function(rookie) {
            promises.push( Utils.loadPlayer(rookie, controller.rookies, Globals.season, false) );
        });

        $.when.apply(null, promises).done(function() {
            controller.rookies.set( "pitchers", controller.rookies.pitchers.sortBy("rating").reverseObjects());
            controller.rookies.set( "batters",  controller.rookies.batters .sortBy("rating").reverseObjects());
            controller.send('showFirstTeam');

            deferred.resolve();
        });
    }

    var loadRookies = function(controller, deferred) {
        $.ajax( "/mba/resources/players?rookie=true&season=" + Globals.season, {
            success: function(rookies) {
                loadRookieDetails(controller, deferred, rookies);
            },
            error: function() {
                alert("Error retrieving rookies!");

                deferred.reject();
            }
        });
    }

    var RookieDraftActions = {
        prepareData: function(controller, deferred) {
            $.ajax( "/mba/resources/drafts/rookie/season/" + (Globals.season - 1), {
                success: function(draft) {
                    controller.rookies.set( "draftOrder", draft );

                    loadRookies(controller, deferred);
                },
                error: function() {
                    alert("Error retrieving rookie draft!");

                    deferred.reject();
                }
            });
        },
        toggleRookieTable: function(controller) {
            if (controller.showRookiePitchers) {
                controller.set("showRookiePitchers", false);
                controller.set("showRookieBatters",  true );

                controller.set("toggleRookieButtonLabel", "Show Pitchers");

                controller.rookies.pitchers.setEach("isSelected", false);
            }
            else {
                controller.set("showRookiePitchers", true );
                controller.set("showRookieBatters",  false);

                controller.set("toggleRookieButtonLabel", "Show Batters");

                controller.rookies.batters.setEach("isSelected", false);
            }

            controller.send("setDraftStatus");
        },
        sortPitchers: function(controller, field) {
            if (field === controller.currentPitcherSortField) {
                controller.rookies.set("pitchers", controller.rookies.pitchers.reverseObjects());
                return;
            }

            controller.set("currentPitcherSortField", field);

            controller.rookies.set("pitchers", controller.rookies.pitchers.sortBy(field, "rating"));
        },
        sortBatters: function(controller, field) {
            if (field === controller.currentBatterSortField) {
                controller.rookies.set("batters", controller.rookies.batters.reverseObjects());
                return;
            }

            controller.set("currentBatterSortField", field);

            controller.rookies.set("batters", controller.rookies.batters.sortBy(field, "rating"));
        },
        selectRookiePitcher: function(controller, pitcher) {
            if (pitcher.isSelected) {
                pitcher.set("isSelected", false);
                return;
            }

            controller.rookies.pitchers.setEach("isSelected", false);

            pitcher.set("isSelected", true);
        },
        selectRookieBatter: function(controller, batter) {
            if (batter.isSelected) {
                batter.set("isSelected", false);
                return;
            }

            controller.rookies.batters.setEach("isSelected", false);

            batter.set("isSelected", true);
        },
        draftRookie: function(controller) {
            if (!controller.canDraft) return;

            var rookie;
            if      (rookie = controller.rookies.pitchers.findBy("isSelected")) { controller.send("draftPitcher", rookie); }
            else if (rookie = controller.rookies.batters. findBy("isSelected")) { controller.send("draftBatter",  rookie); }
        },
        draftPitcher: function(controller, pitcher) {
            var url = Utils.findLink( controller.team.links, "players" ) + "/" + pitcher.player_id + "/season/" + Globals.season;

            $.ajax( url, {
                type: 'POST',
                success: function() {
                    controller.rookies.pitchers.removeObject(pitcher);
                    controller.team.   pitchers.   addObject(pitcher);
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
                    controller.rookies.batters.removeObject(batter);
                    controller.team   .batters.   addObject(batter);
                    controller.send("showNextTeam");
                },
                error: function() {
                    alert("Error drafting pitcher!");
                }
            });
        },
        showFirstTeam: function(controller) {
            controller.set("currentTeamIdx", 0);

            controller.send("loadTeam", controller.rookies.draftOrder[controller.currentTeamIdx] );
        },
        showNextTeam: function(controller) {
            var nextTeamIdx = controller.currentTeamIdx + 1;

            if (nextTeamIdx == controller.rookies.draftOrder.length) {
                controller.set("canDraft",      false);
                controller.set("stageComplete", true );
                return;
            }

            controller.set("currentTeamIdx", nextTeamIdx);

            controller.send("loadTeam", controller.rookies.draftOrder[controller.currentTeamIdx] );
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

            players.forEach(function(player) {
                promises.push( Utils.loadPlayer(player, team, Globals.season, false) );
            });

            $.when.apply(null, promises).done(function() {
                controller.set("team", Ember.Object.create().setProperties(team));
                controller.send("setDraftStatus");
            });
        },
        setDraftStatus: function(controller) {
            if (controller.stageComplete) return;

            if (controller.showRookiePitchers && (controller.team.pitchers.length >= Constants.ROOKIE_DRAFT_MAX_PITCHERS)) {
                controller.set("canDraft", false);
                return;
            }

            if (controller.showRookieBatters && (controller.team.batters.length >= Constants.ROOKIE_DRAFT_MAX_BATTERS)) {
                controller.set("canDraft", false);
                return;
            }

            controller.set("canDraft", true);
        },
        finishStage: function(controller) {
            if (controller.stageComplete) {
                var deferred = $.Deferred();

                controller.get("controllers.free-agents").send('prepareData', deferred);

                deferred.promise().then(function() {

                    controller.get("controllers.progress").send('nextStage');
                });
            }
        }
    };

    return RookieDraftActions;
});
