import {
    ShareData,
    updateSize,
    onResizeEvent,
    animationLoop,
    setupVisibilityListeners,
    setupGesture,
} from "./common";

import { SeatCraftAppPickerBind } from 'seatcraft_app_picker'

import SeatCraftAppPickerInit from '../../out/SeatCraftAppPicker';

let shareData: ShareData = new ShareData();
shareData.canvasId = "seatcraft_picker_view"
shareData.containerId = "container"

if (typeof window != "undefined") {
    window.onload = async () => {
        try {
            console.log('Starting SeatCraftAppPicker module initialization...');
            shareData.module = await SeatCraftAppPickerInit({
                locateFile: (file: string) => '/wasm/' + file,
                mainScriptUrlOrBlob: '/wasm/SeatCraftAppPicker.js'
            });

            console.log('SeatCraftAppPicker Module loaded:', shareData.module);
            if (!shareData.module) {
                throw new Error('SeatCraftAppPicker not found in Module');
            }

            SeatCraftAppPickerBind(shareData.module)

            const density = window.devicePixelRatio
            console.log('devicePixelRatio:', density);
            shareData.viewCore = shareData.module.SeatCraftViewCore.MakeFrom("#" + shareData.canvasId, density)
            if (!shareData.viewCore) {
                throw new Error('SeatCraftViewCore not create in SeatCraftAppPicker Module');
            }
            console.log('SeatCraftViewCore created successfully');

            const svgBuffer = await fetch("/resources/svg/performbg_2.svg").then((response) => response.arrayBuffer());
            console.log('svgBuffer', svgBuffer);
            const svgUIntArray = new Uint8Array(svgBuffer);
            shareData.viewCore.updateAreaSvgData(svgUIntArray);
            setupGesture(shareData)
            updateSize(shareData)
            animationLoop(shareData)
            setupVisibilityListeners(shareData)
        } catch (error) {
            console.error(error);
            throw new Error("SeatCraftAppPicker init failed. Please check the .wasm file path!.");
        }
    }

    window.onresize = () => {
        onResizeEvent(shareData)
    }
}