var oto = require('../index.js');
console.log('loaded');
const electron = require('electron');

function resizeRuler() {
    var ruler = document.getElementById('ruler');
    var pixelsPerCM = oto.ppcm();
    var rulerLength = 30;
    var pixelLength = 30 * pixelsPerCM.horizontal;

    ruler.width = pixelLength;
};