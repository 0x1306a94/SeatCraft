
import { SeatCraftAppPicker, SeatCraftViewCore } from 'seatcraft_app_picker/types/types'
import { GestureManager } from "./GestureManager";
export class ShareData {
    public module: SeatCraftAppPicker | null = null;
    public containerId: string | null = null
    public canvasId: string | null = null
    public viewCore: SeatCraftViewCore | null = null;
    public animationFrameId: number | null = null;
    public isPageVisible: boolean = true;
    public resized: boolean = false;
    public gestureManager: GestureManager | null = null;
}

function draw(shareData: ShareData) {
    if (shareData.viewCore) {
        shareData.viewCore.draw(false)
    }
}

export function updateSize(shareData: ShareData) {
    if (!shareData.viewCore || !shareData.canvasId || !shareData.containerId) {
        return;
    }
    shareData.resized = false;
    const canvas = document.getElementById(shareData.canvasId) as HTMLCanvasElement;
    const container = document.getElementById(shareData.containerId) as HTMLDivElement;
    const screenRect = container.getBoundingClientRect();
    const scaleFactor = window.devicePixelRatio;
    canvas.width = screenRect.width * scaleFactor;
    canvas.height = screenRect.height * scaleFactor;
    canvas.style.width = screenRect.width + "px";
    canvas.style.height = screenRect.height + "px";
    const sizeChanged = shareData.viewCore.updateSize();
}

export function onResizeEvent(shareData: ShareData) {
    if (!shareData.viewCore || shareData.resized) {
        return;
    }
    shareData.resized = true;
    window.setTimeout(() => {
        updateSize(shareData);
    }, 300);
}

function handleVisibilityChange(shareData: ShareData) {
    shareData.isPageVisible = !document.hidden;
    if (shareData.isPageVisible && shareData.animationFrameId === null) {
        animationLoop(shareData);
    }
}

export function animationLoop(shareData: ShareData) {
    const frame = async (timestamp: number) => {
        if (shareData.viewCore && shareData.isPageVisible) {
            await draw(shareData);
            shareData.animationFrameId = requestAnimationFrame(frame);
        } else {
            shareData.animationFrameId = null;
        }
    };
    shareData.animationFrameId = requestAnimationFrame(frame);
}

export function setupVisibilityListeners(shareData: ShareData) {
    if (typeof window !== 'undefined') {
        document.addEventListener('visibilitychange', () => handleVisibilityChange(shareData));
        window.addEventListener('beforeunload', () => {
            if (shareData.animationFrameId !== null) {
                cancelAnimationFrame(shareData.animationFrameId);
                shareData.animationFrameId = null;
            }
        });
    }
}

export function setupGesture(shareData: ShareData) {
    if (shareData.gestureManager || !shareData.canvasId) {
        return
    }

    const canvas = document.getElementById(shareData.canvasId);
    if (!canvas) {
        return
    }

    console.log('setupGesture:', canvas);
    shareData.gestureManager = new GestureManager({
        onPan: (deltaX: number, deltaY: number) => {
            // console.log('Pan:', deltaX, deltaY);
            shareData.viewCore?.handlePan(deltaX, deltaY)
        },
        onZoom: (scale: number, centerX: number, centerY: number) => {
            // console.log('Zoom:', scale, centerX, centerY);
            shareData.viewCore?.handlePinch(scale, centerX, centerY)
        }
    })

    canvas.addEventListener('wheel', (e: WheelEvent) => {
        e.preventDefault();
        shareData?.gestureManager?.onWheel(e, canvas);
    }, { passive: false });
}

