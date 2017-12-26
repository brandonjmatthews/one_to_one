'use strict';
const electron = require('electron');
const path = require('path');
const os = require('os');

const OTO_Addon = require('bindings')('OneToOne');

const oto_util = {
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
};

const oto_core = {
    supported: [
        'darwin',
        'win32',
    ],
    /**
     * Gets information for a display using native binary
     */
     getDisplayInformation: function(display) {
        // If on a supported system use our native binary to do the work, otherwise fall back
        // to a default
        
        let displayInformation = {};

        if (oto_core.supported.indexOf(os.platform()) !== -1) {
            displayInformation = OTO_Addon.detectScreenSize(display.id);
            console.log(displayInformation);
        } else {
            displayInformation = {}
            displayInformation.ppcm = {
                horizontal: 28.34,
                vertical: 28.34,
            };
            displayInformation.physical = {
                width: display.size.width / displayInformation.ppcm.width,
                height: display.size.height / displayInformation.ppcm.height,
            };
            displayInformation.pixel = {
                width: display.size.width,
                height: display.size.height,
            };
        }

        return displayInformation;
     }
};

const one_to_one = {

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
        const ppcm = oto_core.getDisplayInformation(display).ppcm;
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

