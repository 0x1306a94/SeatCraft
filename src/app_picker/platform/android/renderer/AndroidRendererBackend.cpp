
#include "AndroidRendererBackend.h"

#include <tgfx/gpu/opengl/egl/EGLWindow.h>
#include <tgfx/platform/Print.h>

#include "../../../core/SeatCraftCoreApp.hpp"

#include <cmath>

namespace kk::renderer {

AndroidRendererBackend::AndroidRendererBackend(ANativeWindow *nativeWindow)
    : _nativeWindow(nativeWindow) {
}

AndroidRendererBackend::~AndroidRendererBackend() {
    tgfx::PrintLog("AndroidRendererBackend::~AndroidRendererBackend");
}

std::shared_ptr<tgfx::Window> AndroidRendererBackend::getWindow() {
    if (_window == nullptr) {
        _window = tgfx::EGLWindow::MakeFrom(_nativeWindow);
    }
    return _window;
}

int AndroidRendererBackend::getWidth() {
    if (_nativeWindow == nullptr) {
        return 0;
    }

    auto width = ANativeWindow_getWidth(_nativeWindow);
    return width;
}

int AndroidRendererBackend::getHeight() {
    if (_nativeWindow == nullptr) {
        return 0;
    }

    auto height = ANativeWindow_getHeight(_nativeWindow);
    return height;
}

float AndroidRendererBackend::getDensity() {
    return 1.0;
}

};  // namespace kk::renderer
