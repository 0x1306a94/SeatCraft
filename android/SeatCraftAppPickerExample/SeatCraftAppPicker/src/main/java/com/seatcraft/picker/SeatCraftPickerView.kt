package com.seatcraft.picker

import android.animation.ValueAnimator
import android.content.Context
import android.content.res.AssetManager
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.Surface
import android.view.TextureView


class SeatCraftPickerView : TextureView, TextureView.SurfaceTextureListener {
    private var surface: Surface? = null
    private var nativePtr: Long = 0
    private var areaMapSvgData: ByteArray? = null
    private var areaMapSvgPath: String? = null
    private var isScaling = false
    private lateinit var scaleGestureDetector: ScaleGestureDetector
    private lateinit var gestureDetector: GestureDetector
    private var continuousAnimator: ValueAnimator? = null

    constructor(context: Context) : super(context) {
        setup()
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        setup()
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        setup()
    }

    private fun setup() {
        setupNativePtr()
        setupSurfaceTexture()
        setupGesture()
        setupContinuousAnimator()
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
                    nativeUpdatePinch(detector.scaleFactor, detector.focusX, detector.focusY)
                    return true
                }

                override fun onScaleEnd(detector: ScaleGestureDetector) {
                    isScaling = false
                }

                override fun onScale(detector: ScaleGestureDetector): Boolean {
                    nativeUpdatePinch(detector.scaleFactor, detector.focusX, detector.focusY)
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
                    }
                    return true
                }

                override fun onFling(e1: MotionEvent?, e2: MotionEvent, velocityX: Float, velocityY: Float): Boolean {
                    return true
                }

                override fun onSingleTapUp(e: MotionEvent): Boolean {
                    return !isScaling
                }
            }
        )
    }

    private fun setupContinuousAnimator() {
        continuousAnimator = ValueAnimator.ofFloat(0f, 1f).apply {
            repeatCount = ValueAnimator.INFINITE
            addUpdateListener { animation ->
                notifyNativeDraw(false)
            }
            start()
        }

    }

    fun setAreaMapSvgData(data: ByteArray?) {
        areaMapSvgData = data
        areaMapSvgPath = null
        if (nativeInitialized()) {
            nativeSetAreaMapSvgData(data)
        }
    }

    fun setAreaMapSvgPath(path: String?) {
        areaMapSvgData = null
        areaMapSvgPath = path
        if (nativeInitialized()) {
            nativeSetAreaMapSvgPath(path)
        }
    }

    fun onPause() {
        continuousAnimator?.pause()
    }

    fun onResume() {
        if (!nativeInitialized()) {
            setupNativePtr()
        }
        continuousAnimator?.resume()
    }

    fun onDestroy() {
        continuousAnimator?.cancel()
        continuousAnimator = null
        release()
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