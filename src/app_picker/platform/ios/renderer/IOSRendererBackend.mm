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
    : eagLayer(eagLayer) {
}

IOSRendererBackend::~IOSRendererBackend() {
    tgfx::PrintLog("IOSRendererBackend::~IOSRendererBackend");
}

std::shared_ptr<tgfx::Window> IOSRendererBackend::getWindow() {
    if (window_ == nullptr) {
        window_ = tgfx::EAGLWindow::MakeFrom(eagLayer);
    }
    return window_;
}

int IOSRendererBackend::getWidth() {
    auto width = static_cast<int>(roundf(static_cast<float>(eagLayer.bounds.size.width * eagLayer.contentsScale)));
    return width;
}

int IOSRendererBackend::getHeight() {
    auto height = static_cast<int>(roundf(static_cast<float>(eagLayer.bounds.size.height * eagLayer.contentsScale)));
    return height;
}

float IOSRendererBackend::getDensity() {
    float contentsScale = static_cast<float>(eagLayer.contentsScale);
    return contentsScale;
}

};  // namespace kk::renderer
