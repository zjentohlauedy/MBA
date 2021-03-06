define(['utils'], function(Utils) {

    describe('Utils:', function() {

        describe('decorateTeams', function() {

            it('should add empty pitchers and batters lists', function() {
                var teams = [{},{},{}];

                Utils.decorateTeams(teams);

                expect(teams[0].pitchers).toEqual([]);
                expect(teams[0].batters ).toEqual([]);

                expect(teams[1].pitchers).toEqual([]);
                expect(teams[1].batters ).toEqual([]);

                expect(teams[2].pitchers).toEqual([]);
                expect(teams[2].batters ).toEqual([]);
            });

            it('should add flags', function() {
                var teams = [{},{},{}];

                Utils.decorateTeams(teams);

                for ( var i = 0; i < teams.length; i++ ) {
                    expect(teams[i].isSelected          ).toBe(false);
                    expect(teams[i].isComplete          ).toBe(false);
                    expect(teams[i].isError             ).toBe(false);
                    expect(teams[i].draftedRookiePitcher).toBe(false);
                    expect(teams[i].draftedRookieBatter ).toBe(false);
                }
            });
        });

        describe('decoratePitcher', function() {

            it('should add the selected flag', function() {
                var player = {};

                Utils.decoratePitcher(player, null);

                expect(player.isSelected).toBe(false);
            });

            it('should add the cut flag based on the isCut argument', function() {
                var player = {};

                Utils.decoratePitcher(player, null, true);

                expect(player.isCut).toBe(true);
            });

            it('should calculate the player rating', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };

                Utils.decoratePitcher(player, null);

                expect(player.rating).toEqual(17);
            });

            it('should copy base stats to the player given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { wins: 21, losses: 13, games: 22, saves: 12, hits: 33, earned_runs: 10, home_runs: 5, walks: 11, strike_outs: 9 };

                Utils.decoratePitcher(player, stats);

                expect(player.wins       ).toEqual(stats.wins       );
                expect(player.losses     ).toEqual(stats.losses     );
                expect(player.games      ).toEqual(stats.games      );
                expect(player.saves      ).toEqual(stats.saves      );
                expect(player.hits       ).toEqual(stats.hits       );
                expect(player.earned_runs).toEqual(stats.earned_runs);
                expect(player.home_runs  ).toEqual(stats.home_runs  );
                expect(player.walks      ).toEqual(stats.walks      );
                expect(player.strike_outs).toEqual(stats.strike_outs);
            });

            it('should calculate the innings pitched given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 221, outs: 1 };

                Utils.decoratePitcher(player, stats);

                expect(player.innings).toEqual('221.1');
            });

            it('should calculate the pitcher ERA given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, earned_runs: 18 };

                Utils.decoratePitcher(player, stats);

                expect(player.era).toEqual('2.25');
            });

            it('should calculate the pitcher vs. batting average given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, earned_runs: 18, walks: 20, hits: 35 };

                Utils.decoratePitcher(player, stats);

                expect(player.vsba).toEqual('.129');
            });

            it('should calculate the pitcher innings per game given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, games: 10 };

                Utils.decoratePitcher(player, stats);

                expect(player.ipg).toEqual('7.20');
            });

            it('should calculate the pitcher WHIP given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, walks: 10, hits: 35 };

                Utils.decoratePitcher(player, stats);

                expect(player.whip).toEqual('0.625');
            });

            it('should calculate the pitcher strike outs per 9 innings given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, strike_outs: 38 };

                Utils.decoratePitcher(player, stats);

                expect(player.sop9).toEqual('4.75');
            });

            it('should calculate the pitcher home runs per 9 innings given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, home_runs: 38 };

                Utils.decoratePitcher(player, stats);

                expect(player.hrp9).toEqual('4.75');
            });

            it('should calculate the pitcher efficiency given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, outs: 0, hits: 60, strike_outs: 86 };

                Utils.decoratePitcher(player, stats);

                expect(player.eff).toEqual('+4.75');
            });
        });

        describe('decorateBatter', function() {

            it('should add selected flag', function() {
                var player = {};

                Utils.decorateBatter(player, null);

                expect(player.isSelected).toBe(false);
            });

            it('should add the cut flag based on the isCut argument', function() {
                var player = {};

                Utils.decorateBatter(player, null, true);

                expect(player.isCut).toBe(true);
            });

            it('should calculate the player rating', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };

                Utils.decorateBatter(player, null);

                expect(player.rating).toEqual(18);
            });

            it('should copy base stats to the player given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { games: 149, at_bats: 516, runs: 101, hits: 214, doubles: 26, triples: 9, home_runs: 48, runs_batted_in: 112, walks: 49, strike_outs: 109, steals: 27, errors: 4 };

                Utils.decorateBatter(player, stats);

                expect(player.games         ).toEqual(stats.games         );
                expect(player.at_bats       ).toEqual(stats.at_bats       );
                expect(player.runs          ).toEqual(stats.runs          );
                expect(player.hits          ).toEqual(stats.hits          );
                expect(player.doubles       ).toEqual(stats.doubles       );
                expect(player.triples       ).toEqual(stats.triples       );
                expect(player.home_runs     ).toEqual(stats.home_runs     );
                expect(player.runs_batted_in).toEqual(stats.runs_batted_in);
                expect(player.walks         ).toEqual(stats.walks         );
                expect(player.strike_outs   ).toEqual(stats.strike_outs   );
                expect(player.steals        ).toEqual(stats.steals        );
                expect(player.errors        ).toEqual(stats.errors        );
            });

            it('should calculate the player batting average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { hits: 100, at_bats: 400 };

                Utils.decorateBatter(player, stats);

                expect(player.avg).toEqual('.250');
            });

            it('should calculate the player slugging average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { hits: 100, at_bats: 400, doubles: 16, triples: 4, home_runs: 12 };

                Utils.decorateBatter(player, stats);

                expect(player.slugging).toEqual('.400');
            });

            it('should calculate the player on base average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { hits: 75, at_bats: 375, walks: 25 };

                Utils.decorateBatter(player, stats);

                expect(player.oba).toEqual('.250');
            });

            it('should calculate the player strike out average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { at_bats: 400, strike_outs: 100 };

                Utils.decorateBatter(player, stats);

                expect(player.soa).toEqual('.250');
            });

            it('should calculate the player runs per game given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { games: 148, home_runs: 35, runs: 64, runs_batted_in: 88 };

                Utils.decorateBatter(player, stats);

                expect(player.rpg).toEqual('0.79');
            });
        });

        describe('mergePlayerLists', function() {

            it('should return a list of players', function() {
                var result = Utils.mergePlayerLists([], []);

                expect(result).toEqual([]);
            });

            it('should combine the player lists', function() {
                var result = Utils.mergePlayerLists([{player_id: 1}], [{player_id: 2},{player_id: 3}]);

                expect(result.length).toBe(3);
            });

            it('should not return duplicates', function() {
                var result = Utils.mergePlayerLists([{player_id: 1}], [{player_id: 1},{player_id: 3}]);

                expect(result.length).toBe(2);
            });

            it('should always keep players from the first array when there are duplicates', function() {
                var list1 = [{player_id: 1, season: 3},{player_id: 2, season: 3},{player_id: 4, season: 3}];
                var list2 = [{player_id: 1, season: 4},{player_id: 2, season: 4},{player_id: 3, season: 4}];

                var result = Utils.mergePlayerLists(list1,list2);

                expect(result.length).toBe(4);

                expect(result[0].season).toBe(3);
                expect(result[1].season).toBe(3);
                expect(result[2].season).toBe(4);
                expect(result[3].season).toBe(3);
            });

            it('should sort the returned list by player id', function() {
                var result = Utils.mergePlayerLists([{player_id: 2}],[{player_id: 1}]);

                expect(result[0].player_id).toBe(1);
                expect(result[1].player_id).toBe(2);
            });
        });

        describe('sortBattersByPosition', function() {

            it('should return an array with the same number of batters', function() {
                var batters = [{},{},{},{},{}];

                var result = Utils.sortBattersByPosition(batters);

                expect(result.length).toBe(batters.length);
            });

            it('should order batters in baseball position order', function() {
                var batters = [{player_id: 1, primary_position: 'LF'},
                               {player_id: 2, primary_position: 'SS'},
                               {player_id: 3, primary_position: 'RF'},
                               {player_id: 4, primary_position: '1B'},
                               {player_id: 5, primary_position:  'C'},
                               {player_id: 6, primary_position: '2B'},
                               {player_id: 7, primary_position: 'CF'},
                               {player_id: 8, primary_position: '3B'}];

                var result = Utils.sortBattersByPosition(batters);

                expect(result[0].player_id).toBe(5);
                expect(result[1].player_id).toBe(4);
                expect(result[2].player_id).toBe(6);
                expect(result[3].player_id).toBe(8);
                expect(result[4].player_id).toBe(2);
                expect(result[5].player_id).toBe(1);
                expect(result[6].player_id).toBe(7);
                expect(result[7].player_id).toBe(3);
            });

            it('should put undefined positions at the end', function() {
                var batters = [{player_id: 1},{player_id: 2, primary_position: 'LF'}];

                var result = Utils.sortBattersByPosition(batters);

                expect(result[0].player_id).toBe(2);
                expect(result[1].player_id).toBe(1);
            });
        });

        describe('loadPlayer', function() {

            it('should add a pitcher without stats to the given team pitcher list', function() {
                var team    = { pitchers: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success({player_type: 'Pitcher'});
                });

                Utils.loadPlayer(player, team, 5, false);

                expect(team.pitchers.length).toBe(1);
            });

            it('should retrieve the pitcher without stats from the rest service using the self link in the player object', function() {
                var selfUrl = 'http://localhost/rest/resource/thing/5';
                var team    = { pitchers: [] };
                var player  = { _links: {self: {rel: 'self', href: selfUrl}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success({player_type: 'Pitcher'});
                });

                Utils.loadPlayer(player, team, 5, false);

                expect($.ajax).toHaveBeenCalledWith(selfUrl, jasmine.any(Object));
            });

            it('should decorate the pitcher without stats', function() {
                var team          = { pitchers: [] };
                var player        = { isCut: false, _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Pitcher' };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success(playerDetails);
                });

                spyOn(Utils, 'decoratePitcher').and.callThrough();

                Utils.loadPlayer(player, team, 5, false);

                expect(Utils.decoratePitcher).toHaveBeenCalledWith(playerDetails, null, player.isCut);
            });

            it('should add a pitcher with stats to the given team pitcher list', function() {
                var team          = { pitchers: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Pitcher', _links: {player: {rel: 'player', href: 'playerUrl'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success({});
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                Utils.loadPlayer(player, team, 5, true);

                expect(team.pitchers.length).toBe(1);
            });

            it('should retrieve the pitcher stats from the rest service using the stats link in the player details object', function() {
                var team          = { pitchers: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Pitcher', _links: {player: {rel: 'player', href: 'playerUrl'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success({});
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                Utils.loadPlayer(player, team, 5, true);

                expect($.ajax).toHaveBeenCalledWith('playerUrl/stats?season=4&phase=1', jasmine.any(Object));
            });

            it('should decorate the pitcher with stats', function() {
                var team          = { pitchers: [] };
                var player        = { isCut: false, _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Pitcher', _links: {player: {rel: 'player', href: 'playerUrl'}} };
                var pitcherStats  = { innings: 100, outs: 0 };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success(pitcherStats);
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                spyOn(Utils, 'decoratePitcher').and.callThrough();

                Utils.loadPlayer(player, team, 5, true);

                expect(Utils.decoratePitcher).toHaveBeenCalledWith(playerDetails, pitcherStats, player.isCut);
            });

            it('should add a batter without stats to the given team batter list', function() {
                var team    = { batters: [] };
                var player  = { _links: {self: {rel: 'self', href: 'url'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success({player_type: 'Batter'});
                });

                Utils.loadPlayer(player, team, 5, false);

                expect(team.batters.length).toBe(1);
            });

            it('should retrieve the batter without stats from the rest service using the self link in the player object', function() {
                var selfUrl = 'http://localhost/rest/resource/thing/5';
                var team    = { batters: [] };
                var player  = { _links: {self: {rel: 'self', href: selfUrl}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success({player_type: 'Batter'});
                });

                Utils.loadPlayer(player, team, 5, false);

                expect($.ajax).toHaveBeenCalledWith(selfUrl, jasmine.any(Object));
            });

            it('should decorate the batter without stats', function() {
                var team          = { batters: [] };
                var player        = { isCut: false, _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter' };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success(playerDetails);
                });

                spyOn(Utils, 'decorateBatter').and.callThrough();

                Utils.loadPlayer(player, team, 5, false);

                expect(Utils.decorateBatter).toHaveBeenCalledWith(playerDetails, null, player.isCut);
            });

            it('should add a batter with stats to the given team batter list', function() {
                var team          = { batters: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter', _links: {player: {rel: 'player', href: 'playerUrl'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success({});
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                Utils.loadPlayer(player, team, 5, true);

                expect(team.batters.length).toBe(1);
            });

            it('should retrieve the batter stats from the rest service using the stats link in the player details object', function() {
                var team          = { batters: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter', _links: {player: {rel: 'player', href: 'playerUrl'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success({});
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                Utils.loadPlayer(player, team, 5, true);

                expect($.ajax).toHaveBeenCalledWith('playerUrl/stats?season=4&phase=1', jasmine.any(Object));
            });

            it('should decorate the batter with stats', function() {
                var team          = { batters: [] };
                var player        = { isCut: false, _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter', _links: {player: {rel: 'player', href: 'playerUrl'}} };
                var batterStats   = { innings: 100, outs: 0 };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success(batterStats);
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                spyOn(Utils, 'decorateBatter').and.callThrough();

                Utils.loadPlayer(player, team, 5, true);

                expect(Utils.decorateBatter).toHaveBeenCalledWith(playerDetails, batterStats, player.isCut);
            });

            it('should resolve the deferred object on successful player without stats load', function() {
                var team    = { batters: [] };
                var player  = { _links: {self: {rel: 'self', href: 'url'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success({player_type: 'Batter'});
                });

                var deferred = Utils.loadPlayer(player, team, 5, false);

                expect(deferred.state()).toEqual("resolved");
            });

            it('should reject the deferred object on unsuccessful player without stats load', function() {
                var team    = { batters: [] };
                var player  = { _links: {self: {rel: 'self', href: 'url'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.error();
                });

                var deferred = Utils.loadPlayer(player, team, 5, false);

                expect(deferred.state()).toEqual("rejected");
            });

            it('should resolve the deferred object on successful player with stats load', function() {
                var team          = { batters: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter', _links: {player: {rel: 'player', href: 'playerUrl'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.success({});
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                var deferred = Utils.loadPlayer(player, team, 5, true);

                expect(deferred.state()).toEqual("resolved");
            });

            it('should reject the deferred object on unsuccessful player with stats load', function() {
                var team          = { batters: [] };
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter', _links: {player: {rel: 'player', href: 'playerUrl'}} };

                spyOn($, 'ajax').and.callFake(function (req, options) {

                    if (req.match(/stats/)) {
                        options.error();
                    }
                    else {
                        options.success(playerDetails);
                    }
                });

                var deferred = Utils.loadPlayer(player, team, 5, true);

                expect(deferred.state()).toEqual("rejected");
            });
        });
    });
});
