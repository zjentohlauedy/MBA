define(['ember'], function(Ember) {

    var Utils = {
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
        decoratePitcher: function(player, stats, isCut) {

            player.isCut      = isCut;
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
        decorateBatter: function(player, stats, isCut) {

            player.isCut      = isCut;
            player.isSelected = false;
            player.rating     = player.power + player.hit_n_run + player.running;

            if (stats) {
                avg      = (stats.at_bats == 0) ? 0 : stats.hits / stats.at_bats;
                singles  = (stats.hits - (stats.doubles + stats.triples + stats.home_runs));
                slugging = (stats.at_bats == 0) ? 0 : (singles + (2 * stats.doubles) + (3 * stats.triples) + (4 * stats.home_runs)) / stats.at_bats;
                oba      = (stats.at_bats == 0) ? 0 : (stats.hits + stats.walks) / (stats.at_bats + stats.walks);
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
        mergePlayerLists: function(list1, list2) {
            var resultList = [];

            list1.forEach(function(player) {
                resultList.push(player);
            });

            list2.forEach(function(player) {
                var found = false;

                for (var i = 0; i < resultList.length; ++i) {
                    if (resultList[i].player_id === player.player_id) {
                        found = true;
                    }
                }

                if (!found) { resultList.push(player) }
            });

            return resultList.sort(function(p1,p2) { return p1.player_id - p2.player_id; });
        },
        sortBattersByPosition: function(batters) {
            return batters.sort(function(p1, p2) {
                var posMap = function(p) {
                    if (p ===  'C') { return 0; }
                    if (p === '1B') { return 1; }
                    if (p === '2B') { return 2; }
                    if (p === '3B') { return 3; }
                    if (p === 'SS') { return 4; }
                    if (p === 'LF') { return 5; }
                    if (p === 'CF') { return 6; }
                    if (p === 'RF') { return 7; }
                    return 8;
                };

                return posMap(p1.primary_position) - posMap(p2.primary_position);
            });
        },
        loadPlayer: function(player, team, season, needStats) {
            var deferred = $.Deferred();

            $.ajax( player._links.self.href, {
                success: function(playerDetails) {

                    if (needStats) {
                        $.ajax( playerDetails._links.player.href + "/stats?season=" + (season - 1) + "&phase=1", {
                            success: function(playerStats) {

                                if (playerDetails.player_type === "Pitcher") {
                                    team.pitchers.addObject(Utils.decoratePitcher(playerDetails, playerStats, player.isCut));
                                }

                                if (playerDetails.player_type === "Batter") {
                                    team.batters. addObject(Utils.decorateBatter( playerDetails, playerStats, player.isCut));
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
                        team.pitchers.addObject(Utils.decoratePitcher(playerDetails, null, player.isCut));
                    }

                    if (playerDetails.player_type === "Batter") {
                        team.batters. addObject(Utils.decorateBatter( playerDetails, null, player.isCut));
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
    };

    return Utils;
});
