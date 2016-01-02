define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    function filterAccolades(accolades) {
        return accolades.filter(function(element) { return ! element.automatic });
    };

    function assignAccoladeIds(accolades) {
        for (var i = 0; i < accolades.length; ++i) {
            accolades[i].id = i + 1;
        }
    };

    function getAccoladeDetails(controller, id) {
        for (var i = 0; i < controller.availableAccolades.length; ++i) {
            if (controller.availableAccolades[i].id === id) {
                return controller.availableAccolades[i];
            }
        }
    }

    function getMessageFromErrorResponse(response) {
        try {
            return JSON.parse(response.responseText).error;
        }
        catch (e) {
            console.error("Error response: " + response.responseText);
        }

        return "Unknown error, check console log.";
    }

    function postPlayerAccolade(controller, player, accolade) {
        var deferred = $.Deferred();

        var accoladeDetails = getAccoladeDetails(controller, accolade);

        $.ajax( Constants.PLAYERS_URI + '/' + player + '/accolades', {
            type: 'POST',
            contentType: 'application/json',
            data: JSON.stringify({
                player_id: player,
                season:    Globals.season,
                accolade:  accoladeDetails.value,
                type:      accoladeDetails.type
            }),
            success: function() {
                deferred.resolve();
            },
            error: function(response) {
                controller.errorMessages.addObject(getMessageFromErrorResponse(response));
                deferred.reject();
            }
        });

        return deferred.promise();
    }

    function distributeAccolades(players, accolades) {
        accolades.forEach(function(accolade) {
            player = players[accolade.player_id];

            player.accolades.push(accolade.accolade);
        });
    }

    function getPlayerListSortedByAccoladeCount(players) {
        return $.map(players, function(player) { return player; }).sort(function(object, other) {
            if (other.accolades.length !== object.accolades.length) {
                return other.accolades.length - object.accolades.length;
            }

            return object.name.localeCompare(other.name);
        });
    }

    function retrievePlayerDetails(controller, players, player_id) {
        var deferred = $.Deferred();

        $.ajax(Constants.PLAYERS_URI + '/' + player_id, {
            success: function(player) {
                player.accolades = [];

                players[player.player_id] = player;

                deferred.resolve();
            },
            error: function(response) {
                controller.errorMessages.addObject(getMessageFromErrorResponse(response));
                deferred.reject();
            }
        });

        return deferred.promise();
    }

    var AccoladesActions = {
        prepareData: function(controller, deferred) {
            $.ajax( '/mba/resources/accolades', {
                success: function(accolades) {
                    var filteredAccolades = filterAccolades(accolades);

                    assignAccoladeIds(filteredAccolades);

                    controller.set('availableAccolades', filteredAccolades);
                    deferred.resolve();
                },
                error: function() {
                    alert("Error loading accolades!")
                    deferred.reject();
                }
            });
        },
        saveAccolades: function(controller) {
            if (! controller.canSave) return;

            controller.set('canSave', false);
            controller.errorMessages.clear();

            var promises = [];

            controller.accoladeList.forEach(function(accolade) {
                promises.push(postPlayerAccolade(controller, accolade.selectedPlayer, accolade.selectedAccolade));
            });

            $.when.apply(null, promises).then(function() {
                controller.send('resolveAccolades');
            }, function() {
                controller.set('canSave', true);
            });
        },
        resolveAccolades: function(controller) {
            $.ajax(Constants.RESOLVE_ACCOLADES_URI, {
                type: 'POST',
                success: function(accolades) {
                    controller.send('displayAccolades', accolades);
                },
                error: function(response) {
                    controller.errorMessages.addObject(getMessageFromErrorResponse(response));
                    controller.set('canSave', true);
                }
            });
        },
        displayAccolades: function(controller, accolades) {
            var players  = {};
            var promises = [];

            accolades.forEach(function(accolade) {
                if ( ! players[accolade.player_id] ) {
                    players[accolade.player_id] = {};

                    promises.push(retrievePlayerDetails(controller, players, accolade.player_id));
                }
            });

            $.when.apply(null, promises).then(function success() {
                distributeAccolades(players, accolades);

                getPlayerListSortedByAccoladeCount(players).forEach(function(player) {
                    controller.playerAccolades.addObject(player);
                });

                controller.set('stageComplete', true);
            }, function error() {
                controller.set('canSave', true);
            });
        },
        finishStage: function(controller) {
            if (controller.stageComplete) {
                controller.get("controllers.progress").send('nextStage');
            }
        }
    };

    return AccoladesActions;
});
