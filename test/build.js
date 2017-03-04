#!/usr/bin/env node

// NOTE: OSX only for now

var execSync = require('child_process').execSync;

// Create an Electron app for each demo
createApp("OTO Ruler Test");
// Copy electron-prebuilt's dist/Electron.app as a starting point and then
// customize it
function createApp(appName) {
    execSync("rm -rf test/build/" + appName + ".app");
    execSync("./node_modules/.bin/electron test");
}
