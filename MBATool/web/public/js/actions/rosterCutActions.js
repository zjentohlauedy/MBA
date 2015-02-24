define(['objects/constants', 'objects/globals', 'utils'], function(Constants, Globals, Utils) {

    var loadPlayerDetails = function(team, players, defer) {
        var promises = [];

        for (var i = 0; i < players.length; i++) {
            promises.push( Utils.loadPlayer(players[i], team, Globals.season, true) );
        }

        $.when.apply(null, promises).done(function() {
            defer.resolve();
        });
    };

    var loadCurrentSeasonPlayers = function(team) {
        var defer = $.Deferred();

        $.ajax( team._links.players.href, {
            success: function(players) {
                defer.resolve(players);
            },
            error: function() {
                defer.reject();
            }
        });

        return defer.promise();
    };

    var loadPreviousSeasonPlayers = function(team) {
        var defer = $.Deferred();

        $.ajax( team._links.team.href + '/players?season=' + (Globals.season - 1), {
            success: function(players) {
                defer.resolve(players);
            },
            error: function() {
                defer.reject();
            }
        });

        return defer.promise();
    };

    var loadTeamPlayers = function(team) {
        var defer = $.Deferred();

        $.when( loadCurrentSeasonPlayers(team), loadPreviousSeasonPlayers(team) ).then(
            function(players_current, players_previous) {
                players_current. forEach(function(player) { player.isCut = false; });
                players_previous.forEach(function(player) { player.isCut = true;  });

                var players = Utils.mergePlayerLists(players_current, players_previous);

                loadPlayerDetails(team, players, defer);
            },
            function() {
                defer.reject();
            }
        );

        return defer.promise();
    };

    var RosterCutActions = {
        prepareData: function(controller, deferred) {
            $.ajax( Constants.TEAMS_URI + '?season=' + Globals.season, {
                success: function(teams) {
                    controller.set('teams', Utils.decorateTeams(teams));
                    deferred.resolve();
                },
                error: function() {
                    alert("Error loading teams!")
                    deferred.reject();
                }
            });
        },
        selectTeam: function(controller, team) {
            if (team.isSelected) return;

            team.set("isSelected", true);
            controller.currentTeam.set("isSelected", false);
            controller.set( "currentTeam", team );

            if ( team.pitchers.length == 0 ) {
                $.when(loadTeamPlayers(team)).then(
                    function() {
                        controller.currentTeam.set('batters', Ember.copy(Utils.sortBattersByPosition(team.batters)));

                        controller.send("updatePitchersStatus");
                        controller.send("updateBattersStatus");
                        controller.send("updateTeamStatus");
                    },
                    function() {
                        alert("Error loading team players!")
                    }
                );
            }
            else
            {
                controller.send("updatePitchersStatus");
                controller.send("updateBattersStatus");
            }
        },
        toggleCutPitcher: function(controller, pitcher) {
            var url = controller.currentTeam._links.team.href + "/players/" + pitcher.player_id + "/season/" + Globals.season;

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
            var url = controller.currentTeam._links.team.href + "/players/" + batter.player_id + "/season/" + Globals.season;

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
        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return RosterCutActions;
});
