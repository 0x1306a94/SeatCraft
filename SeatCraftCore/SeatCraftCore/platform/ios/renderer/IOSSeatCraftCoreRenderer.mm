//
//  IOSSeatCraftCoreRenderer.mm
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "IOSSeatCraftCoreRenderer.h"

#import <tgfx/gpu/opengl/eagl/EAGLWindow.h>

#import "../../../core/SeatCraftCoreApp.hpp"

namespace kk::renderer {

IOSSeatCraftCoreRenderer::IOSSeatCraftCoreRenderer(const kk::SeatCraftCoreApp *app, CAEAGLLayer *eagLayer)
    : SeatCraftCoreRenderer(app)
    , eagLayer(eagLayer) {
}

IOSSeatCraftCoreRenderer::~IOSSeatCraftCoreRenderer() {
}

void IOSSeatCraftCoreRenderer::initWindow() {
    if (window) {
        return;
    }

    window = tgfx::EAGLWindow::MakeFrom(eagLayer);
}

};  // namespace kk::renderer
