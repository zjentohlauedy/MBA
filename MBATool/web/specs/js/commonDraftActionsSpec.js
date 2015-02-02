define(['objects/globals', 'utils', 'actions/commonDraftActions'], function(Globals, Utils, Actions) {

    describe('CommonDraftActions', function() {

        describe('prepareData', function() {

            var controller;
            var deferred;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.set.and.callFake(function(field, value) {
                    controller[field] = value;
                });
                controller.availablePlayers = Ember.Object.create({pitchers: [], batters: []});
                controller.draftResource = function() {
                    return "controller-draft-resource";
                };
                controller.playersResource = function() {
                    return "controller-players-resource";
                };

                deferred = jasmine.createSpyObj('deferred', ['resolve','reject']);
            });

            it('should request the draft for the previous season from the drafts resource defined in the controller', function() {

                spyOn($, 'ajax').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('controller-draft-resource', jasmine.any(Object));
            });

            it('should set the draft on the controller', function() {

                var draft = [1,2,3,4,5];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/draft/)) {
                        options.success(draft);
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});
                spyOn(Utils, 'findLink').and.callFake(function() {});

                deferred.resolve.and.callFake(function(payload) { return payload; });

                Actions.prepareData(controller, deferred);

                expect(controller.set).toHaveBeenCalledWith('draftOrder', draft);
            });

            it('should retrieve the available players for the current season from the players resource defined in the controller', function() {

                Globals.season = 6;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([]);
                    }
                });

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('controller-players-resource', jasmine.any(Object));
            });

            it('should add pitchers and batters to available players', function() {

                var availablePlayers = [{player_id: 1},{player_id: 2},{player_id: 3}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') { options.success([]); }
                    else                                    { options.success(availablePlayers); }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {
                    if (player.player_id === 1) { team.pitchers.push(player); }
                    else                        { team.batters. push(player); }
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(controller.availablePlayers.pitchers.length).toBe(1);
                expect(controller.availablePlayers.batters. length).toBe(2);
            });

            it('should call utils loadPlayer for each available player given', function() {

                var players = [{},{},{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([]);
                    } else if (rel == 'controller-players-resource') {
                        options.success(players);
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(Utils.loadPlayer.calls.count()).toEqual(players.length);
            });

            it('should set the current team index to 0', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(controller.set).toHaveBeenCalledWith('currentTeamIdx', 0);
            });

            it('should retrieve the team from the team resource endpoint', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([23]);
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('/mba/resources/teams/23', jasmine.any(Object));
            });

            it('should decorate the team received from the team resource', function() {

                var team = {team_id: 23, links: []};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([23]);
                    } else if (rel == '/mba/resources/teams/23') {
                        options.success(team);
                    } else {
                        options.success([]);
                    }
                });

                Actions.prepareData(controller, deferred);

                expect(team.pitchers).toEqual([]);
                expect(team.batters ).toEqual([]);
            });

            it('should retrieve the current season team players from the team resource endpoint', function() {

                var team = {team_id: 23, links: [{rel: 'players', href: 'team-resource/players'}]};
                Globals.season = 4;

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([23]);
                    } else if (rel == '/mba/resources/teams/23') {
                        options.success(team);
                    } else {
                        options.success([]);
                    }
                });

                Actions.prepareData(controller, deferred);

                expect($.ajax).toHaveBeenCalledWith('team-resource/players?season=4', jasmine.any(Object));
            });

            it('should add pitchers and batters to the given team', function() {

                var team = {team_id: 23, links: [{rel: 'players', href: 'team-resource/players'}]};
                var players = [{player_id: 1},{player_id: 2},{player_id: 3}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([23]);
                    } else if (rel == '/mba/resources/teams/23') {
                        options.success(team);
                    } else if (rel.match(/season/)) {
                        options.success(players);
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function(player, team) {
                    if (player.player_id === 1) {
                        team.pitchers.push(player);
                    } else {
                        team.batters.push(player);
                    }
                });

                Actions.prepareData(controller, deferred);

                expect(team.pitchers.length).toBe(1);
                expect(team.batters. length).toBe(2);
            });

            it('should call utils loadPlayer for each team player given', function() {

                var team = {team_id: 23, links: [{rel: 'players', href: 'team-resource/players'}]};
                var players = [{},{},{},{},{}];

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') {
                        options.success([23]);
                    } else if (rel == '/mba/resources/teams/23') {
                        options.success(team);
                    } else if (rel.match(/season/)) {
                        options.success(players);
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'loadPlayer').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(Utils.loadPlayer.calls.count()).toEqual(players.length);
            });

            it('should set draft status', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(controller.send).toHaveBeenCalledWith('setDraftStatus');
            });

            it('should add a team to the controller', function() {

                var team = {team_id: 12};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/teams/)) {
                        options.success(team);
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(controller.set).toHaveBeenCalledWith('team', team);
            });

            it('should resolve the given promise when the work is complete', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.success([]);
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(deferred.resolve).toHaveBeenCalled();
            });

            it('should reject the promise if the draft ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the draft ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    options.error();
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should reject the promise if the players ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') { options.success([]); }
                    else                                    { options.error();     }
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the players ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel == 'controller-draft-resource') { options.success([]); }
                    else                                    { options.error();     }
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should reject the promise if the team ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/teams/)) {
                        options.error();
                    } else {
                        options.success([]);
                    }
                });

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the team ajax call fails', function() {

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/teams/)) {
                        options.error();
                    } else {
                        options.success([]);
                    }
                });

                spyOn(window, 'alert').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should reject the promise if the team players ajax call fails', function() {

                var team = {team_id: 23, links: [{rel:'players', href: '/mba/resources/teams/23/players'}]};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/teams/)) {
                        options.success({});
                    } else if (rel.match(/season/)) {
                        options.error();
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});

                Actions.prepareData(controller, deferred);

                expect(deferred.reject).toHaveBeenCalled();
            });

            it('should show an alert if the team players ajax call fails', function() {

                var team = {team_id: 23, links: [{rel:'players', href: '/mba/resources/teams/23/players'}]};

                spyOn($, 'ajax').and.callFake(function(rel, options) {
                    if (rel.match(/teams/)) {
                        options.success({});
                    } else if (rel.match(/season/)) {
                        options.error();
                    } else {
                        options.success([]);
                    }
                });

                spyOn(Utils, 'findLink').and.callFake(function() {});
                spyOn(window, 'alert').and.callThrough();

                Actions.prepareData(controller, deferred);

                expect(window.alert).toHaveBeenCalled();
            });
        });

        describe('toggleAvailablePlayersTable', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.availablePlayers = Ember.Object.create({
                    pitchers: [],
                    batters:  []
                });
            });

            describe('when showing available pitchers', function() {

                beforeEach(function() {
                    controller.showAvailablePitchers = true;
                    controller.showAvailableBatters = false;
                });

                it('should mark available pitchers as not being shown', function() {

                    Actions.toggleAvailablePlayersTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showAvailablePitchers', false);
                });

                it('should mark available batters as being shown', function() {

                    Actions.toggleAvailablePlayersTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showAvailableBatters', true);
                });

                it('should change the text on the toggle button to "show pitchers"', function() {

                    Actions.toggleAvailablePlayersTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('toggleButtonLabel', 'Show Pitchers');
                });

                it('should deselect all available pitchers', function() {

                    for (var i = 0; i < 10; i++) {
                        controller.availablePlayers.pitchers.push({isSelected: true});
                    }

                    Actions.toggleAvailablePlayersTable(controller);

                    for (var i = 0; i < 10; i++) {
                        expect(controller.availablePlayers.pitchers[i].isSelected).toBe(false);
                    }
                });
            });

            describe('when showing available batters', function() {

                beforeEach(function() {
                    controller.showAvailablePitchers = false;
                    controller.showAvailableBatters = true;
                });

                it('should mark available batters as not being shown', function() {

                    Actions.toggleAvailablePlayersTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showAvailableBatters', false);
                });

                it('should mark available pitchers as being shown', function() {

                    Actions.toggleAvailablePlayersTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('showAvailablePitchers', true);
                });

                it('should change the text on the toggle availables button to "show batters"', function() {

                    Actions.toggleAvailablePlayersTable(controller);

                    expect(controller.set).toHaveBeenCalledWith('toggleButtonLabel', 'Show Batters');
                });

                it('should deselect all available batters', function() {

                    for (var i = 0; i < 10; i++) {
                        controller.availablePlayers.batters.push({isSelected: true});
                    }

                    Actions.toggleAvailablePlayersTable(controller);

                    for (var i = 0; i < 10; i++) {
                        expect(controller.availablePlayers.batters[i].isSelected).toBe(false);
                    }
                });
            });
        });

        describe('sortPitchers', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.availablePlayers = Ember.Object.create({ pitchers: [
                    {player_id: 1, field1: 5, field2: 1, rating: 9},
                    {player_id: 2, field1: 4, field2: 1, rating: 7},
                    {player_id: 3, field1: 6, field2: 1, rating: 8}
                ] });
            });

            it('should sort the pitchers by the given field', function() {

                controller.currentPitcherSortField = null;

                Actions.sortPitchers(controller, 'field1');

                expect(controller.availablePlayers.pitchers[0].player_id).toBe(2);
                expect(controller.availablePlayers.pitchers[1].player_id).toBe(1);
                expect(controller.availablePlayers.pitchers[2].player_id).toBe(3);
            });

            it('should set the given field as the current pitcher sort field', function() {

                controller.currentPitcherSortField = null;

                Actions.sortPitchers(controller, 'field2');

                expect(controller.set).toHaveBeenCalledWith('currentPitcherSortField', 'field2');
            });

            it('should reverse the order if the given field is the same as the current sort field', function() {

                controller.currentPitcherSortField = 'field1';

                Actions.sortPitchers(controller, 'field1');

                expect(controller.availablePlayers.pitchers[0].player_id).toBe(3);
                expect(controller.availablePlayers.pitchers[1].player_id).toBe(2);
                expect(controller.availablePlayers.pitchers[2].player_id).toBe(1);
            });
        });

        describe('sortBatters', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.availablePlayers = Ember.Object.create({ batters: [
                    {player_id: 1, field1: 5, field2: 1, rating: 9},
                    {player_id: 2, field1: 4, field2: 1, rating: 7},
                    {player_id: 3, field1: 6, field2: 1, rating: 8}
                ] });
            });

            it('should sort the batters by the given field', function() {

                controller.currentBatterSortField = null;

                Actions.sortBatters(controller, 'field1');

                expect(controller.availablePlayers.batters[0].player_id).toBe(2);
                expect(controller.availablePlayers.batters[1].player_id).toBe(1);
                expect(controller.availablePlayers.batters[2].player_id).toBe(3);
            });

            it('should set the given field as the current batter sort field', function() {

                controller.currentBatterSortField = null;

                Actions.sortBatters(controller, 'field2');

                expect(controller.set).toHaveBeenCalledWith('currentBatterSortField', 'field2');
            });

            it('should reverse the order if the given field is the same as the current sort field', function() {

                controller.currentBatterSortField = 'field1';

                Actions.sortBatters(controller, 'field1');

                expect(controller.availablePlayers.batters[0].player_id).toBe(3);
                expect(controller.availablePlayers.batters[1].player_id).toBe(2);
                expect(controller.availablePlayers.batters[2].player_id).toBe(1);
            });
        });

        describe('selectAvailablePlayer', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
            });

            it('should mark the given player as selected', function() {

                var player  = Ember.Object.create({isSelected: false});
                var players = Ember.A([]);

                players.push(player);

                Actions.selectAvailablePlayer(controller, players, player);

                expect(player.isSelected).toBe(true);
            });

            it('should mark all other players as not selected', function() {

                var player = Ember.Object.create({isSelected: false});
                var players = Ember.A([]);

                for (var i = 0; i < 10; i++) {
                    players.push({isSelected: true});
                }

                players.push(player);

                Actions.selectAvailablePlayer(controller, players, player);

                for (var i = 0; i < 10; i++) {
                    expect(players[i].isSelected).toBe(false);
                }
            });

            it('should mark the player as not selected if the player is already selected', function() {

                var player = Ember.Object.create({isSelected: true});
                var players = Ember.A([]);

                players.push(player);

                Actions.selectAvailablePlayer(controller, players, player);

                expect(player.isSelected).toBe(false);
            });
        });

        describe('draftFreeAgent', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.team = Ember.Object.create({batters: [], pitchers: [], links: [{rel: 'players', href: 'team-players-resource'}]});
                controller.availablePlayers = Ember.Object.create({
                    pitchers: [],
                    batters:  []
                });
                controller.canDraft = true;
            });

            describe('when a pitcher has been selected', function() {

                it('should send an update to the team players resource with the selected pitcher', function() {

                    controller.availablePlayers.pitchers.push({player_id: 123, isSelected: true});
                    Globals.season = 7;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.draftSelectedPlayer(controller);

                    expect($.ajax.calls.mostRecent().args[0]).toEqual('team-players-resource/123/season/7');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                });

                it('should remove the pitcher from the available players pitchers list', function() {

                    controller.availablePlayers.pitchers.push({player_id: 123, isSelected: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                    Actions.draftSelectedPlayer(controller);

                    expect(controller.availablePlayers.pitchers.length).toBe(0);
                });

                it('should add the pitcher to the team pitcher list', function() {

                    controller.availablePlayers.pitchers.push({player_id: 123, isSelected: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                    Actions.draftSelectedPlayer(controller);

                    expect(controller.team.pitchers.length).toBe(1);
                });
            });

            describe('when a batter has been selected', function() {

                it('should send an update to the team players resource with the selected batter', function() {

                    controller.availablePlayers.batters.push({player_id: 123, isSelected: true});
                    Globals.season = 7;

                    spyOn($, 'ajax').and.callFake(function() {});

                    Actions.draftSelectedPlayer(controller);

                    expect($.ajax.calls.mostRecent().args[0]).toEqual('team-players-resource/123/season/7');
                    expect($.ajax.calls.mostRecent().args[1].type).toEqual('POST');
                });

                it('should remove the batter from the available players batters list', function() {

                    controller.availablePlayers.batters.push({player_id: 123, isSelected: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                    Actions.draftSelectedPlayer(controller);

                    expect(controller.availablePlayers.batters.length).toBe(0);
                });

                it('should add the batter to the team batter list', function() {

                    controller.availablePlayers.batters.push({player_id: 123, isSelected: true});

                    spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                    Actions.draftSelectedPlayer(controller);

                    expect(controller.team.batters.length).toBe(1);
                });
            });

            it('should move to the next team in the draft', function() {

                controller.availablePlayers.batters.push({player_id: 123, isSelected: true});

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftSelectedPlayer(controller);

                expect(controller.send).toHaveBeenCalledWith('showNextTeam');
            });

            it('should show an alert if the ajax call fails', function() {

                controller.availablePlayers.pitchers.push({player_id: 123, isSelected: true});

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.error(); });

                spyOn(window, 'alert').and.callThrough();

                Actions.draftSelectedPlayer(controller);

                expect(window.alert).toHaveBeenCalled();
            });

            it('should not draft any players if no players are selected', function() {

                controller.availablePlayers.pitchers.push({player_id: 123, isSelected: false});
                controller.availablePlayers.batters. push({player_id: 456, isSelected: false});

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftSelectedPlayer(controller);

                expect($.ajax).not.toHaveBeenCalled();
                expect(controller.send).not.toHaveBeenCalled();
                expect(controller.availablePlayers.pitchers.length).toBe(1);
                expect(controller.availablePlayers.batters. length).toBe(1);
                expect(controller.team.pitchers.length).toBe(0);
                expect(controller.team.batters. length).toBe(0);
            });

            it('should not draft any players if drafting is disabled', function() {

                controller.availablePlayers.pitchers.push({player_id: 123, isSelected: true});
                controller.canDraft = false;

                spyOn($, 'ajax').and.callFake(function(rel, options) { options.success(); });

                Actions.draftSelectedPlayer(controller);

                expect($.ajax).not.toHaveBeenCalled();
                expect(controller.send).not.toHaveBeenCalled();
                expect(controller.availablePlayers.pitchers.length).toBe(1);
                expect(controller.team.pitchers.length).toBe(0);
            });
        });

        describe('showFirstTeam', function() {

            var controller;

            beforeEach(function() {

                controller = jasmine.createSpyObj('controller', ['send', 'get', 'set']);
                controller.draftOrder = [13,7,25,9,11];
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
                controller.draftOrder = [13,7,25,9,11];
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
    });
});
