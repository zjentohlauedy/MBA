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
