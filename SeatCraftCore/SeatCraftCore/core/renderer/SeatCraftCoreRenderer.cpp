//
//  SeatCraftCoreRenderer.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "SeatCraftCoreRenderer.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/core/Surface.h>
#include <tgfx/gpu/Device.h>
#include <tgfx/gpu/Window.h>

#include "../SeatCraftCoreApp.hpp"

#include "../drawers/ConicGradientDrawer.hpp"
#include "../drawers/GridBackgroundDrawer.hpp"
#include "../drawers/SeatMinimapDrawer.hpp"

namespace kk::renderer {
SeatCraftCoreRenderer::SeatCraftCoreRenderer(const kk::SeatCraftCoreApp *app)
    : app(app)
    , window(nullptr)
    , invalidate(true)
    , gridBackground(std::make_unique<kk::drawers::GridBackgroundDrawer>())
    , conicGradient(std::make_unique<kk::drawers::ConicGradientDrawer>())
    , minimap(std::make_unique<kk::drawers::SeatMinimapDrawer>()) {
}

SeatCraftCoreRenderer::~SeatCraftCoreRenderer() {
}

void SeatCraftCoreRenderer::initWindow() {
}

void SeatCraftCoreRenderer::updateSize() {
    if (window) {
        window->invalidSize();
    }
    invalidateContent();
}

void SeatCraftCoreRenderer::invalidateContent() {
    invalidate = true;
}

void SeatCraftCoreRenderer::draw() {
    if (window == nullptr) {
        initWindow();
    }

    if (window == nullptr) {
        return;
    }

    if (!invalidate) {
        return;
    }

    auto device = window->getDevice();
    auto context = device->lockContext();
    if (context == nullptr) {
        return;
    }

    auto surface = window->getSurface(context);
    if (surface == nullptr) {
        device->unlock();
        return;
    }

    auto canvas = surface->getCanvas();
    canvas->clear();
    
    gridBackground->draw(canvas, app);
    conicGradient->draw(canvas, app);
    minimap->draw(canvas, app);
    
    context->flushAndSubmit();
    window->present(context);

    device->unlock();

    invalidate = false;
}
};  // namespace kk::renderer
