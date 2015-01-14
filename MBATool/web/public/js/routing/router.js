define(['ember'], function(Ember) {

    var Router = Ember.Router.extend();

    Router.map(function() {
        this.route("start-season",    { path: "/start"  });
        this.route("roster-cut",      { path: "/cut"    });
        this.route("rookie-draft",    { path: "/rookie" });
        this.route("free-agents",     { path: "/agents" });
        this.route("export-season",   { path: "/export" });
        this.route("import-season",   { path: "/import" });
        this.route("season-complete", { path: "/done"   });
    });

    return Router;
});
