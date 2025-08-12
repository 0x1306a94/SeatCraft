//
//  IOSRendererBackend.mm
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "IOSRendererBackend.h"

#import <tgfx/gpu/opengl/eagl/EAGLWindow.h>
#include <tgfx/platform/Print.h>

#import "../../../core/SeatCraftCoreApp.hpp"

#import <cmath>

namespace kk::renderer {

IOSRendererBackend::IOSRendererBackend(CAEAGLLayer *eagLayer)
    : _eagLayer(eagLayer) {
}

IOSRendererBackend::~IOSRendererBackend() {
    tgfx::PrintLog("IOSRendererBackend::~IOSRendererBackend");
}

std::shared_ptr<tgfx::Window> IOSRendererBackend::getWindow() {
    if (_window == nullptr) {
        _window = tgfx::EAGLWindow::MakeFrom(_eagLayer);
    }
    return _window;
}

int IOSRendererBackend::getWidth() {
    auto width = static_cast<int>(roundf(static_cast<float>(_eagLayer.bounds.size.width * _eagLayer.contentsScale)));
    return width;
}

int IOSRendererBackend::getHeight() {
    auto height = static_cast<int>(roundf(static_cast<float>(_eagLayer.bounds.size.height * _eagLayer.contentsScale)));
    return height;
}

float IOSRendererBackend::getDensity() {
    float contentsScale = static_cast<float>(_eagLayer.contentsScale);
    return contentsScale;
}

};  // namespace kk::renderer
