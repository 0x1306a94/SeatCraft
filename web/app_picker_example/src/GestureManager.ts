enum DeviceType {
    MOUSE,
    TOUCH
}

export interface GestureManagerOptions {
    onPan?: (deltaX: number, deltaY: number) => void;
    onZoom?: (scale: number, centerX: number, centerY: number) => void;
}

export class GestureManager {
    private pinchTimeout = 150; // ms
    private timer: number | undefined;

    private lastEventTime = 0;
    private lastDeltaY = 0;
    private timeThreshold = 50; // ms
    private deltaYThreshold = 50;
    private deltaYChangeThreshold = 10;
    private mouseWheelRatio = 800;
    private touchWheelRatio = 100;

    private onPan: ((deltaX: number, deltaY: number) => void) | undefined;
    private onZoom: ((scale: number, centerX: number, centerY: number) => void) | undefined;

    constructor(options: GestureManagerOptions) {
        this.onPan = options.onPan;
        this.onZoom = options.onZoom;
    }

    private handleScrollEvent(
        event: WheelEvent,
        deviceType: DeviceType = DeviceType.MOUSE,
    ) {
        const deltaX = event.shiftKey && event.deltaX === 0 && event.deltaY !== 0 ? 0 : event.deltaX;
        const deltaY = event.deltaY;

        if (this.onPan) {
            this.onPan(-deltaX * window.devicePixelRatio, -deltaY * window.devicePixelRatio);
        }
    }

    private handleScaleEvent(
        event: WheelEvent,
        canvas: HTMLElement,
        deviceType: DeviceType = DeviceType.MOUSE
    ) {
        const rect = canvas.getBoundingClientRect();
        const pixelX = (event.clientX - rect.left) * window.devicePixelRatio;
        const pixelY = (event.clientY - rect.top) * window.devicePixelRatio;
        const scale = Math.exp(-(event.deltaY) / (deviceType === DeviceType.MOUSE ? this.mouseWheelRatio : this.touchWheelRatio));

        if (this.onZoom) {
            this.onZoom(scale, pixelX, pixelY);
        }
    }

    public clearState() {
        this.timer = undefined;
    }

    private resetTimeout() {
        clearTimeout(this.timer);
        this.timer = window.setTimeout(() => {
            this.timer = undefined;
            this.clearState();
        }, this.pinchTimeout);
    }

    private getDeviceType(event: WheelEvent): DeviceType {
        const now = Date.now();
        const timeDifference = now - this.lastEventTime;
        const deltaYChange = Math.abs(event.deltaY - this.lastDeltaY);
        let isTouchpad = false;
        if (event.deltaMode === event.DOM_DELTA_PIXEL && timeDifference < this.timeThreshold) {
            if (Math.abs(event.deltaY) < this.deltaYThreshold && deltaYChange < this.deltaYChangeThreshold) {
                isTouchpad = true;
            }
        }
        this.lastEventTime = now;
        this.lastDeltaY = event.deltaY;
        return isTouchpad ? DeviceType.TOUCH : DeviceType.MOUSE;
    }

    public onWheel(event: WheelEvent, canvas: HTMLElement) {
        const deviceType = this.getDeviceType(event);
        if (!event.deltaY || (!event.ctrlKey && !event.metaKey)) {
            this.resetTimeout();
            this.handleScrollEvent(event, deviceType);
            return;
        }
        this.resetTimeout();
        this.handleScaleEvent(event, canvas, deviceType);
    }
}