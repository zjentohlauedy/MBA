define(['objects/constants', 'objects/globals', 'utils', 'ember'], function(Constants, Globals, Utils, Ember) {

    var RosterCutController = Ember.ObjectController.extend({
        needs:          ["progress", "rookie-draft"],
        teams:          [],
        currentTeam:    Ember.Object.create(),
        rookies:        Ember.Object.create().setProperties({
            draftOrder: [],
            pitchers:   [],
            batters:    []
        }),
        pitchers: {
            isComplete: false,
            isError:    false
        },
        batters: {
            isComplete: false,
            isError:    false
        },
        stageComplete:  false,
        actions: {
            selectTeam: function(team) {
                if (team.isSelected) return;

                team.set("isSelected", true);
                this.currentTeam.set("isSelected", false);
                this.set( "currentTeam", team );


                if ( team.pitchers.length == 0 ) {
                    var controller = this;

                    $.ajax( "/mba/resources/teams/" + team.team_id + "/players?season=" + Globals.season + "&phase=1", {
                        success: function(players) {
                            controller.send('loadPlayers', team, players );
                        },
                        error: function() {
                            alert("Error loading players!")
                        }
                    });
                }
                else
                {
                    this.send("updatePitchersStatus");
                    this.send("updateBattersStatus");
                }
            },
            loadPlayers: function(team, players) {
                var promises = [];
                var controller = this;

                for (var i = 0; i < players.length; i++) {
                    promises.push( Utils.loadPlayer(players[i], team, Globals.season, true) );
                }

                $.when.apply(null, promises).done(function() {
                    controller.send("updatePitchersStatus");
                    controller.send("updateBattersStatus");
                    controller.send("updateTeamStatus");
                });
            },
            toggleCutPitcher: function(pitcher) {
                var controller = this;
                var url = Utils.findLink( this.currentTeam.links, "players" ) + "/" + pitcher.player_id + "/season/" + Globals.season;

                $.ajax( url, {
                    type: pitcher.isCut ? 'POST' : 'DELETE',
                    success: function() {
                        pitcher.set("isCut", !pitcher.isCut);

                        controller.send("updatePitchersStatus");
                        controller.send("updateTeamStatus");
                    },
                    error: function() {
                        alert("Error modifying pitcher!");
                    }
                });
            },
            toggleCutBatter: function(batter) {
                var controller = this;
                var url = Utils.findLink( this.currentTeam.links, "players" ) + "/" + batter.player_id + "/season/" + Globals.season;

                $.ajax( url, {
                    type: batter.isCut ? 'POST' : 'DELETE',
                    success: function() {
                        batter.set("isCut", !batter.isCut);

                        controller.send("updateBattersStatus");
                        controller.send("updateTeamStatus");
                    },
                    error: function() {
                        alert("Error modifying batter!");
                    }
                });
            },
            updatePitchersStatus: function() {
                var count = this.currentTeam.pitchers.reduce(function(value, pitcher){ return (pitcher.isCut) ? value : value + 1; }, 0);

                if      (count > Constants.ROSTER_CUT_TARGET_PITCHERS) { this.set("pitchers.isComplete", false); this.set("pitchers.isError", false); }
                else if (count < Constants.ROSTER_CUT_TARGET_PITCHERS) { this.set("pitchers.isComplete", false); this.set("pitchers.isError", true ); }
                else                                                        { this.set("pitchers.isComplete", true ); this.set("pitchers.isError", false); }
            },
            updateBattersStatus: function() {
                var count = this.currentTeam.batters.reduce(function(value, batter){ return (batter.isCut) ? value : value + 1; }, 0);

                if      (count > Constants.ROSTER_CUT_TARGET_BATTERS) { this.set("batters.isComplete", false); this.set("batters.isError", false); }
                else if (count < Constants.ROSTER_CUT_TARGET_BATTERS) { this.set("batters.isComplete", false); this.set("batters.isError", true ); }
                else                                                       { this.set("batters.isComplete", true ); this.set("batters.isError", false); }
            },
            updateTeamStatus: function() {
                if   (this.pitchers.isComplete && this.batters.isComplete) this.currentTeam.set("isComplete", true );
                else                                                       this.currentTeam.set("isComplete", false);

                if   (this.pitchers.isError || this.batters.isError) this.currentTeam.set("isError", true );
                else                                                 this.currentTeam.set("isError", false);

                this.send("updateStatus");
            },
            updateStatus: function() {
                var count = this.teams.reduce(function(value, team){ return (team.isComplete) ? value : value + 1; }, 0);

                if   (count > 0) this.set("stageComplete", false);
                else             this.set("stageComplete", true );
            },
            loadRookies: function(rookies) {
                var promises = [];
                var controller = this;

                for (var i = 0; i < rookies.length; i++) {
                    promises.push( Utils.loadPlayer(rookies[i], controller.rookies, Globals.season, false) );
                }

                $.when.apply(null, promises).done(function() {
                    controller.rookies.set( "pitchers", controller.rookies.pitchers.sortBy("rating").reverseObjects());
                    controller.rookies.set( "batters",  controller.rookies.batters .sortBy("rating").reverseObjects());

                    controller.get("controllers.rookie-draft").set('rookies', controller.rookies);
                    controller.get("controllers.rookie-draft").send('showFirstTeam');
                    controller.get("controllers.progress").send('nextStage');
                });
            },
            finishStage: function() {
                var controller = this;

                if (this.stageComplete) {
                    $.ajax( "/mba/resources/drafts/rookie/season/" + (Globals.season - 1), {
                        success: function(draft) {
                            controller.rookies.set( "draftOrder", draft );

                            $.ajax( "/mba/resources/players?rookie=true&season=" + Globals.season, {
                                success: function(rookies) {
                                    controller.send( "loadRookies", rookies );
                                },
                                error: function() {
                                    alert("Error retrieving rookies!");
                                }
                            });
                        },
                        error: function() {
                            alert("Error retrieving draft!");
                        }
                    });
                }
            }
        }
    });

    return RosterCutController;
});
