package com.seatcraft.picker

import android.view.TextureView
import android.graphics.SurfaceTexture
import android.view.Surface

class SeatCraftPickerView : TextureView, TextureView.SurfaceTextureListener {
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
            onSurfaceTextureAvailable(surfaceTexture!!, width, height)
        } else {
            surfaceTextureListener = this
        }
    }

    private fun setupSurfaceTexture() {
        surfaceTextureListener = this
    }

    override fun onSurfaceTextureAvailable(p0: SurfaceTexture, p1: Int, p2: Int) {
        release()
        val metrics = resources.displayMetrics
        surface = Surface(p0)
        nativePtr = setupFromSurface(surface!!, metrics.density)
    }

    override fun onSurfaceTextureSizeChanged(p0: SurfaceTexture, p1: Int, p2: Int) {
        nativeUpdateSize()
    }

    override fun onSurfaceTextureDestroyed(p0: SurfaceTexture): Boolean {
        return true
    }

    override fun onSurfaceTextureUpdated(p0: SurfaceTexture) {
    }

    fun release() {
        if (surface != null) {
            surface!!.release()
            surface = null
        }
        nativeRelease()
    }

    fun draw(force: Boolean) {
        nativeDraw(force);
    }

    private external fun nativeUpdateSize()
    private external fun nativeDraw(force: Boolean)
    private external fun nativeRelease()
    private var surface: Surface? = null
    private var nativePtr: Long = 0

    companion object {
        private external fun nativeInit()
        private external fun setupFromSurface(surface: Surface, density: Float): Long

        init {
            System.loadLibrary("SeatCraftAppPicker")
            nativeInit()
        }
    }
}