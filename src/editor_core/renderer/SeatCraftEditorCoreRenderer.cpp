//
//  SeatCraftEditorCoreRenderer.cpp
//  SeatCraftEditorCore
//
//  Created by king on 2025/8/18.
//

#include <SeatCraftEditorCore/renderer/SeatCraftEditorCoreRenderer.hpp>

#include "drawers/GridBackgroundLayerTree.hpp"

#include <SeatCraft/common/common_macro.h>
#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>
#include <SeatCraftEditorCore/renderer/RendererBackend.hpp>

#include <tgfx/core/Canvas.h>
#include <tgfx/core/Surface.h>
#include <tgfx/gpu/Device.h>
#include <tgfx/gpu/Window.h>
#include <tgfx/platform/Print.h>

namespace kk::renderer {
SeatCraftEditorCoreRenderer::SeatCraftEditorCoreRenderer(std::shared_ptr<kk::SeatCraftEditorCoreApp> app, std::shared_ptr<RendererBackend> backend)
    : _app(std::move(app))
    , _backend(std::move(backend))
    , _gridLayer(std::make_unique<kk::drawers::GridBackgroundLayerTree>())
    , _invalidate(true) {

    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
    updateSize();
}

SeatCraftEditorCoreRenderer::~SeatCraftEditorCoreRenderer() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

std::shared_ptr<RendererBackend> SeatCraftEditorCoreRenderer::getRendererBackend() {
    return _backend;
}

void SeatCraftEditorCoreRenderer::replaceBackend(std::shared_ptr<RendererBackend> backend) {
    _backend = std::move(backend);
}

bool SeatCraftEditorCoreRenderer::updateSize() {
    if (_backend == nullptr) {
        return false;
    }

    auto window = _backend->getWindow();
    if (window == nullptr) {
        return false;
    }

    auto width = _backend->getWidth();
    auto height = _backend->getHeight();
    auto density = _backend->getDensity();
    auto sizeChanged = _app->updateBounds(tgfx::Size{static_cast<float>(width), static_cast<float>(height)}, density);
    if (sizeChanged) {
        window->invalidSize();
    }
    return sizeChanged;
}

void SeatCraftEditorCoreRenderer::invalidateContent() {
    _invalidate = true;
}

void SeatCraftEditorCoreRenderer::draw(bool force) {
    UNUSED_PARAM(force);
    if (_backend == nullptr) {
        return;
    }

    if (_app == nullptr) {
        return;
    }

    auto window = _backend->getWindow();
    if (window == nullptr) {
        return;
    }

    auto device = window->getDevice();
    if (device == nullptr) {
        return;
    }

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
    if (canvas == nullptr) {
        device->unlock();
        return;
    }

    auto appPtr = _app.get();
    _gridLayer->prepare(canvas, appPtr, force);

    bool hasContentChanged = _gridLayer->hasContentChanged();

    if (!hasContentChanged && !force && !_invalidate) {
        device->unlock();
        return;
    }

    canvas->clear();
    canvas->save();

    _gridLayer->draw(canvas, appPtr);

    canvas->restore();

    context->flushAndSubmit();
    window->present(context);

    device->unlock();

    _invalidate = false;
}

};  // namespace kk::renderer
