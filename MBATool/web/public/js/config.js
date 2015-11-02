define({
    app_name: "mba-tool",
    urlArgs: "bust=" + (new Date()).getTime(),
    shim: {
        'ember': {
            deps: ['handlebars', 'jquery'],
            exports: 'Ember'
        }
    },
    paths: {
        'App': 'application',
        'jquery': 'libs/jquery/dist/jquery',
        'handlebars': 'libs/handlebars/handlebars',
        'ember': 'libs/ember/ember',
        'sprintf': 'libs/sprintf/dist/sprintf.min'
    }
});
