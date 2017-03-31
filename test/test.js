var oto = require('../index.js');
console.log('loaded');
function resizeRuler() {
    console.log(oto);
    var ruler = document.getElementById('ruler');
    var pixelsPerCM = oto.ppcm();
    var rulerLength = 30;
    var pixelLength = 30 * pixelsPerCM.horizontal;
    console.log(pixelLength);
    ruler.width = pixelLength;
};