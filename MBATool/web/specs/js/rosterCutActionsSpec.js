define(['objects/constants', 'objects/globals', 'utils', 'actions/rosterCutActions', 'ember'], function(Constants, Globals, Utils, Actions, Ember) {

    describe('RosterCutActions:', function() {

        describe('selectTeam', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create();
            });

            it('should mark the given team as selected', function() {

                var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}]});

                Actions.selectTeam(controller, team);

                expect(team.isSelected).toBe(true);
            });

            it('should mark the current team as not selected', function() {

                var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}]});
                var currentTeam = controller.currentTeam;

                currentTeam.set('isSelected', true);

                Actions.selectTeam(controller, team);

                expect(currentTeam.isSelected).toBe(false);
            });

            it('should set the given team as the current team', function() {

                var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}]});

                Actions.selectTeam(controller, team);

                expect(controller.set).toHaveBeenCalledWith('currentTeam', team);
            });

            describe('when the given team has no players', function() {

                it('should retrieve the players from the backend', function() {

                    var team    = Ember.Object.create().setProperties({team_id: 13, isSelected: false, pitchers: []});
                    var players = [{}, {}, {}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    Globals.season = 7;

                    Actions.selectTeam(controller, team);

                    expect($.ajax).toHaveBeenCalledWith('/mba/resources/teams/13/players?season=7&phase=1', jasmine.any(Object));
                    expect(controller.send).toHaveBeenCalledWith('loadPlayers', team, players);
                });

                it('should show an alert if the ajax call fails', function() {

                    var team = Ember.Object.create().setProperties({isSelected: false, pitchers: []});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.error();
                    });

                    spyOn(window, 'alert').and.callThrough();

                    Actions.selectTeam(controller, team);

                    expect(window.alert).toHaveBeenCalled();
                });
            });

            describe('when the given team has players', function() {

                it('should update pitchers and batters status', function() {

                    var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}]});

                    Actions.selectTeam(controller, team);

                    expect(controller.send).toHaveBeenCalledWith('updatePitchersStatus');
                    expect(controller.send).toHaveBeenCalledWith('updateBattersStatus');
                });
            });
        });

        describe('loadPlayers', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create();
            });

            it('should add pitchers and batters to the given team', function() {

                var team    = {pitchers: [], batters: []};
                var players = [{player_id: 1},{player_id: 2},{player_id: 3}];

                spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {

                    if (player.player_id === 1) {
                        team.pitchers.push(player);
                    } else {
                        team.batters.push(player);
                    }
                });

                Actions.loadPlayers(controller, team, players);

                expect(team.pitchers.length).toBe(1);
                expect(team.batters. length).toBe(2);
            });

            it('should call utils loadPlayer for each player given', function() {

                var team = {};
                var players = [{},{},{},{},{}];

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                Actions.loadPlayers(controller, team, players);

                expect(Utils.loadPlayer.calls.count()).toEqual(players.length);
            });

            it('should update pitchers, batters and team status', function() {

                var team    = {};
                var players = [];

                Actions.loadPlayers(controller, team, players);

                expect(controller.send).toHaveBeenCalledWith('updatePitchersStatus');
                expect(controller.send).toHaveBeenCalledWith('updateBattersStatus');
                expect(controller.send).toHaveBeenCalledWith('updateTeamStatus');
            });
        });

        describe('toggleCutPitcher', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create();
            });

            describe('when pitcher has not been cut', function() {

                it('should send a delete request to the team player resource', function() {

                    controller.currentTeam.links = [{rel:'players', href:'http://localhost/mba/resources/teams/7/players'}];
                    var pitcher = {player_id: 123};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('DELETE');
                });

                it('should mark the pitcher as having been cut', function() {

                    controller.currentTeam.links = [];
                    var pitcher = Ember.Object.create({isCut: false});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success();
                    });

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect(pitcher.isCut).toBe(true);
                });
            });

            describe('when pitcher has been cut', function() {

                it('should send a post request to the team player resource', function() {

                    controller.currentTeam.links = [{rel:'players', href:'http://localhost/mba/resources/teams/7/players'}];
                    var pitcher = {player_id: 123, isCut: true};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                });

                it('should mark the pitcher as not having been cut', function() {

                    controller.currentTeam.links = [];
                    var pitcher = Ember.Object.create({isCut: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success();
                    });

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect(pitcher.isCut).toBe(false);
                });
            });

            it('should update the pitchers and team status', function() {

                controller.currentTeam.links = [];
                var pitcher = Ember.Object.create({isCut: false});

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success();
                });

                Actions.toggleCutPitcher(controller, pitcher);

                expect(controller.send).toHaveBeenCalledWith('updatePitchersStatus');
                expect(controller.send).toHaveBeenCalledWith('updateTeamStatus');
            });

            it('should show an alert if the ajax call fails', function() {

                controller.currentTeam.links = [];
                var pitcher = {};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.toggleCutPitcher(controller, pitcher);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('toggleCutBatter', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create();
            });

            describe('when batter has not been cut', function() {

                it('should send a delete request to the team player resource', function() {

                    controller.currentTeam.links = [{rel:'players', href:'http://localhost/mba/resources/teams/7/players'}];
                    var batter = {player_id: 123};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutBatter(controller, batter);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('DELETE');
                });

                it('should mark the batter as having been cut', function() {

                    controller.currentTeam.links = [];
                    var batter = Ember.Object.create({isCut: false});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success();
                    });

                    Actions.toggleCutBatter(controller, batter);

                    expect(batter.isCut).toBe(true);
                });
            });

            describe('when batter has been cut', function() {

                it('should send a post request to the team player resource', function() {

                    controller.currentTeam.links = [{rel:'players', href:'http://localhost/mba/resources/teams/7/players'}];
                    var batter = {player_id: 123, isCut: true};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutBatter(controller, batter);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                });

                it('should mark the batter as not having been cut', function() {

                    controller.currentTeam.links = [];
                    var batter = Ember.Object.create({isCut: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success();
                    });

                    Actions.toggleCutBatter(controller, batter);

                    expect(batter.isCut).toBe(false);
                });
            });

            it('should update the batters and team status', function() {

                controller.currentTeam.links = [];
                var batter = Ember.Object.create({isCut: false});

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success();
                });

                Actions.toggleCutBatter(controller, batter);

                expect(controller.send).toHaveBeenCalledWith('updateBattersStatus');
                expect(controller.send).toHaveBeenCalledWith('updateTeamStatus');
            });

            it('should show an alert if the ajax call fails', function() {

                controller.currentTeam.links = [];
                var batter = {};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.toggleCutBatter(controller, batter);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('updatePitchersStatus', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = { pitchers: [] };

                for (var i = 0; i < Constants.TEAM_MAX_PITCHERS; i++) {
                    controller.currentTeam.pitchers.push({isCut: true});
                }
            });

            it('should set both isComplete and isError to false when not enough pitchers are cut', function() {

                for (var i = 0; i < Constants.ROSTER_CUT_TARGET_PITCHERS + 1; i++) {
                    controller.currentTeam.pitchers[i].isCut = false;
                }

                Actions.updatePitchersStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('pitchers.isComplete', false);
                expect(controller.set).toHaveBeenCalledWith('pitchers.isError', false);
            });

            it('should set isComplete to false and isError to true when too many pitchers are cut', function() {

                for (var i = 0; i < Constants.ROSTER_CUT_TARGET_PITCHERS - 1; i++) {
                    controller.currentTeam.pitchers[i].isCut = false;
                }

                Actions.updatePitchersStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('pitchers.isComplete', false);
                expect(controller.set).toHaveBeenCalledWith('pitchers.isError', true);
            });

            it('should set isCompleted to true and isError to false when the right amount of pitchers are cut', function() {

                for (var i = 0; i < Constants.ROSTER_CUT_TARGET_PITCHERS; i++) {
                    controller.currentTeam.pitchers[i].isCut = false;
                }

                Actions.updatePitchersStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('pitchers.isComplete', true);
                expect(controller.set).toHaveBeenCalledWith('pitchers.isError', false);
            });
        });

        describe('updateBattersStatus', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = { batters: [] };

                for (var i = 0; i < Constants.TEAM_MAX_BATTERS; i++) {
                    controller.currentTeam.batters.push({isCut: true});
                }
            });

            it('should set both isComplete and isError to false when not enough batters are cut', function() {

                for (var i = 0; i < Constants.ROSTER_CUT_TARGET_BATTERS + 1; i++) {
                    controller.currentTeam.batters[i].isCut = false;
                }

                Actions.updateBattersStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('batters.isComplete', false);
                expect(controller.set).toHaveBeenCalledWith('batters.isError', false);
            });

            it('should set isComplete to false and isError to true when too many batters are cut', function() {

                for (var i = 0; i < Constants.ROSTER_CUT_TARGET_BATTERS - 1; i++) {
                    controller.currentTeam.batters[i].isCut = false;
                }

                Actions.updateBattersStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('batters.isComplete', false);
                expect(controller.set).toHaveBeenCalledWith('batters.isError', true);
            });

            it('should set isCompleted to true and isError to false when the right amount of batters are cut', function() {

                for (var i = 0; i < Constants.ROSTER_CUT_TARGET_BATTERS; i++) {
                    controller.currentTeam.batters[i].isCut = false;
                }

                Actions.updateBattersStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('batters.isComplete', true);
                expect(controller.set).toHaveBeenCalledWith('batters.isError', false);
            });
        });

        describe('updateTeamStatus', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = jasmine.createSpyObj('controller.currentTeam', ['set']);
                controller.pitchers = {};
                controller.batters = {};
            });

            it('should set current team to complete if both pitchers and batters are complete', function() {

                controller.pitchers.isComplete = true;
                controller.batters.isComplete = true;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isComplete', true);
            });

            it('should set current team to not complete if both pitchers is complete and batters is not complete', function() {

                controller.pitchers.isComplete = true;
                controller.batters.isComplete = false;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isComplete', false);
            });

            it('should set current team to not complete if both pitchers is not complete and batters is complete', function() {

                controller.pitchers.isComplete = false;
                controller.batters.isComplete = true;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isComplete', false);
            });

            it('should set current team to not complete if neither pitchers nor batters are complete', function() {

                controller.pitchers.isComplete = false;
                controller.batters.isComplete = false;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isComplete', false);
            });

            it('should set current team to error if both pitchers and batters are error', function() {

                controller.pitchers.isError = true;
                controller.batters.isError = true;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isError', true);
            });

            it('should set current team to error if both pitchers is error and batters is not error', function() {

                controller.pitchers.isError = true;
                controller.batters.isError = false;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isError', true);
            });

            it('should set current team to error if both pitchers is not error and batters is error', function() {

                controller.pitchers.isError = false;
                controller.batters.isError = true;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isError', true);
            });

            it('should set current team to not error if neither pitchers nor batters are error', function() {

                controller.pitchers.isError = false;
                controller.batters.isError = false;

                Actions.updateTeamStatus(controller);

                expect(controller.currentTeam.set).toHaveBeenCalledWith('isError', false);
            });
        });

        describe('updateStatus', function() {


            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            });

            it('should mark stage as complete if all teams are complete', function() {

                controller.teams = [{isComplete: true},{isComplete: true},{isComplete: true},{isComplete: true},{isComplete: true}];

                Actions.updateStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('stageComplete', true);
            });

            it('should mark stage as not complete if any teams are not complete', function() {

                controller.teams = [{isComplete: true},{isComplete: false},{isComplete: true},{isComplete: true},{isComplete: true}];

                Actions.updateStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('stageComplete', false);
            });
        });

        describe('loadRookies', function() {

            var controller;
            var progressController;
            var rookieDraftController;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.rookies = Ember.Object.create({pitchers: [], batters: []});

                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);
                rookieDraftController = jasmine.createSpyObj('rookieDraftController', ['send', 'get', 'set']);

                controller.get = function(name) {
                    if (name == 'controllers.progress') {
                        return progressController;
                    }
                    else if (name == 'controllers.rookie-draft') {
                        return rookieDraftController;
                    }

                    return null;
                }
            });

            it('should add pitchers and batters to rookies', function() {

                var rookies = [{player_id: 1},{player_id: 2},{player_id: 3}];

                spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {

                    if (player.player_id === 1) {
                        team.pitchers.push(player);
                    } else {
                        team.batters.push(player);
                    }
                });

                Actions.loadRookies(controller, rookies);

                expect(controller.rookies.pitchers.length).toBe(1);
                expect(controller.rookies.batters. length).toBe(2);
            });

            it('should call utils loadPlayer for each player given', function() {

                var rookies = [{},{},{},{},{}];

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                Actions.loadRookies(controller, rookies);

                expect(Utils.loadPlayer.calls.count()).toEqual(rookies.length);
            });

            it('should sort rookie pitchers and batters by rating, descending', function() {

                var rookies = [{player_id: 1},{player_id: 2},{player_id: 3},{player_id: 4}];

                spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {
                    switch (player.player_id) {
                    case 1: team.pitchers.push({player_id: 1, rating:  7}); break;
                    case 2: team.pitchers.push({player_id: 2, rating: 10}); break;

                    case 3: team.batters.push({player_id: 3, rating:  5}); break;
                    case 4: team.batters.push({player_id: 4, rating: 12}); break;
                    }
                });

                Actions.loadRookies(controller, rookies);

                expect(controller.rookies.pitchers[0].player_id).toBe(2);
                expect(controller.rookies.pitchers[1].player_id).toBe(1);

                expect(controller.rookies.batters[0].player_id).toBe(4);
                expect(controller.rookies.batters[1].player_id).toBe(3);
            });

            it('should set the rookies on the rookie draft controller', function() {

                var rookies = [];

                Actions.loadRookies(controller, rookies);

                expect(rookieDraftController.set).toHaveBeenCalledWith('rookies', controller.rookies);
            });

            it('should direct the rookie draft controller to show the first team', function() {

                var rookies = [];

                Actions.loadRookies(controller, rookies);

                expect(rookieDraftController.send).toHaveBeenCalledWith('showFirstTeam');
            });

            it('should direct the progress controller to move to the next stage', function() {

                var rookies = [];

                Actions.loadRookies(controller, rookies);

                expect(progressController.send).toHaveBeenCalledWith('nextStage');
            });
        });

        describe('finishStage', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.rookies = Ember.Object.create();
            });

            it('should request the rookie draft from the drafts resource', function() {

                Globals.season = 6;
                controller.stageComplete = true;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.finishStage(controller);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/drafts/rookie/season/5', jasmine.any(Object));
            });

            it('should set the rookie draft on the rookies object', function() {

                var draft = [1,2,3,4,5];

                controller.stageComplete = true;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(draft);
                });

                spyOn(controller.rookies, 'set').and.callThrough();

                Actions.finishStage(controller);

                expect(controller.rookies.set).toHaveBeenCalledWith('draftOrder', draft);
            });

            it('should retrieve the rookies for the current season from the players resource', function() {

                Globals.season = 6;
                controller.stageComplete = true;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == '/mba/resources/drafts/rookie/season/5') {
                        options.success([]);
                    }
                });

                Actions.finishStage(controller);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/players?rookie=true&season=6', jasmine.any(Object));
            });

            it('should call the load rookies controller method to finish loading rookies', function() {

                Globals.season = 6;
                controller.stageComplete = true;

                var rookies = [{},{},{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == '/mba/resources/drafts/rookie/season/5') {
                        options.success([]);
                    }
                    else if (rel = '/mba/resources/players?rookie=true&season=6') {
                        options.success(rookies);
                    }
                });

                Actions.finishStage(controller);

                expect(controller.send).toHaveBeenCalledWith('loadRookies', rookies);
            });

            it('should do nothing if the stage is not complete', function() {

                controller.stageComplete = false;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.finishStage(controller);

                expect($.ajax.calls.count()).toBe(0);
            });

            it('should show an alert if the rookie draft ajax call fails', function() {

                controller.stageComplete = true;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.finishStage(controller);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should show an alert if the rookie players ajax call fails', function() {

                controller.stageComplete = true;

                spyOn($, 'ajax').and.callFake(function(rel, options) {

                    if (rel.match(/draft/)) {
                        options.success([]);
                    } else {
                        options.error();
                    }
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.finishStage(controller);

                expect(window.alert).toHaveBeenCalled();
            });
        });
    });
});
