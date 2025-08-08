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
#include <tgfx/platform/Print.h>

#include "RendererBackend.hpp"

#include "../SeatCraftCoreApp.hpp"

#include "../drawers/ConicGradientDrawer.hpp"
#include "../drawers/GridBackgroundDrawer.hpp"
#include "../drawers/SeatMinimapDrawer.hpp"

namespace kk::renderer {
SeatCraftCoreRenderer::SeatCraftCoreRenderer(std::shared_ptr<kk::SeatCraftCoreApp> app, std::unique_ptr<RendererBackend> backend)
    : app(app)
    , backend(std::move(backend))
    , invalidate(true)
    , gridBackground(std::make_unique<kk::drawers::GridBackgroundDrawer>())
    , conicGradient(std::make_unique<kk::drawers::ConicGradientDrawer>())
    , minimap(std::make_unique<kk::drawers::SeatMinimapDrawer>()) {

    updateSize();
}

SeatCraftCoreRenderer::~SeatCraftCoreRenderer() {
    tgfx::PrintLog("SeatCraftCoreRenderer::~SeatCraftCoreRenderer");
}

void SeatCraftCoreRenderer::updateSize() {
    auto window = backend->getWindow();
    if (window == nullptr) {
        return;
    }

    auto width = backend->getWidth();
    auto height = backend->getHeight();
    auto density = backend->getDensity();
    auto sizeChanged = app->updateScreen(width, height, density);
    if (sizeChanged) {
        window->invalidSize();
        invalidateContent();
    }
}

void SeatCraftCoreRenderer::invalidateContent() {
    invalidate = true;
}

void SeatCraftCoreRenderer::draw(bool force) {
    auto window = backend->getWindow();

    if (window == nullptr) {
        return;
    }

    if (!invalidate && !force) {
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
    canvas->save();

    auto appPtr = app.get();

    gridBackground->draw(canvas, appPtr);
    conicGradient->draw(canvas, appPtr);
    minimap->draw(canvas, appPtr);

    canvas->restore();

    context->flushAndSubmit();
    window->present(context);

    device->unlock();

    invalidate = false;
}
};  // namespace kk::renderer
