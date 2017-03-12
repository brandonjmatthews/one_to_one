#!/usr/bin/env node

// NOTE: OSX only for now
var os = require('os');
var fs = require('fs');
var execSync = require('child_process').execSync;

// Create an Electron app for each demo
// Copy electron-prebuilt's dist/Electron.app as a starting point and then
// customize it

switch(os.platform()) {
    case 'darwin':
        darwinCreateApp('OTO Ruler Test');
        break;
    case 'win32':
        win32CreateApp('OTO Ruler Test');
        break;
}

function darwinCreateApp(appName) {
    execSync('rm -rf test/build/' + appName + '.app');
    execSync('./node_modules/.bin/electron test');
}

function win32CreateApp(appName) {
    if (fs.existsSync('test/build/' + appName + '.exe')) {
        execSync('del test\\build\\' + appName + '.exe');
    }
    execSync('.\\node_modules\\.bin\\electron test');
}