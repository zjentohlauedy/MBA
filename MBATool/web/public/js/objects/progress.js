define(['ember'], function(Ember) {

    var Progress = {
        stage: 0,
        stages: [
            { number: "1", name: "Start New Season", status: "progress-curr", route: "start-season"    },
            { number: "2", name: "Roster Cut",       status: "progress-todo", route: "roster-cut"      },
            { number: "3", name: "Rookie Draft",     status: "progress-todo", route: "rookie-draft"    },
            { number: "4", name: "Free Agent Draft", status: "progress-todo", route: "free-agents"     },
            { number: "5", name: "Export Season",    status: "progress-todo", route: "export-season"   },
            { number: "6", name: "Import Season",    status: "progress-todo", route: "import-season"   },
            { number: "7", name: "Accolades",        status: "progress-todo", route: "accolades"       },
            { number: "8", name: "Complete Season",  status: "progress-todo", route: "complete-season" }
        ].map(function(entry){
            return Ember.Object.create().setProperties(entry);
        })
    };

    return Progress;
});
