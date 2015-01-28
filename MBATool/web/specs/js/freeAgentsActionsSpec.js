define(['objects/constants', 'objects/globals', 'utils', 'actions/freeAgentsActions'], function(Constants, Globals, Utils, Actions) {

    describe('FreeAgentsActions', function() {

        describe('prepareData', function() {

            var controller;
            var deferred;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({pitchers: [], batters: []});

                deferred = jasmine.createSpyObj('deferred', ['resolve','reject']);
            });

            it('should request the free agent draft for the previous season from the drafts resource', function() {

                Globals.season = 6;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/drafts/free-agent/season/5', jasmine.any(Object));
            });

            it('should set the free agent draft on the free agents object', function() {

                var draft = [1,2,3,4,5];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(draft);
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                spyOn(controller.freeAgents, 'set').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(controller.freeAgents.set).toHaveBeenCalledWith('draftOrder', draft);
            });

            it('should retrieve the free agents for the current season from the players resource', function() {

                Globals.season = 6;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == '/mba/resources/drafts/free-agent/season/5') {
                        options.success([]);
                    }
                });

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/players?freeagent=true&season=6', jasmine.any(Object));
            });

            it('should add pitchers and batters to free agents', function() {

                var freeAgents = [{player_id: 1},{player_id: 2},{player_id: 3}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == '/mba/resources/drafts/free-agent/season/5') { options.success([]); }
                    else                                                    { options.success(freeAgents); }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {
                    if (player.player_id === 1) { team.pitchers.push(player); }
                    else                        { team.batters. push(player); }
                });

                Actions.prepareData(controller, deferred);

                expect(controller.freeAgents.pitchers.length).toBe(1);
                expect(controller.freeAgents.batters. length).toBe(2);
            });

            it('should call utils loadPlayer for each player given', function() {

                var freeAgents = [{},{},{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == '/mba/resources/drafts/free-agent/season/5') { options.success([]); }
                    else                                                    { options.success(freeAgents); }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(Utils.loadPlayer.calls.count()).toEqual(freeAgents.length);
            });

            it('should show the first team', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                Actions.prepareData(controller, deferred);

                expect(controller.send).toHaveBeenCalledWith('showFirstTeam');
            });

            it('should resolve the given promise when the work is complete', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.resolve).toHaveBeenCalled();
            });

            it('should reject the promise if the free agent draft ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the free agent draft ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should reject the promise if the free agent players ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/draft/)) { options.success([]); }
                    else                    { options.error();     }
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the free agent players ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/draft/)) { options.success([]); }
                    else                    { options.error();     }
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('toggleFreeAgentTable', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({
                    pitchers: [],
                    batters:  []
                });
            });

            describe('when showing free agent pitchers', function() {

                beforeEach(function() {
                    controller.showFreeAgentPitchers = true;
                    controller.showFreeAgentBatters = false;
                });

                it('should mark free agent pitchers as not being shown', function() {

                    Actions.toggleFreeAgentTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showFreeAgentPitchers', false);
                });

                it('should mark free agent batters as being shown', function() {

                    Actions.toggleFreeAgentTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showFreeAgentBatters', true);
                });

                it('should change the text on the toggle free agents button to "show pitchers"', function() {

                    Actions.toggleFreeAgentTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('toggleFreeAgentButtonLabel', 'Show Pitchers');
                });

                it('should deselect all free agent pitchers', function() {

                    for (var i = 0; i < 10; i++) {
                        controller.freeAgents.pitchers.push({isSelected: true});
                    }

                    Actions.toggleFreeAgentTable(controller);

                    for (var i = 0; i < 10; i++) {
                        expect(controller.freeAgents.pitchers[i].isSelected).toBe(false);
                    }
                });
            });

            describe('when showing free agent batters', function() {

                beforeEach(function() {
                    controller.showFreeAgentPitchers = false;
                    controller.showFreeAgentBatters = true;
                });

                it('should mark free agent batters as not being shown', function() {

                    Actions.toggleFreeAgentTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showFreeAgentBatters', false);
                });

                it('should mark free agent pitchers as being shown', function() {

                    Actions.toggleFreeAgentTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showFreeAgentPitchers', true);
                });

                it('should change the text on the toggle free agents button to "show batters"', function() {

                    Actions.toggleFreeAgentTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('toggleFreeAgentButtonLabel', 'Show Batters');
                });

                it('should deselect all free agent batters', function() {

                    for (var i = 0; i < 10; i++) {
                        controller.freeAgents.batters.push({isSelected: true});
                    }

                    Actions.toggleFreeAgentTable(controller);

                    for (var i = 0; i < 10; i++) {
                        expect(controller.freeAgents.batters[i].isSelected).toBe(false);
                    }
                });
            });
        });

        describe('sortPitchers', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({ pitchers: [
                    {player_id: 1, field1: 5, field2: 1, rating: 9},
                    {player_id: 2, field1: 4, field2: 1, rating: 7},
                    {player_id: 3, field1: 6, field2: 1, rating: 8}
                ] });
            });

            it('should sort the pitchers by the given field', function() {

                controller.currentPitcherSortField = null;

                Actions.sortPitchers(controller, 'field1');

                expect(controller.freeAgents.pitchers[0].player_id).toBe(2);
                expect(controller.freeAgents.pitchers[1].player_id).toBe(1);
                expect(controller.freeAgents.pitchers[2].player_id).toBe(3);
            });

            it('should set the given field as the current pitcher sort field', function() {

                controller.currentPitcherSortField = null;

                Actions.sortPitchers(controller, 'field2');

                expect(controller.set).toHaveBeenCalledWith('currentPitcherSortField', 'field2');
            });

            it('should reverse the order if the given field is the same as the current sort field', function() {

                controller.currentPitcherSortField = 'field1';

                Actions.sortPitchers(controller, 'field1');

                expect(controller.freeAgents.pitchers[0].player_id).toBe(3);
                expect(controller.freeAgents.pitchers[1].player_id).toBe(2);
                expect(controller.freeAgents.pitchers[2].player_id).toBe(1);
            });
        });

        describe('sortBatters', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({ batters: [
                    {player_id: 1, field1: 5, field2: 1, rating: 9},
                    {player_id: 2, field1: 4, field2: 1, rating: 7},
                    {player_id: 3, field1: 6, field2: 1, rating: 8}
                ] });
            });

            it('should sort the batters by the given field', function() {

                controller.currentBatterSortField = null;

                Actions.sortBatters(controller, 'field1');

                expect(controller.freeAgents.batters[0].player_id).toBe(2);
                expect(controller.freeAgents.batters[1].player_id).toBe(1);
                expect(controller.freeAgents.batters[2].player_id).toBe(3);
            });

            it('should set the given field as the current batter sort field', function() {

                controller.currentBatterSortField = null;

                Actions.sortBatters(controller, 'field2');

                expect(controller.set).toHaveBeenCalledWith('currentBatterSortField', 'field2');
            });

            it('should reverse the order if the given field is the same as the current sort field', function() {

                controller.currentBatterSortField = 'field1';

                Actions.sortBatters(controller, 'field1');

                expect(controller.freeAgents.batters[0].player_id).toBe(3);
                expect(controller.freeAgents.batters[1].player_id).toBe(2);
                expect(controller.freeAgents.batters[2].player_id).toBe(1);
            });
        });

        describe('selectFreeAgentPitcher', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({
                    pitchers: []
                });
            });

            it('should mark the given pitcher as selected', function() {

                var pitcher = Ember.Object.create({isSelected: false});

                controller.freeAgents.pitchers.push(pitcher);

                Actions.selectFreeAgentPitcher(controller, pitcher);

                expect(pitcher.isSelected).toBe(true);
            });

            it('should mark all other pitchers as not selected', function() {

                var pitcher = Ember.Object.create({isSelected: false});

                for (var i = 0; i < 10; i++) {
                    controller.freeAgents.pitchers.push({isSelected: true});
                }

                Actions.selectFreeAgentPitcher(controller, pitcher);

                for (var i = 0; i < 10; i++) {
                    expect(controller.freeAgents.pitchers[i].isSelected).toBe(false);
                }
            });

            it('should mark the pitcher as not selected if the pitcher is already selected', function() {

                var pitcher = Ember.Object.create({isSelected: true});

                controller.freeAgents.pitchers.push(pitcher);

                Actions.selectFreeAgentPitcher(controller, pitcher);

                expect(pitcher.isSelected).toBe(false);
            });
        });

        describe('selectFreeAgentBatter', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({
                    batters: []
                });
            });

            it('should mark the given batter as selected', function() {

                var batter = Ember.Object.create({isSelected: false});

                controller.freeAgents.batters.push(batter);

                Actions.selectFreeAgentBatter(controller, batter);

                expect(batter.isSelected).toBe(true);
            });

            it('should mark all other batters as not selected', function() {

                var batter = Ember.Object.create({isSelected: false});

                for (var i = 0; i < 10; i++) {
                    controller.freeAgents.batters.push({isSelected: true});
                }

                Actions.selectFreeAgentBatter(controller, batter);

                for (var i = 0; i < 10; i++) {
                    expect(controller.freeAgents.batters[i].isSelected).toBe(false);
                }
            });

            it('should mark the batter as not selected if the batter is already selected', function() {

                var batter = Ember.Object.create({isSelected: true});

                controller.freeAgents.batters.push(batter);

                Actions.selectFreeAgentBatter(controller, batter);

                expect(batter.isSelected).toBe(false);
            });
        });

        describe('draftFreeAgent', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({
                    pitchers: [],
                    batters:  []
                });
            });

            it('should draft the pitcher that was selected', function() {

                var freeAgent = {isSelected: true};

                controller.canDraft = true;

                controller.freeAgents.pitchers.push({isSelected: false});
                controller.freeAgents.pitchers.push(freeAgent);

                Actions.draftFreeAgent(controller);

                expect(controller.send).toHaveBeenCalledWith('draftPitcher', freeAgent);
            });

            it('should draft the batter that was selected', function() {

                var freeAgent = {isSelected: true};

                controller.canDraft = true;

                controller.freeAgents.batters.push({isSelected: false});
                controller.freeAgents.batters.push(freeAgent);

                Actions.draftFreeAgent(controller);

                expect(controller.send).toHaveBeenCalledWith('draftBatter', freeAgent);
            });

            it('should not draft any players if drafting is disabled', function() {

                var freeAgent = {isSelected: true};

                controller.canDraft = false;

                controller.freeAgents.batters.push(freeAgent);

                Actions.draftFreeAgent(controller);

                expect(controller.send.calls.count()).toBe(0);
            });
        });

        describe('draftPitcher', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({pitchers: []});
                controller.team = Ember.Object.create({pitchers: [], links: [{rel: 'players', href: '/mba/resources/teams/3/players'}]});
            });

            it('should send an update to the team players resource with the drafted pitcher', function() {

                var pitcher = {player_id: 123};
                Globals.season = 7;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.draftPitcher(controller, pitcher);

                expect($.ajax.calls.mostRecent().args[0]).toEqual('/mba/resources/teams/3/players/123/season/7');
                expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
            });

            it('should remove the pitcher from the free agent pitchers list', function() {

                var pitcher = {player_id: 123};

                controller.freeAgents.pitchers.push(pitcher);

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftPitcher(controller, pitcher);

                expect(controller.freeAgents.pitchers.length).toBe(0);
            });

            it('should add the pitcher to the team pitcher list', function() {

                var pitcher = {player_id: 123};

                controller.freeAgents.pitchers.push(pitcher);

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftPitcher(controller, pitcher);

                expect(controller.team.pitchers.length).toBe(1);
            });

            it('should move to the next team in the draft', function() {

                var pitcher = {player_id: 123};

                controller.freeAgents.pitchers.push(pitcher);

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftPitcher(controller, pitcher);

                expect(controller.send).toHaveBeenCalledWith('showNextTeam');
            });

            it('should show an alert if the ajax call fails', function() {

                var pitcher = {player_id: 123};

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error(); });

                spyOn(window, 'alert').and.callThrough();

                Actions.draftPitcher(controller, pitcher);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('draftBatter', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({batters: []});
                controller.team = Ember.Object.create({batters: [], links: [{rel: 'players', href: '/mba/resources/teams/3/players'}]});
            });

            it('should send an update to the team players resource with the drafted batter', function() {

                var batter = {player_id: 123};
                Globals.season = 7;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.draftBatter(controller, batter);

                expect($.ajax.calls.mostRecent().args[0]).toEqual('/mba/resources/teams/3/players/123/season/7');
                expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
            });

            it('should remove the batter from the free agent batters list', function() {

                var batter = {player_id: 123};

                controller.freeAgents.batters.push(batter);

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftBatter(controller, batter);

                expect(controller.freeAgents.batters.length).toBe(0);
            });

            it('should add the batter to the team batter list', function() {

                var batter = {player_id: 123};

                controller.freeAgents.batters.push(batter);

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftBatter(controller, batter);

                expect(controller.team.batters.length).toBe(1);
            });

            it('should move to the next team in the draft', function() {

                var batter = {player_id: 123};

                controller.freeAgents.batters.push(batter);

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftBatter(controller, batter);

                expect(controller.send).toHaveBeenCalledWith('showNextTeam');
            });

            it('should show an alert if the ajax call fails', function() {

                var batter = {player_id: 123};

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error(); });

                spyOn(window, 'alert').and.callThrough();

                Actions.draftBatter(controller, batter);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('showFirstTeam', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({draftOrder: [13,7,25,9,11]});
            });

            it('should set the current team index to 0', function() {

                Actions.showFirstTeam(controller);

                expect(controller.set).toHaveBeenCalledWith('currentTeamIdx', 0);
            });

            it('should load the team at index 0 of the draft order', function() {

                controller.set = function() {
                    controller.currentTeamIdx = 0;
                }

                Actions.showFirstTeam(controller);

                expect(controller.send).toHaveBeenCalledWith('loadTeam', 13);
            });
        });

        describe('showNextTeam', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.freeAgents = Ember.Object.create({draftOrder: [13,7,25,9,11]});
            });

            it('should load the next team in the draft', function() {

                controller.currentTeamIdx = 3;

                controller.set = function(field, idx) {
                    controller.currentTeamIdx = idx;
                }

                Actions.showNextTeam(controller);

                expect(controller.send).toHaveBeenCalledWith('loadTeam', 11);
            });

            it('should increment the current team index', function() {

                controller.currentTeamIdx = 3;

                controller.set = function(field, idx) {
                    controller.currentTeamIdx = idx;
                }

                Actions.showNextTeam(controller);

                expect(controller.currentTeamIdx).toBe(4);
            });

            it('should disable the draft and complete the stage if the end of the draft order is reached', function() {

                controller.currentTeamIdx = 4;

                Actions.showNextTeam(controller);

                expect(controller.set).toHaveBeenCalledWith('canDraft', false);
                expect(controller.set).toHaveBeenCalledWith('stageComplete', true);
            });
        });

        describe('loadTeam', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            });

            it('should retrieve the team from the team resource endpoint', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.loadTeam(controller, 23);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/teams/23', jasmine.any(Object));
            });

            it('should decorate the team received from the team resource', function() {

                var team = {team_id: 23, links: []};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(team);
                });

                Actions.loadTeam(controller, team.team_id);

                expect(team.pitchers).toEqual([]);
                expect(team.batters ).toEqual([]);
            });

            it('should retrieve the current season team players from the team resource endpoint', function() {

                var team = {team_id: 23, links: [{rel: 'players', href: '/mba/resources/teams/23/players'}]};
                Globals.season = 4;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success(team);
                });

                Actions.loadTeam(controller, team.team_id);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/teams/23/players?season=4', jasmine.any(Object));
            });

            it('should load the team player details', function() {

                var team = {team_id: 23, links: [{rel:'players', href: '/mba/resources/teams/23/players'}]};
                var players = [{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/players/)) {
                        options.success(players);
                    } else {
                        options.success(team);
                    }
                });

                Actions.loadTeam(controller, team.team_id);

                expect(controller.send).toHaveBeenCalledWith('loadPlayers', team, players);
            });

            it('should show an alert if the team ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.loadTeam(controller, 19);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should show an alert if the team players ajax call fails', function() {

                var team = {team_id: 23, links: [{rel:'players', href: '/mba/resources/teams/23/players'}]};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/players/)) {
                        options.error();
                    } else {
                        options.success(team);
                    }
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.loadTeam(controller, team.team_id);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('loadPlayers', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
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

            it('should set draft status', function() {

                var team    = {};
                var players = [];

                Actions.loadPlayers(controller, team, players);

                expect(controller.send).toHaveBeenCalledWith('setDraftStatus');
            });

            it('should add a team to the controller', function() {

                var team    = {};
                var players = [];

                Actions.loadPlayers(controller, team, players);

                expect(controller.set).toHaveBeenCalledWith('team', jasmine.any(Object));
            });
        });

        describe('setDraftStatus', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.team = Ember.Object.create({
                    pitchers: [],
                    batters:  []
                });
            });

            it('should enable drafting', function() {

                Actions.setDraftStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('canDraft', true);
            });

            it('should disable drafting if showing pitchers and the team already drafted a pitcher', function() {

                controller.showFreeAgentPitchers = true;

                for (var i = 0; i < Constants.TEAM_MAX_PITCHERS; i++) {
                    controller.team.pitchers.push({});
                }

                Actions.setDraftStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('canDraft', false);
            });

            it('should disable drafting if showing batters and the team already drafted a batter', function() {

                controller.showFreeAgentBatters = true;

                for (var i = 0; i < Constants.TEAM_MAX_BATTERS; i++) {
                    controller.team.batters.push({});
                }

                Actions.setDraftStatus(controller);

                expect(controller.set).toHaveBeenCalledWith('canDraft', false);
            });

            it('should do nothing if the stage is complete', function() {

                controller.stageComplete = true;

                Actions.setDraftStatus(controller);

                expect(controller.set.calls.count()).toBe(0);
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

            it('should direct the progress controller to move to the next stage', function() {

                controller.stageComplete = true;

                Actions.finishStage(controller);

                expect(progressController.send).toHaveBeenCalledWith('nextStage');
            });

            it('should do nothing if the stage is not complete', function() {

                controller.stageComplete = false;

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.finishStage(controller);

                expect($.ajax.calls.count()).toBe(0);
            });
        });
    });
});
