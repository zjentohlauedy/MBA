window.App = Ember.Application.create();

var App = window.App

App._progress = {
    stage: 0,
    stages: [
        { name: "Start New Season", isDone: false, route: "start-season"  },
        { name: "Roster Cut",       isDone: false, route: "roster-cut"    },
        { name: "Rookie Draft",     isDone: false, route: "rookie-draft"  },
        { name: "Free Agent Draft", isDone: false, route: "free-agents"   },
        { name: "Export Season",    isDone: false, route: "export-season" },
        { name: "Import Season",    isDone: false, route: "import-season" }
    ].map(function(entry){
        return Ember.Object.create().setProperties(entry);
    })
};

App._teams = [
    { name: "Team01" },
    { name: "Team02" },
    { name: "Team03" },
    { name: "Team04" },
    { name: "Team05" },
    { name: "Team06" },
    { name: "Team07" },
    { name: "Team08" },
    { name: "Team09" },
    { name: "Team10" },
    { name: "Team11" },
    { name: "Team12" },
    { name: "Team13" },
    { name: "Team14" },
    { name: "Team15" },
    { name: "Team16" },
    { name: "Team17" },
    { name: "Team18" },
    { name: "Team19" },
    { name: "Team20" },
    { name: "Team21" },
    { name: "Team22" },
    { name: "Team23" },
    { name: "Team24" },
    { name: "Team25" },
    { name: "Team26" },
    { name: "Team27" },
    { name: "Team28" },
    { name: "Team29" },
    { name: "Team30" },
    { name: "Team31" },
    { name: "Team32" }
];

App.Router.map(function() {
    this.route("start-season",    { path: "/start"  });
    this.route("roster-cut",      { path: "/cut"    });
    this.route("rookie-draft",    { path: "/rookie" });
    this.route("free-agents",     { path: "/agents" });
    this.route("export-season",   { path: "/export" });
    this.route("import-season",   { path: "/import" });
    this.route("season-complete", { path: "/done"   });
});

App.IndexRoute = Ember.Route.extend({
  redirect: function() {
    this.transitionTo(App._progress.stages[App._progress.stage].route);
  }
});

App.ProgressController = Ember.ObjectController.extend({
    stage:  App._progress.stage,
    stages: App._progress.stages,
    actions: {
        nextStage: function() {
            if (this.stage >= this.stages.length) {
                this.stages.setEach("isDone", false);
                this.stage = 0;

                this.transitionToRoute(this.stages[this.stage].route);

                return;
            }

            this.stages[this.stage].set("isDone", true);
            this.stage++;

            if (this.stage < this.stages.length) {
                this.transitionToRoute(this.stages[this.stage].route);
            }
            else {
                this.transitionToRoute("season-complete");
            }
        }
    }
});

App.RosterCutController = Ember.ObjectController.extend({
    teams: App._teams,
    pitchers: [
        { name: "Conley, Bud",        record: "2 - 11", era: "6.84", vsba: ".239", ipg: "6.25", whip: "1.569", sop9: "5.40", eff: "-58" },
        { name: "Gold, Johnny",       record: "3 - 5",  era: "4.67", vsba: ".215", ipg: "6.93", whip: "1.288", sop9: "7.88", eff: "+3"  },
        { name: "Hartman, Dave",      record: "0 - 0",  era: "9.00", vsba: ".227", ipg: "5.00", whip: "1.400", sop9: "3.60", eff: "-3"  },
        { name: "Hearns, Bill",       record: "1 - 0",  era: "2.08", vsba: ".211", ipg: "0.87", whip: "1.385", sop9: "9.69", eff: "+3"  },
        { name: "Michael, Danny",     record: "0 - 0",  era: "2.11", vsba: ".206", ipg: "2.84", whip: "0.998", sop9: "8.98", eff: "+6"  },
        { name: "Montgomery, Curtis", record: "5 - 5",  era: "4.20", vsba: ".217", ipg: "1.94", whip: "1.450", sop9: "8.25", eff: "-1"  },
        { name: "Risc, Paul",         record: "3 - 7",  era: "6.37", vsba: ".215", ipg: "7.38", whip: "1.302", sop9: "5.91", eff: "-19" },
        { name: "Stern, Dan",         record: "6 - 1",  era: "4.22", vsba: ".213", ipg: "6.75", whip: "1.111", sop9: "5.44", eff: "-12" },
        { name: "Thorpe, Denny",      record: "1 - 1",  era: "5.57", vsba: ".250", ipg: "1.11", whip: "1.762", sop9: "8.57", eff: "-9"  },
        { name: "Vowles, Keith",      record: "5 - 5",  era: "4.12", vsba: ".236", ipg: "1.92", whip: "1.581", sop9: "5.43", eff: "-27" }
    ]
});

App.NextButtonController = Ember.ObjectController.extend({
    needs: ['progress']
});

App.NextButtonView = Ember.View.extend({
    templateName: "next-button",
    name: "Next",
    click: function(event) {
        this.get("controller.controllers.progress").send('nextStage');
    }
});
