var electron = require('electron');
var execSync = require('child_process').execSync;
var path = require('path');
var os = require('os');

var oto_util = {
    /**
     * Converts value in cm to value in inches
     */
    cmToInch: function(cm) {
        // 1 inch = 2.54 cm
        return cm * 2.54;
    },
    /**
     * Get the display that the window is mostly over
     */
    getBrowserWindowDisplay: function () {
        // Don't require remote unless we hit here as it can't be required in the
        // main process. 
        return electron.screen.getDisplayMatching(electron.remote.getCurrentWindow().getBounds());
    },

    /**
     * Split display information string into components
     */
    splitDisplayInformation: function(infoString) {
        var splitInfo = infoString.split(',');
        return {
            pixel: {
                width: parseFloat(splitInfo[0]),
                height: parseFloat(splitInfo[1]),
            },
            physical: {
                width: parseFloat(splitInfo[2]),
                height: parseFloat(splitInfo[3]),
            },
            ppcm: {
                horizontal: parseFloat(splitInfo[4]),
                vertical: parseFloat(splitInfo[5]),
            }
        };
    }
};

var oto_core = {
    supported: {
        darwin: {name: 'darwin', path: '../../build/darwin/one_to_one/Build/Products/Debug/one_to_one'},
    },
    /**
     * Gets information for a display using native binary
     */
     getDisplayInformation: function(display) {
        // If on a supported system use our native binary to do the work, otherwise fall back
        // to a default
        var displayInformation = {};
        switch(os.platform()) {
            case oto_core.supported.darwin.name:
                displayInformation = oto_util.splitDisplayInformation(
                    execSync(
                        path.resolve(__dirname, oto_core.supported.darwin.path) + " " + display.id
                    ).toString()
                );
                break;
            default:
                displayInformation = {}
                displayInformation.ppcm = {
                    horizontal: 28.34,
                    vertical: 28.834,
                };
                displayInformation.physical = {
                    width: display.size.width / displayInformation.ppcm.width,
                    height: display.size.height / displayInformation.ppcm.height,
                };
                displayInformation.pixel = {
                    width: display.size.width,
                    height: display.size.height,
                };

                break;
        }

        return displayInformation;
     }
};

var one_to_one = {

    /**
     * Get the pixels per centimetre of a screen.
     * 
     * @param [display] (Electron Screen Instance)
     *        or the display that the process is in will be used.
     * @returns Pixels per centimetre of the display.
     */
    ppcm: function (display) {
        display = display || oto_util.getBrowserWindowDisplay();
        return oto_core.getDisplayInformation(display).ppcm;
    },

    /**
      * Get the pixels per inch of a screen.
      * 
      * @param [display] (Electron Screen Instance)
      *        or the display that the process is in will be used.
      * @returns Pixels per inch of the display.
      */
    ppi: function (display) {
        display = display || oto_util.getBrowserWindowDisplay();
        var ppcm = oto_core.getDisplayInformation(display).ppcm;
        return {
            horizontal: oto_util.cmToInch(ppcm.horizontal),
            vertical: oto_util.cmToInch(ppcm.vertical),
        };
    },
    /**
     * Get the physical dimensions of the screen in millimetres.
     * 
     * @param [display] (Electron Screen Instance)
     *        or the display that the process is in will be used
     * @returns Physical dimensions of the screen in millimetres.
     */
    physicalSize: function(display) {
        display = display || oto_util.getBrowserWindowDisplay();
        return oto_core.getDisplayInformation(display).physical;
    },

    pixelSize: function(display) {
        display = display || oto_util.getBrowserWindowDisplay();
        return oto_core.getDisplayInformation(display).pixel;
    }
};

module.exports = one_to_one;

