define(['objects/constants', 'objects/globals', 'sprintf'], function(Constants, Globals, Sprintf) {

    var AccoladesActions = {
        prepareData: function(controller, deferred) {
            $.ajax( '/mba/resources/accolades', {
                success: function(accolades) {
                    filteredAccolades = accolades.filter(function(element) { return ! element.automatic });

                    for (var i = 0; i < filteredAccolades.length; ++i) {
                        filteredAccolades[i].id = i + 1;
                    }

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
                var deferred = $.Deferred();

                var accoladeValue = null;
                var accoladeType  = null;

                controller.availableAccolades.forEach(function(availableAccolade) {
                    if (availableAccolade.id === accolade.selectedAccolade) {
                        accoladeValue = availableAccolade.value;
                        accoladeType  = availableAccolade.type;
                    }
                });

                $.ajax( Constants.PLAYERS_URI + '/' + accolade.selectedPlayer + '/accolades', {
                    type: 'POST',
                    contentType: 'application/json',
                    data: JSON.stringify({
                        player_id: accolade.selectedPlayer,
                        season:    Globals.season,
                        accolade:  accoladeValue,
                        type:      accoladeType
                    }),
                    success: function() {
                        deferred.resolve();
                    },
                    error: function(response) {
                        var message = null;

                        try {
                            message = JSON.parse(response.responseText).error;
                        }
                        catch (e) {
                            console.error("Error response: " + response.responseText);
                            message = "Unknown error, check console log.";
                        }

                        controller.errorMessages.addObject(message);
                        deferred.reject();
                    }
                });

                promises.push(deferred.promise());
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
                    var message = null;

                    try {
                        message = JSON.parse(response.responseText).error;
                    }
                    catch (e) {
                        console.error("Error response: " + response.responseText);
                        message = "Unknown error, check console log.";
                    }

                    controller.errorMessages.addObject(message);
                    controller.set('canSave', true);
                }
            });
        },
        displayAccolades: function(controller, accolades) {
            var players  = {};
            var promises = [];

            accolades.forEach(function(accolade) {
                if ( ! players[accolade.player_id] ) {
                    var deferred = $.Deferred();

                    players[accolade.player_id] = {};

                    $.ajax(Constants.PLAYERS_URI + '/' + accolade.player_id, {
                        success: function(player) {
                            player.accolades = [];

                            players[player.player_id] = player;

                            deferred.resolve();
                        },
                        error: function(response) {
                            var message = null;

                            try {
                                message = JSON.parse(response.responseText).error;
                            }
                            catch (e) {
                                console.error("Error response: " + response.responseText);
                                message = "Unknown error, check console log.";
                            }

                            controller.errorMessages.addObject(message);

                            deferred.reject();
                        }
                    });

                    promises.push(deferred.promise());
                }
            });

            $.when.apply(null, promises).then(function() {
                accolades.forEach(function(accolade) {
                    player = players[accolade.player_id];

                    player.accolades.push(accolade.accolade);
                });

                $.map(players, function(player) { return player; }).sort(function(object, other) {
                    if (other.accolades.length !== object.accolades.length) {
                        return other.accolades.length - object.accolades.length;
                    }

                    return object.name.localeCompare(other.name);
                }).forEach(function(player) {
                    controller.playerAccolades.addObject(player);
                });
            }, function() {
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
