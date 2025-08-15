package com.seatcraft.picker

import android.content.Context
import android.content.res.AssetManager
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.Choreographer
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.Surface
import android.view.TextureView


class SeatCraftPickerView : TextureView, TextureView.SurfaceTextureListener, Choreographer.FrameCallback {
    private var surface: Surface? = null
    private var nativePtr: Long = 0
    private var areaMapSvgData: ByteArray? = null
    private var areaMapSvgPath: String? = null
    private var isScaling = false
    private var isDrawing = false // 是否在进行持续绘制
    private lateinit var scaleGestureDetector: ScaleGestureDetector
    private lateinit var gestureDetector: GestureDetector

    constructor(context: Context) : super(context) {
        setupNativePtr()
        setupSurfaceTexture()
        setupGesture()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        setupNativePtr()
        setupSurfaceTexture()
        setupGesture()
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        setupNativePtr()
        setupSurfaceTexture()
        setupGesture()
    }

    private fun setupSurfaceTexture() {
        surfaceTextureListener = this
    }

    private fun setupNativePtr() {
        val metrics = resources.displayMetrics
        nativePtr = nativeCreate(context.assets, metrics.density)
    }

    private fun setupGesture() {
        scaleGestureDetector = ScaleGestureDetector(
            context,
            object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
                override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
                    isScaling = true
                    stopDrawing()
                    nativeUpdatePinch(detector.scaleFactor, detector.focusX, detector.focusY)
                    notifyNativeDraw(true)
                    return true
                }

                override fun onScaleEnd(detector: ScaleGestureDetector) {
                    isScaling = false
                    startDrawing()
                }

                override fun onScale(detector: ScaleGestureDetector): Boolean {
                    nativeUpdatePinch(detector.scaleFactor, detector.focusX, detector.focusY)
                    notifyNativeDraw(true)
                    return true
                }
            }
        )

        gestureDetector = GestureDetector(
            context,
            object : GestureDetector.SimpleOnGestureListener() {
                override fun onScroll(
                    e1: MotionEvent?,
                    e2: MotionEvent,
                    distanceX: Float,
                    distanceY: Float
                ): Boolean {
                    if (!isScaling) {
                        stopDrawing()
                        nativeUpdatePan(-distanceX, -distanceY)
                        notifyNativeDraw(true)
                    }
                    return true
                }

                override fun onFling(e1: MotionEvent?, e2: MotionEvent, velocityX: Float, velocityY: Float): Boolean {
                    startDrawing()
                    return true
                }

                override fun onSingleTapUp(e: MotionEvent): Boolean {
                    startDrawing()
                    return !isScaling
                }
            }
        )
    }

    fun setAreaMapSvgData(data: ByteArray?) {
        areaMapSvgData = data
        areaMapSvgPath = null
        if (nativeInitialized()) {
            nativeSetAreaMapSvgData(data)
            nativeDraw(true)
        }
    }

    fun setAreaMapSvgPath(path: String?) {
        areaMapSvgData = null
        areaMapSvgPath = path
        if (nativeInitialized()) {
            nativeSetAreaMapSvgPath(path)
            nativeDraw(true)
        }
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        scaleGestureDetector.onTouchEvent(event)
        if (!isScaling) {
            gestureDetector.onTouchEvent(event)
        }
        return true
    }


    override fun onSurfaceTextureAvailable(p0: SurfaceTexture, p1: Int, p2: Int) {
        val metrics = resources.displayMetrics
        surface = Surface(p0)
        nativeUpdateSurface(surface!!, metrics.density)
        nativeUpdateSize()
        nativeDraw(true)
        startDrawing()
    }

    override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture, p1: Int, p2: Int) {
        if (!nativeInitialized()) {
            return
        }
        nativeUpdateSize()
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {
    }

    override fun onSurfaceTextureDestroyed(p0: SurfaceTexture): Boolean {
        stopDrawing()
        val metrics = resources.displayMetrics
        nativeUpdateSurface(null, metrics.density)
        return true
    }


    private fun nativeInitialized(): Boolean {
        return nativePtr != 0L
    }

    private fun release() {
        surface?.release()
        surface = null
        if (nativeInitialized()) {
            nativeRelease()
            nativePtr = 0L
        }
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        if (!nativeInitialized()) {
            setupNativePtr()
        }
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        release()
    }

    /** 启动持续绘制（直到 stopDrawing 被调用） */
    private fun startDrawing() {
        if (!isDrawing) {
            isDrawing = true
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    /** 停止持续绘制 */
    private fun stopDrawing() {
        if (isDrawing) {
            isDrawing = false
            Choreographer.getInstance().removeFrameCallback(this)
        }
    }

    /** 每帧回调 */
    override fun doFrame(frameTimeNanos: Long) {
        notifyNativeDraw(false);
        if (isDrawing) {
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    private fun notifyNativeDraw(force: Boolean) {
        if (nativeInitialized()) {
            nativeDraw(force)
        }
    }

    private external fun nativeSetAreaMapSvgData(data: ByteArray?)
    private external fun nativeSetAreaMapSvgPath(path: String?)
    private external fun nativeUpdateSize()
    private external fun nativeUpdatePan(x: Float, y: Float)
    private external fun nativeUpdatePinch(scale: Float, cx: Float, cy: Float)
    private external fun nativeUpdateSurface(surface: Surface?, density: Float)
    private external fun nativeDraw(force: Boolean)
    private external fun nativeCreate(assetManager: AssetManager, density: Float): Long
    private external fun nativeRelease()

    companion object {
        private external fun nativeInit()

        init {
            System.loadLibrary("SeatCraftAppPicker")
            nativeInit()
        }
    }
}