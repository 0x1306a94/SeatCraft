
#include "WebRendererBackend.h"

#include <emscripten/bind.h>
#include <tgfx/gpu/opengl/webgl/WebGLWindow.h>
#include <tgfx/platform/Print.h>

#include "core/SeatCraftCoreApp.hpp"

#include <cmath>

namespace kk::renderer {

WebRendererBackend::WebRendererBackend(const std::string &canvasId, float density)
    : _canvasId(canvasId)
    , _density(density) {
    tgfx::PrintLog("WebRendererBackend::WebRendererBackend()");
}

WebRendererBackend::~WebRendererBackend() {
    tgfx::PrintLog("WebRendererBackend::~WebRendererBackend");
}

std::shared_ptr<tgfx::Window> WebRendererBackend::getWindow() {
    if (_window == nullptr) {
        _window = tgfx::WebGLWindow::MakeFrom(_canvasId);
    }
    return _window;
}

int WebRendererBackend::getWidth() {
    if (_canvasId.empty()) {
        return 0;
    }

    int width = 0;
    emscripten_get_canvas_element_size(_canvasId.c_str(), &width, nullptr);
    return width;
}

int WebRendererBackend::getHeight() {
    if (_canvasId.empty()) {
        return 0;
    }

    int height = 0;
    emscripten_get_canvas_element_size(_canvasId.c_str(), nullptr, &height);
    return height;
}

float WebRendererBackend::getDensity() {
    return _density;
}

};  // namespace kk::renderer
