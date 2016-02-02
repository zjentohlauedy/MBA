define(['objects/constants', 'objects/globals', 'utils', 'actions/rosterCutActions', 'ember'], function(Constants, Globals, Utils, Actions, Ember) {

    describe('RosterCutActions:', function() {

        describe('prepareData', function() {

            var controller;
            var deferred;

            beforeEach(function() {
                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);

                deferred = jasmine.createSpyObj('deferred', ['resolve','reject']);
            });

            it('should send a request to the teams resource', function() {

                Globals.season = 8;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/teams?season=8', jasmine.any(Object));
            });

            it('should set the teams on the controller', function() {

                var teams = [{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(teams);
                });

                spyOn(Utils, 'decorateTeams').and.callFake(function(teams) { return teams });

                Actions.prepareData(controller, deferred);

                expect(controller.set).toHaveBeenCalledWith('teams', teams);
            });

            it('should decorate the teams retrieved from the teams resource', function() {

                var teams = [{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(teams);
                });

                spyOn(Utils, 'decorateTeams').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(Utils.decorateTeams).toHaveBeenCalledWith(teams);
            });

            it('should resolve the given promise when the work is complete', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.resolve).toHaveBeenCalled();
            });

            it('should reject the promise if the rookie draft ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the ajax call failed', function() {

                spyOn(window, 'alert').and.callThrough();

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('selectTeam', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create();
            });

            it('should mark the given team as selected', function() {

                var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}], batters: []});

                Actions.selectTeam(controller, team);

                expect(team.isSelected).toBe(true);
            });

            it('should mark the current team as not selected', function() {

                var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}], batters: []});
                var currentTeam = controller.currentTeam;

                currentTeam.set('isSelected', true);

                Actions.selectTeam(controller, team);

                expect(currentTeam.isSelected).toBe(false);
            });

            it('should set the given team as the current team', function() {

                var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}], batters: []});

                Actions.selectTeam(controller, team);

                expect(controller.set).toHaveBeenCalledWith('currentTeam', team);
            });

            describe('when the given team has players', function() {

                it('should update pitchers and batters status', function() {

                    var team = Ember.Object.create().setProperties({isSelected: false, pitchers: [{}], batters: []});

                    Actions.selectTeam(controller, team);

                    expect(controller.send).toHaveBeenCalledWith('updatePitchersStatus');
                    expect(controller.send).toHaveBeenCalledWith('updateBattersStatus');
                });
            });

            describe('when the given team has no players', function() {

                it('should retrieve the team players for the current season from the backend', function() {

                    var team    = Ember.Object.create().setProperties({
                        team_id: 13, isSelected: false, pitchers: [], batters: [],
                        _links: {players: {href: "team-player-resource"}, team: {href: "team-resource"}}
                    });
                    var players = [{}, {}, {}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                    Actions.selectTeam(controller, team);

                    expect($.ajax).toHaveBeenCalledWith('team-player-resource', jasmine.any(Object));
                });

                it('should mark the team players for the current season as not being cut', function() {

                    var team    = Ember.Object.create().setProperties({
                        team_id: 13, isSelected: false, pitchers: [], batters: [],
                        _links: {players: {href: "team-player-resource"}, team: {href: "team-resource"}}
                    });
                    var players = [{}, {}, {}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        if (rel.match(/team-player-resource/)) { options.success(players); }
                        else                                   { options.success([]);      }
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                    Actions.selectTeam(controller, team);

                    expect(players[0].isCut).toBe(false);
                    expect(players[1].isCut).toBe(false);
                    expect(players[2].isCut).toBe(false);
                });

                it('should retrieve the team players for the previous season from the backend', function() {
                    var team    = Ember.Object.create().setProperties({
                        team_id: 13, isSelected: false, pitchers: [], batters: [],
                        _links: {players: {href: "team-player-resource"}, team: {href: "team-resource"}}
                    });
                    var players = [{}, {}, {}];
                    Globals.season = 5;

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                    Actions.selectTeam(controller, team);

                    expect($.ajax).toHaveBeenCalledWith('team-resource/players?season=4', jasmine.any(Object));
                });

                it('should mark the team players for the previous season as being cut', function() {
                    var team    = Ember.Object.create().setProperties({
                        team_id: 13, isSelected: false, pitchers: [], batters: [],
                        _links: {players: {href: "team-player-resource"}, team: {href: "team-resource"}}
                    });
                    var players = [{}, {}, {}];
                    Globals.season = 5;

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        if (rel.match(/team-resource/)) { options.success(players); }
                        else                            { options.success([]);      }
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                    Actions.selectTeam(controller, team);

                    expect(players[0].isCut).toBe(true);
                    expect(players[1].isCut).toBe(true);
                    expect(players[2].isCut).toBe(true);
                });

                it('should merge the players from current and previous seasons', function() {
                    var team    = Ember.Object.create().setProperties({
                        team_id: 13, isSelected: false, pitchers: [], batters: [],
                        _links: {players: {href: "team-player-resource"}, team: {href: "team-resource"}}
                    });
                    var plist1 = [{player_id: 1}];
                    var plist2 = [{player_id: 2}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        if      (rel.match(/team-player-resource/)) { options.success(plist1); }
                        else if (rel.match(/team-resource/)       ) { options.success(plist2); }
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function() {});
                    spyOn(Utils, 'mergePlayerLists').and.callThrough();

                    Actions.selectTeam(controller, team);

                    expect(Utils.mergePlayerLists).toHaveBeenCalledWith(plist1, plist2);
                });

                it('should add pitchers and batters to the given team', function() {

                    var team    = Ember.Object.create().setProperties({
                        pitchers: [], batters: [], _links: {players: {href: ""}, team: {href: ""}}
                    });
                    var players = [{player_id: 1},{player_id: 2},{player_id: 3}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {
                        if   (player.player_id === 1) { team.pitchers.push(player); }
                        else                          { team.batters. push(player); }
                    });

                    Actions.selectTeam(controller, team);

                    expect(team.pitchers.length).toBe(1);
                    expect(team.batters. length).toBe(2);
                });

                it('should call utils loadPlayer for each player given', function() {

                    var team = Ember.Object.create().setProperties({
                        pitchers: [], batters: [], _links: {players: {href: ""}, team: {href: ""}}
                    });
                    var players = [{},{},{},{},{}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                    Actions.selectTeam(controller, team);

                    expect(Utils.loadPlayer.calls.count()).toEqual(players.length);
                });

                it('should sort the batters by position', function() {
                    var team    = Ember.Object.create().setProperties({
                        pitchers: [], batters: [], _links: {players: {href: ""}, team: {href: ""}}
                    });
                    var players = [{},{},{},{},{}];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {
                        team.batters.push(player);
                    });

                    spyOn(Utils, 'sortBattersByPosition').and.callThrough();

                    Actions.selectTeam(controller, team);

                    expect(Utils.sortBattersByPosition).toHaveBeenCalledWith(team.batters);
                });

                it('should update pitchers, batters and team status', function() {

                    var team = Ember.Object.create().setProperties({
                        pitchers: [], batters: [], _links: {players: {href: ""}, team: {href: ""}}
                    });
                    var players = [];

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success(players);
                    });

                    Actions.selectTeam(controller, team);

                    expect(controller.send).toHaveBeenCalledWith('updatePitchersStatus');
                    expect(controller.send).toHaveBeenCalledWith('updateBattersStatus');
                    expect(controller.send).toHaveBeenCalledWith('updateTeamStatus');
                });

                it('should show an alert if the ajax call fails', function() {

                    var team    = Ember.Object.create().setProperties({
                        team_id: 13, isSelected: false, pitchers: [], _links: {players: {href: "team-player-resource"}, team: {href: ""}}
                    });

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.error();
                    });

                    spyOn(window, 'alert').and.callThrough();

                    Actions.selectTeam(controller, team);

                    expect(window.alert).toHaveBeenCalled();
                });
            });
        });

        describe('toggleCutPitcher', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create();
                controller.currentTeam._links = {team:{href:'http://localhost/mba/resources/teams/7'}};
            });

            describe('when pitcher has not been cut', function() {

                it('should send a delete request to the team player resource', function() {

                    var pitcher = {player_id: 123};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('DELETE');
                });

                it('should mark the pitcher as having been cut', function() {

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

                    var pitcher = {player_id: 123, isCut: true};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                });

                it('should mark the pitcher as not having been cut', function() {

                    var pitcher = Ember.Object.create({isCut: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success();
                    });

                    Actions.toggleCutPitcher(controller, pitcher);

                    expect(pitcher.isCut).toBe(false);
                });
            });

            it('should update the pitchers and team status', function() {

                var pitcher = Ember.Object.create({isCut: false});

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success();
                });

                Actions.toggleCutPitcher(controller, pitcher);

                expect(controller.send).toHaveBeenCalledWith('updatePitchersStatus');
                expect(controller.send).toHaveBeenCalledWith('updateTeamStatus');
            });

            it('should show an alert if the ajax call fails', function() {

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
                controller.currentTeam._links = {team:{href:'http://localhost/mba/resources/teams/7'}};
            });

            describe('when batter has not been cut', function() {

                it('should send a delete request to the team player resource', function() {

                    var batter = {player_id: 123};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutBatter(controller, batter);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('DELETE');
                });

                it('should mark the batter as having been cut', function() {

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

                    var batter = {player_id: 123, isCut: true};
                    Globals.season = 2;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.toggleCutBatter(controller, batter);

                    expect($.ajax.calls.mostRecent().args[0]     ).toEqual('http://localhost/mba/resources/teams/7/players/123/season/2');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                });

                it('should mark the batter as not having been cut', function() {

                    var batter = Ember.Object.create({isCut: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) {
                        options.success();
                    });

                    Actions.toggleCutBatter(controller, batter);

                    expect(batter.isCut).toBe(false);
                });
            });

            it('should update the batters and team status', function() {

                var batter = Ember.Object.create({isCut: false});

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success();
                });

                Actions.toggleCutBatter(controller, batter);

                expect(controller.send).toHaveBeenCalledWith('updateBattersStatus');
                expect(controller.send).toHaveBeenCalledWith('updateTeamStatus');
            });

            it('should show an alert if the ajax call fails', function() {

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

        describe('sortPitchers', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create({ pitchers: [
                    {player_id: 1, name: 'Joe', field1: 5, field2: 1, averageField: '12.62', isCut: false},
                    {player_id: 2, name: 'Bob', field1: 4, field2: 1, averageField: '2.27',  isCut: false},
                    {player_id: 3, name: 'Ian', field1: 6, field2: 1, averageField: '4.33',  isCut: false}
                ] });
            });

            it('should sort the pitchers by the given field', function() {

                Actions.sortPitchers(controller, 'field1');

                expect(controller.currentTeam.pitchers[0].player_id).toBe(2);
                expect(controller.currentTeam.pitchers[1].player_id).toBe(1);
                expect(controller.currentTeam.pitchers[2].player_id).toBe(3);
            });

            it('should set the given field as the current pitcher sort field on the current team', function() {

                spyOn(controller.currentTeam, 'set').and.callThrough();

                Actions.sortPitchers(controller, 'field2');

                expect(controller.currentTeam.set).toHaveBeenCalledWith('currentPitcherSortField', 'field2');
            });

            it('should reverse the order if the given field is the same as the current sort field', function() {

                controller.currentTeam.currentPitcherSortField = 'field1';

                Actions.sortPitchers(controller, 'field1');

                expect(controller.currentTeam.pitchers[0].player_id).toBe(3);
                expect(controller.currentTeam.pitchers[1].player_id).toBe(2);
                expect(controller.currentTeam.pitchers[2].player_id).toBe(1);
            });

            it('should sort numerical strings as if they were numbers', function() {

                Actions.sortPitchers(controller, 'averageField');

                expect(controller.currentTeam.pitchers[0].player_id).toBe(2);
                expect(controller.currentTeam.pitchers[1].player_id).toBe(3);
                expect(controller.currentTeam.pitchers[2].player_id).toBe(1);
            });

            it('should sort names as alphanumeric strings', function() {

                Actions.sortPitchers(controller, 'name');

                expect(controller.currentTeam.pitchers[0].player_id).toBe(2);
                expect(controller.currentTeam.pitchers[1].player_id).toBe(3);
                expect(controller.currentTeam.pitchers[2].player_id).toBe(1);
            });

            it('should sort cut players to the bottom', function() {

                controller.currentTeam.pitchers.addObject({player_id: 4, name: 'Aaa', field1: 5, field2: 1, averageField: '12.62', isCut: true});

                Actions.sortPitchers(controller, 'name');

                expect(controller.currentTeam.pitchers[0].player_id).toBe(2);
                expect(controller.currentTeam.pitchers[1].player_id).toBe(3);
                expect(controller.currentTeam.pitchers[2].player_id).toBe(1);
                expect(controller.currentTeam.pitchers[3].player_id).toBe(4);
            });

            it('should leave cut players at the bottom when reversing', function() {

                controller.currentTeam.pitchers.addObject({player_id: 4, name: 'Aaa', field1: 5, field2: 1, averageField: '12.62', isCut: true});
                controller.currentTeam.currentPitcherSortField = 'field1';

                Actions.sortPitchers(controller, 'field1');

                expect(controller.currentTeam.pitchers[0].player_id).toBe(3);
                expect(controller.currentTeam.pitchers[1].player_id).toBe(2);
                expect(controller.currentTeam.pitchers[2].player_id).toBe(1);
                expect(controller.currentTeam.pitchers[3].player_id).toBe(4);
            });
        });

        describe('sortBatters', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.currentTeam = Ember.Object.create({ batters: [
                    {player_id: 1, name: 'Joe', primary_position: 'CF', field1: 5, field2: 1, averageField: '12.62'},
                    {player_id: 2, name: 'Bob', primary_position: '1B', field1: 4, field2: 1, averageField: '2.27'},
                    {player_id: 3, name: 'Ian', primary_position: 'C',  field1: 6, field2: 1, averageField: '4.33'}
                ] });
            });

            it('should sort the batters by the given field', function() {

                Actions.sortBatters(controller, 'field1');

                expect(controller.currentTeam.batters[0].player_id).toBe(2);
                expect(controller.currentTeam.batters[1].player_id).toBe(1);
                expect(controller.currentTeam.batters[2].player_id).toBe(3);
            });

            it('should set the given field as the current batter sort field', function() {

                spyOn(controller.currentTeam, 'set').and.callThrough();

                Actions.sortBatters(controller, 'field2');

                expect(controller.currentTeam.set).toHaveBeenCalledWith('currentBatterSortField', 'field2');
            });

            it('should reverse the order if the given field is the same as the current sort field', function() {

                controller.currentTeam.currentBatterSortField = 'field1';

                Actions.sortBatters(controller, 'field1');

                expect(controller.currentTeam.batters[0].player_id).toBe(3);
                expect(controller.currentTeam.batters[1].player_id).toBe(2);
                expect(controller.currentTeam.batters[2].player_id).toBe(1);
            });

            it('should sort numerical strings as if they were numbers', function() {

                Actions.sortBatters(controller, 'averageField');

                expect(controller.currentTeam.batters[0].player_id).toBe(2);
                expect(controller.currentTeam.batters[1].player_id).toBe(3);
                expect(controller.currentTeam.batters[2].player_id).toBe(1);
            });

            it('should sort names as alphanumeric strings', function() {

                Actions.sortBatters(controller, 'name');

                expect(controller.currentTeam.batters[0].player_id).toBe(2);
                expect(controller.currentTeam.batters[1].player_id).toBe(3);
                expect(controller.currentTeam.batters[2].player_id).toBe(1);
            });

            it('should sort positions in the correct order', function() {

                Actions.sortBatters(controller, 'primary_position');

                expect(controller.currentTeam.batters[0].player_id).toBe(3);
                expect(controller.currentTeam.batters[1].player_id).toBe(2);
                expect(controller.currentTeam.batters[2].player_id).toBe(1);
            });

            it('should sort cut players to the bottom', function() {

                controller.currentTeam.batters.addObject({player_id: 4, name: 'Aaa', field1: 5, field2: 1, averageField: '12.62', isCut: true});

                Actions.sortBatters(controller, 'name');

                expect(controller.currentTeam.batters[0].player_id).toBe(2);
                expect(controller.currentTeam.batters[1].player_id).toBe(3);
                expect(controller.currentTeam.batters[2].player_id).toBe(1);
                expect(controller.currentTeam.batters[3].player_id).toBe(4);
            });

            it('should leave cut players at the bottom when reversing', function() {

                controller.currentTeam.batters.addObject({player_id: 4, name: 'Aaa', field1: 5, field2: 1, averageField: '12.62', isCut: true});
                controller.currentTeam.currentBatterSortField = 'field1';

                Actions.sortBatters(controller, 'field1');

                expect(controller.currentTeam.batters[0].player_id).toBe(3);
                expect(controller.currentTeam.batters[1].player_id).toBe(2);
                expect(controller.currentTeam.batters[2].player_id).toBe(1);
                expect(controller.currentTeam.batters[3].player_id).toBe(4);
            });
        });

        describe('finishStage', function() {

            var controller;
            var progressController;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                progressController = jasmine.createSpyObj('progressController', ['send', 'get', 'set']);

                controller.get = function(name) {
                    if (name == 'controllers.progress') {
                        return progressController;
                    }

                    return null;
                }
            });

            it('should direct the progress controller to move to the next stage when the rookie draft controller is finished preparing data', function() {

                var fakeDeferred = {promise: function(){ return{then: function(callback){ callback(); }}; }};
                controller.stageComplete = true;

                spyOn($, 'Deferred').and.callFake(function() { return fakeDeferred; });

                Actions.finishStage(controller);

                expect(progressController.send).toHaveBeenCalledWith('nextStage');
            });

            it('should do nothing if the stage is not complete', function() {

                controller.stageComplete = false;

                spyOn(controller, 'get').and.callThrough();

                Actions.finishStage(controller);

                expect(controller.get).not.toHaveBeenCalled();
            });
        });
    });
});
