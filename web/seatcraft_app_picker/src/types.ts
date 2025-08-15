/// <reference types="emscripten" />
export declare class SeatCraftViewCore {
    public static MakeFrom(canvasId: string, density: number): SeatCraftViewCore | null
    public updateAreaSvgData(data: Uint8Array): void;
    public updateSize(): boolean;
    public handlePan(deltaX: number, deltaY: number): void;
    public handlePinch(scale: number, cx: number, cy: number): void;
    public draw(force: boolean): void;
}

export declare interface SeatCraftAppPicker extends EmscriptenModule {
    SeatCraftViewCore: typeof SeatCraftViewCore
    [key: string]: any;
}
