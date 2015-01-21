define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var RosterCutActions = {
        selectTeam: function(controller, team) {
            if (team.isSelected) return;

            team.set("isSelected", true);
            controller.currentTeam.set("isSelected", false);
            controller.set( "currentTeam", team );


            if ( team.pitchers.length == 0 ) {
                $.ajax( "/mba/resources/teams/" + team.team_id + "/players?season=" + Globals.season + "&phase=1", {
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
                controller.send("updatePitchersStatus");
                controller.send("updateBattersStatus");
            }
        },
        loadPlayers: function(controller, team, players) {
            var promises = [];

            for (var i = 0; i < players.length; i++) {
                promises.push( Utils.loadPlayer(players[i], team, Globals.season, true) );
            }

            $.when.apply(null, promises).done(function() {
                controller.send("updatePitchersStatus");
                controller.send("updateBattersStatus");
                controller.send("updateTeamStatus");
            });
        },
        toggleCutPitcher: function(controller, pitcher) {
            var url = Utils.findLink( controller.currentTeam.links, "players" ) + "/" + pitcher.player_id + "/season/" + Globals.season;

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
        toggleCutBatter: function(controller, batter) {
            var url = Utils.findLink( controller.currentTeam.links, "players" ) + "/" + batter.player_id + "/season/" + Globals.season;

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
        updatePitchersStatus: function(controller) {
            var count = controller.currentTeam.pitchers.reduce(function(value, pitcher){ return (pitcher.isCut) ? value : value + 1; }, 0);

            if      (count > Constants.ROSTER_CUT_TARGET_PITCHERS) { controller.set("pitchers.isComplete", false); controller.set("pitchers.isError", false); }
            else if (count < Constants.ROSTER_CUT_TARGET_PITCHERS) { controller.set("pitchers.isComplete", false); controller.set("pitchers.isError", true ); }
            else                                                   { controller.set("pitchers.isComplete", true ); controller.set("pitchers.isError", false); }
        },
        updateBattersStatus: function(controller) {
            var count = controller.currentTeam.batters.reduce(function(value, batter){ return (batter.isCut) ? value : value + 1; }, 0);

            if      (count > Constants.ROSTER_CUT_TARGET_BATTERS) { controller.set("batters.isComplete", false); controller.set("batters.isError", false); }
            else if (count < Constants.ROSTER_CUT_TARGET_BATTERS) { controller.set("batters.isComplete", false); controller.set("batters.isError", true ); }
            else                                                  { controller.set("batters.isComplete", true ); controller.set("batters.isError", false); }
        },
        updateTeamStatus: function(controller) {
            if   (controller.pitchers.isComplete && controller.batters.isComplete) controller.currentTeam.set("isComplete", true );
            else                                                                   controller.currentTeam.set("isComplete", false);

            if   (controller.pitchers.isError || controller.batters.isError) controller.currentTeam.set("isError", true );
            else                                                             controller.currentTeam.set("isError", false);

            controller.send("updateStatus");
        },
        updateStatus: function(controller) {
            var count = controller.teams.reduce(function(value, team){ return (team.isComplete) ? value : value + 1; }, 0);

            if   (count > 0) controller.set("stageComplete", false);
            else             controller.set("stageComplete", true );
        },
        loadRookies: function(controller, rookies) {
            var promises = [];

            for (var i = 0; i < rookies.length; i++) {
                promises.push( Utils.loadPlayer(rookies[i], controller.rookies, Globals.season, false) );
            }

            $.when.apply(null, promises).done(function() {
                controller.rookies.set( "pitchers", controller.rookies.pitchers.sortBy("rating").reverseObjects());
                controller.rookies.set( "batters",  controller.rookies.batters .sortBy("rating").reverseObjects());

                controller.get("controllers.rookie-draft").set('rookies', controller.rookies);
                controller.get("controllers.rookie-draft").send('showFirstTeam');
                controller.get("controllers.progress").send('nextStage');
            });
        },
        finishStage: function(controller) {
            if (controller.stageComplete) {
                $.ajax( "/mba/resources/drafts/rookie/season/" + (Globals.season - 1), {
                    success: function(draft) {
                        controller.rookies.set( "draftOrder", draft );

                        $.ajax( "/mba/resources/players?rookie=true&season=" + Globals.season, {
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
    };

    return RosterCutActions;
});
