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

            it('should add the cut and selected flags', function() {
                var player = {};

                Utils.decoratePitcher(player, null);

                expect(player.isCut     ).toBe(false);
                expect(player.isSelected).toBe(false);
            });

            it('should calculate the player rating', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };

                Utils.decoratePitcher(player, null);

                expect(player.rating).toEqual(17);
            });

            it('should calculate the pitcher record given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { wins: 21, losses: 13 };

                Utils.decoratePitcher(player, stats);

                expect(player.record).toEqual('21 - 13');
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

            it('should calculate the pitcher efficiency given stats', function() {
                var player = { speed: 5, control: 4, fatigue: 8 };
                var stats  = { innings: 72, hits: 64, strike_outs: 85 };

                Utils.decoratePitcher(player, stats);

                expect(player.eff).toEqual('+29');
            });
        });

        describe('decorateBatter', function() {

            it('should add cut and selected flags', function() {
                var player = {};

                Utils.decorateBatter(player, null);

                expect(player.isCut     ).toBe(false);
                expect(player.isSelected).toBe(false);
            });

            it('should calculate the player rating', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };

                Utils.decorateBatter(player, null);

                expect(player.rating).toEqual(18);
            });

            it('should copy games, home_runs and steals stats to the player given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { games: 149, home_runs: 48, steals: 27 };

                Utils.decorateBatter(player, stats);

                expect(player.games).toEqual(149);
                expect(player.homers).toEqual(48);
                expect(player.steals).toEqual(27);
            });

            it('should calculate the player batting average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { hits: 100, at_bats: 400 };

                Utils.decorateBatter(player, stats);

                expect(player.avg).toEqual('.250');
            });

            it('should calculate the player slugging average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { hits: 100, at_bats: 400, doubles: 15, triples: 4, home_runs: 12 };

                Utils.decorateBatter(player, stats);

                expect(player.slugging).toEqual('.475');
            });

            it('should calculate the player on base average given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { hits: 100, at_bats: 400, walks: 30 };

                Utils.decorateBatter(player, stats);

                expect(player.oba).toEqual('.325');
            });

            it('should calculate the player runs per game given stats', function() {
                var player = { power: 7, hit_n_run: 5, running: 6 };
                var stats  = { games: 148, home_runs: 35, runs: 64, runs_batted_in: 88 };

                Utils.decorateBatter(player, stats);

                expect(player.rpg).toEqual('0.79');
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
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Pitcher' };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success(playerDetails);
                });

                spyOn(Utils, 'decoratePitcher').and.callThrough();

                Utils.loadPlayer(player, team, 5, false);

                expect(Utils.decoratePitcher).toHaveBeenCalledWith(playerDetails, null);
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
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
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

                expect(Utils.decoratePitcher).toHaveBeenCalledWith(playerDetails, pitcherStats);
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
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
                var playerDetails = { player_type: 'Batter' };

                spyOn($, 'ajax').and.callFake(function (req, options) {
                    options.success(playerDetails);
                });

                spyOn(Utils, 'decorateBatter').and.callThrough();

                Utils.loadPlayer(player, team, 5, false);

                expect(Utils.decorateBatter).toHaveBeenCalledWith(playerDetails, null);
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
                var player        = { _links: {self: {rel: 'self', href: 'url'}} };
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

                expect(Utils.decorateBatter).toHaveBeenCalledWith(playerDetails, batterStats);
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
