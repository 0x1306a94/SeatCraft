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

#include "../drawers/GridBackgroundLayerTree.hpp"
#include "../drawers/SeatLayerTree.hpp"
#include "../drawers/SeatMinimapLayerTree.hpp"

namespace kk::renderer {
SeatCraftCoreRenderer::SeatCraftCoreRenderer(std::shared_ptr<kk::SeatCraftCoreApp> app, std::unique_ptr<RendererBackend> backend)
    : app(std::move(app))
    , backend(std::move(backend))
    , invalidate(true)
    , _gridLayer(std::make_unique<kk::drawers::GridBackgroundLayerTree>())
    , _seatLayer(std::make_unique<kk::drawers::SeatLayerTree>())
    , _minimapLayer(std::make_unique<kk::drawers::SeatMinimapLayerTree>()) {

    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
    updateSize();
}

SeatCraftCoreRenderer::~SeatCraftCoreRenderer() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

void SeatCraftCoreRenderer::updateSize() {
    auto window = backend->getWindow();
    if (window == nullptr) {
        return;
    }

    auto width = backend->getWidth();
    auto height = backend->getHeight();
    auto density = backend->getDensity();
    auto sizeChanged = app->updateScreen(tgfx::Size{static_cast<float>(width), static_cast<float>(height)}, density);
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

    //    if (!invalidate && !force) {
    //        return;
    //    }

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

    auto appPtr = app.get();
    _gridLayer->prepare(canvas, appPtr, force);
    _seatLayer->prepare(canvas, appPtr, force);
    _minimapLayer->prepare(canvas, appPtr, force);

    bool hasContentChanged = _gridLayer->hasContentChanged() ||
        _seatLayer->hasContentChanged() ||
        _minimapLayer->hasContentChanged();

    if (!hasContentChanged && !force) {
        device->unlock();
        return;
    }

    canvas->clear();
    canvas->save();

    _gridLayer->draw(canvas, appPtr);
    _seatLayer->draw(canvas, appPtr);
    _minimapLayer->draw(canvas, appPtr);

    canvas->restore();

    context->flushAndSubmit();
    window->present(context);

    device->unlock();

    invalidate = false;
}
};  // namespace kk::renderer
