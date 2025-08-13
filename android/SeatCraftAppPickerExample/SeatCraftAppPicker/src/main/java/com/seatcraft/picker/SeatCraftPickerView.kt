package com.seatcraft.picker

import android.content.Context
import android.content.res.AssetManager
import android.util.AttributeSet
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.Choreographer

class SeatCraftPickerView : SurfaceView, SurfaceHolder.Callback, Choreographer.FrameCallback {
    private var surface: Surface? = null
    private var nativePtr: Long = 0
    private var areaMapSvgData: ByteArray? = null
    private var isScaling = false
    private var isDrawing = false // 是否在进行持续绘制
    private lateinit var scaleGestureDetector: ScaleGestureDetector
    private lateinit var gestureDetector: GestureDetector

    constructor(context: Context) : super(context) {
        setupSurfaceHolder()
        setupGesture()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        setupSurfaceHolder()
        setupGesture()
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        setupSurfaceHolder()
        setupGesture()
    }

    private fun setupSurfaceHolder() {
        holder.addCallback(this)
        setZOrderOnTop(false)
    }

    private fun setupGesture() {
        scaleGestureDetector = ScaleGestureDetector(
            context,
            object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
                override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
                    isScaling = true
                    notifyNativeDraw(true)
                    startDrawing()
                    return true
                }

                override fun onScaleEnd(detector: ScaleGestureDetector) {
                    isScaling = false
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
                        nativeUpdatePan(-distanceX, -distanceY)
                        notifyNativeDraw(true)
                        startDrawing()
                    }
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
        if (nativeInitialized()) {
            nativeSetAreaMapSvgData(data)
        }
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        scaleGestureDetector.onTouchEvent(event)
        if (!isScaling) {
            gestureDetector.onTouchEvent(event)
        }
        return true
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        release()
        val metrics = resources.displayMetrics
        surface = holder.surface
        nativePtr = nativeSetupFromSurface(context.assets, surface!!, metrics.density)
        nativeUpdateSize()
        nativeSetAreaMapSvgData(areaMapSvgData)
        startDrawing()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        if (nativeInitialized()) {
            nativeUpdateSize()
        }
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        stopDrawing()
        release()
    }

    private fun nativeInitialized(): Boolean {
        return nativePtr != 0L
    }

    private fun release() {
        surface?.release()
        surface = null
        if (nativeInitialized()) {
            nativeRelease()
            nativePtr = 0
        }
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

    private external fun nativeSetupFromSurface(assetManager: AssetManager, surface: Surface, density: Float): Long
    private external fun nativeSetAreaMapSvgData(data: ByteArray?)
    private external fun nativeUpdateSize()
    private external fun nativeUpdatePan(x: Float, y: Float)
    private external fun nativeUpdatePinch(scale: Float, cx: Float, cy: Float)
    private external fun nativeDraw(force: Boolean)
    private external fun nativeRelease()

    companion object {
        private external fun nativeInit()

        init {
            System.loadLibrary("SeatCraftAppPicker")
            nativeInit()
        }
    }
}