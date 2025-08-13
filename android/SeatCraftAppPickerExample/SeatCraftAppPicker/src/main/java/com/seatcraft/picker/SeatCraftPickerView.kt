package com.seatcraft.picker

import android.animation.ValueAnimator
import android.view.TextureView
import android.graphics.SurfaceTexture
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.MotionEvent.INVALID_POINTER_ID
import android.view.ScaleGestureDetector
import android.view.Surface
import androidx.core.view.MotionEventCompat

class SeatCraftPickerView : TextureView, TextureView.SurfaceTextureListener {
    private var surface: Surface? = null
    private var nativePtr: Long = 0
    private var animator: ValueAnimator? = null
    private var areaMapSvgData: ByteArray? = null
    private var isScaling: Boolean = false
    private lateinit var scaleGestureDetector: ScaleGestureDetector
    private lateinit var gestureDetector: GestureDetector

    constructor(context: android.content.Context) : super(context) {
        setupSurfaceTexture()
        setupGesture()
    }

    constructor(context: android.content.Context, attrs: android.util.AttributeSet) : super(
        context,
        attrs
    ) {
        setupSurfaceTexture()
        setupGesture()
    }

    constructor(
        context: android.content.Context,
        attrs: android.util.AttributeSet,
        defStyleAttr: Int
    ) : super(context, attrs, defStyleAttr) {
        setupSurfaceTexture()
        setupGesture()
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        if (surfaceTexture != null) {
            // Surface 已经准备好了，手动调用
            if (navtiveInitialized()) {
                nativeUpdateSize()
                startIfAnimatorNeeded()
            } else {
                onSurfaceTextureAvailable(surfaceTexture!!, width, height)
            }
        } else {
            setupSurfaceTexture()
        }
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        animator?.pause()
    }

    fun setAreaMapSvgData(data: ByteArray?) {
        areaMapSvgData = data;
        if (!navtiveInitialized()) {
            return
        }
        nativeSetAreaMapSvgData(data);
    }

    private fun setupSurfaceTexture() {
        surfaceTextureListener = this
    }

    private fun startIfAnimatorNeeded() {
        if (animator != null) {
            return
        }

        animator = ValueAnimator.ofFloat(0f, 1f).apply {
            repeatCount = ValueAnimator.INFINITE
            addUpdateListener { animation ->
                draw(false)
            }
            start()
        }
    }

    private fun setupGesture() {
        scaleGestureDetector = ScaleGestureDetector(
            context,
            object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
                override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
                    isScaling = true
                    return true
                }

                override fun onScaleEnd(detector: ScaleGestureDetector) {
                    isScaling = false
                }

                override fun onScale(detector: ScaleGestureDetector): Boolean {
                    val scaleFactor = detector.scaleFactor
                    val focusX = detector.focusX
                    val focusY = detector.focusY
                    nativeUpdatePinch(scaleFactor, focusX, focusY)
                    draw(true)
                    return true
                }
            })

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
                        draw(true)
                    }
                    return true
                }

                override fun onSingleTapUp(e: MotionEvent): Boolean {
                    if (!isScaling) {
                        // 点击事件逻辑
                        return true
                    }
                    return false
                }
            })
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        scaleGestureDetector.onTouchEvent(event)
        if (!isScaling) {
            gestureDetector.onTouchEvent(event)
        }
        return true
    }

    override fun onSurfaceTextureAvailable(p0: SurfaceTexture, p1: Int, p2: Int) {
        release()
        val metrics = resources.displayMetrics
        surface = Surface(p0)
        nativePtr = nativeSetupFromSurface(surface!!, metrics.density)
        nativeUpdateSize();
        nativeSetAreaMapSvgData(areaMapSvgData)
        startIfAnimatorNeeded()
    }

    override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture, p1: Int, p2: Int) {
        if (!navtiveInitialized()) {
            return
        }
        nativeUpdateSize()
    }

    override fun onSurfaceTextureDestroyed(p0: SurfaceTexture): Boolean {
        animator?.pause()
        animator = null
        release()
        return true
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {
    }

    private fun navtiveInitialized(): Boolean {
        return nativePtr != 0L
    }

    private fun release() {
        if (surface != null) {
            surface!!.release()
            surface = null
        }
        if (!navtiveInitialized()) {
            return
        }
        nativeRelease()
    }

    private fun draw(force: Boolean) {
        if (!navtiveInitialized()) {
            return
        }
        nativeDraw(force);
    }

    private external fun nativeSetupFromSurface(surface: Surface, density: Float): Long;
    private external fun nativeSetAreaMapSvgData(data: ByteArray?);
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