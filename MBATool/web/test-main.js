var allTestFiles = [];
var TEST_REGEXP = /(spec|test)\.js$/i;

var pathToModule = function(path) {
  return path.replace(/^\/base\//, '').replace(/\.js$/, '');
};

Object.keys(window.__karma__.files).forEach(function(file) {
  if (TEST_REGEXP.test(file)) {
    // Normalize paths to RequireJS module names.
    allTestFiles.push(pathToModule(file));
  }
});

require.config({
  // Karma serves files under /base, which is the basePath from your config file
  baseUrl: '/base',

  shim: {
    'ember': {
      deps: ['handlebars', 'jquery'],
      exports: 'Ember'
    }
  },

  paths: {
    'App': 'public/js/application',
    'utils': 'public/js/utils',
    'controllers': 'public/js/controllers',
    'objects': 'public/js/objects',
    'routing': 'public/js/routing',
    'ember': 'public/js/libs/ember/ember',
    'handlebars': 'public/js/libs/handlebars/handlebars',
    'jquery': 'public/js/libs/jquery/dist/jquery'
  },

  // dynamically load all test files
  deps: allTestFiles,

  // we have to kickoff jasmine, as it is asynchronous
  callback: window.__karma__.start
});
