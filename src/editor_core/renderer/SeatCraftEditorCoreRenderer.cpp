//
//  SeatCraftEditorCoreRenderer.cpp
//  SeatCraftEditorCore
//
//  Created by king on 2025/8/18.
//

#include <SeatCraftEditorCore/renderer/SeatCraftEditorCoreRenderer.hpp>

#include <SeatCraft/common/common_macro.h>
#include <SeatCraftEditorCore/renderer/RendererBackend.hpp>

#include <tgfx/core/Canvas.h>
#include <tgfx/core/Surface.h>
#include <tgfx/gpu/Device.h>
#include <tgfx/gpu/Window.h>
#include <tgfx/platform/Print.h>

namespace kk::renderer {
SeatCraftEditorCoreRenderer::SeatCraftEditorCoreRenderer(std::shared_ptr<RendererBackend> backend)
    : _backend(std::move(backend))
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

    // auto width = _backend->getWidth();
    // auto height = _backend->getHeight();
    // auto density = _backend->getDensity();
    // return sizeChanged;
    return false;
}

void SeatCraftEditorCoreRenderer::invalidateContent() {
    _invalidate = true;
}

void SeatCraftEditorCoreRenderer::draw(bool force) {
    UNUSED_PARAM(force);
    if (_backend == nullptr) {
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

    if (!force && !_invalidate) {
        device->unlock();
        return;
    }

    canvas->clear(tgfx::Color::White());
    canvas->save();

    canvas->restore();

    context->flushAndSubmit();
    window->present(context);

    device->unlock();

    _invalidate = false;
}

};  // namespace kk::renderer
