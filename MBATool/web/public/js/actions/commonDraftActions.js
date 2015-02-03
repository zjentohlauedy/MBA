define(['objects/globals', 'utils'], function(Globals, Utils) {

    var loadPlayerDetails = function(deferred, team, players) {
        var promises = [];

        players.forEach(function(player) {
            promises.push( Utils.loadPlayer(player, team, Globals.season, false) );
        });

        $.when.apply(null, promises).done(function() {
            deferred.resolve(team);
        });
    }

    var loadPlayers = function(controller) {
        var deferred = $.Deferred();

        $.ajax( controller.playersResource(), {
            success: function(players) {
                loadPlayerDetails(deferred, controller.availablePlayers, players);
            },
            error: function() {
                alert("Error retrieving free agents!");

                deferred.reject();
            }
        });

        return deferred.promise();
    }

    var loadTeamPlayers = function(controller, deferred, team) {
        $.ajax( Utils.findLink( team.links, "players" ) + "?season=" + Globals.season, {
            success: function(players) {
                loadPlayerDetails(deferred, team, players);
            },
            error: function() {
                alert("Error retrieving team players!");

                deferred.reject();
            }
        });
    }

    var loadTeam = function(controller) {
        var defer = $.Deferred();
        var teamId = controller.draftOrder[controller.currentTeamIdx];

        $.ajax( "/mba/resources/teams/" + teamId, {
            success: function(team) {
                team.pitchers = [];
                team.batters  = [];

                loadTeamPlayers(controller, defer, team);
            },
            error: function() {
                alert("Error retrieving team!");

                defer.reject();
            }
        });

        return defer.promise();
    }

    var loadDraft = function(controller) {
        var defer = $.Deferred();

        $.ajax( controller.draftResource(), {
            success: function(draft) {
                defer.resolve(draft);
            },
            error: function() {
                alert("Error retrieving free agent draft!");

                defer.reject();
            }
        });

        return defer.promise();
    }

    var draftPlayer = function(controller, player, availablePlayers, teamPlayers) {
        var url = Utils.findLink( controller.team.links, "players" ) + "/" + player.player_id + "/season/" + Globals.season;

        $.ajax( url, {
            type: 'POST',
            success: function() {
                availablePlayers.removeObject(player);
                teamPlayers.addObject(player);
                controller.send("showNextTeam");
            },
            error: function() {
                alert("Error drafting player!");
            }
        });
    }

    var CommonDraftActions = {
        prepareData: function(controller, deferred) {
            var draftPromise   = loadDraft(controller);
            var playersPromise = loadPlayers(controller);

            $.when(draftPromise, playersPromise).then(function(draft) {
                controller.set("draftOrder", draft);
                controller.set("currentTeamIdx", 0);

                var teamPromise = loadTeam(controller);

                $.when(teamPromise).then(function(team) {
                    controller.set('team', team);
                    controller.send('setDraftStatus');

                    deferred.resolve();
                }, function() {
                    deferred.reject();
                });
            }, function() {
                deferred.reject();
            });
        },
        toggleAvailablePlayersTable: function(controller) {
            if (controller.showAvailablePitchers) {
                controller.set("showAvailablePitchers", false);
                controller.set("showAvailableBatters",  true );

                controller.set("toggleButtonLabel", "Show Pitchers");

                controller.availablePlayers.pitchers.setEach("isSelected", false);
            }
            else {
                controller.set("showAvailablePitchers", true );
                controller.set("showAvailableBatters",  false);

                controller.set("toggleButtonLabel", "Show Batters");

                controller.availablePlayers.batters.setEach("isSelected", false);
            }

            controller.send("setDraftStatus");
        },
        sortPitchers: function(controller, field) {
            if (field === controller.currentPitcherSortField) {
                controller.availablePlayers.set("pitchers", controller.availablePlayers.pitchers.reverseObjects());
                return;
            }

            controller.set("currentPitcherSortField", field);

            controller.availablePlayers.set("pitchers", controller.availablePlayers.pitchers.sortBy(field));
        },
        sortBatters: function(controller, field) {
            if (field === controller.currentBatterSortField) {
                controller.availablePlayers.set("batters", controller.availablePlayers.batters.reverseObjects());
                return;
            }

            controller.set("currentBatterSortField", field);

            controller.availablePlayers.set("batters", controller.availablePlayers.batters.sortBy(field));
        },
        selectAvailablePlayer: function(controller, players, player) {
            if ( player.isSelected ) {
                player.set('isSelected', false);
                return;
            }

            players.setEach('isSelected', false);
            player.set('isSelected', true);
        },
        draftSelectedPlayer: function(controller) {
            if (!controller.canDraft) return;

            var player;
            if (player = controller.availablePlayers.pitchers.findBy("isSelected")) {
                draftPlayer(controller, player, controller.availablePlayers.pitchers, controller.team.pitchers);
            }
            else if (player = controller.availablePlayers.batters. findBy("isSelected")) {
                draftPlayer(controller, player, controller.availablePlayers.batters, controller.team.batters);
            }
        },
        showNextTeam: function(controller) {
            var nextTeamIdx = controller.currentTeamIdx + 1;

            if (nextTeamIdx == controller.draftOrder.length) {
                controller.set("canDraft",      false);
                controller.set("stageComplete", true );
                return;
            }

            controller.set("currentTeamIdx", nextTeamIdx);

            var teamPromise = loadTeam(controller);

            $.when(teamPromise).then(function(team) {
                controller.set('team', team);
                controller.send('setDraftStatus');
            });
        },
        setDraftStatus: function(controller) {
            if (controller.stageComplete) return;

            if (controller.showAvailablePitchers && (controller.team.pitchers.length >= controller.maxPitchers)) {
                controller.set("canDraft", false);
                return;
            }

            if (controller.showAvailableBatters && (controller.team.batters.length >= controller.maxBatters)) {
                controller.set("canDraft", false);
                return;
            }

            controller.set("canDraft", true);
        }
    };

    return CommonDraftActions;
});
