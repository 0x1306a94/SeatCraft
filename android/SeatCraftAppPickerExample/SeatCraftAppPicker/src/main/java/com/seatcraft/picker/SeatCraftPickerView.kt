package com.seatcraft.picker

import android.animation.ValueAnimator
import android.view.TextureView
import android.graphics.SurfaceTexture
import android.view.Surface

class SeatCraftPickerView : TextureView, TextureView.SurfaceTextureListener {
    private var surface: Surface? = null
    private var nativePtr: Long = 0
    private var animator: ValueAnimator? = null
    private var areaMapSvgData: ByteArray? = null;

    constructor(context: android.content.Context) : super(context) {
        setupSurfaceTexture()
    }

    constructor(context: android.content.Context, attrs: android.util.AttributeSet) : super(
        context,
        attrs
    ) {
        setupSurfaceTexture()
    }

    constructor(
        context: android.content.Context,
        attrs: android.util.AttributeSet,
        defStyleAttr: Int
    ) : super(context, attrs, defStyleAttr) {
        setupSurfaceTexture()
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
            surfaceTextureListener = this
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