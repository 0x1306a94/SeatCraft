//
//  SeatCraftViewCore.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/14.
//

#include "SeatCraftViewCore.hpp"

#include <SeatCraft/common/common_macro.h>

#include "core/SeatCraftCoreApp.hpp"
#include "core/renderer/RendererBackend.hpp"
#include "core/renderer/SeatCraftCoreRenderer.hpp"
#include "core/thread/UIThreadScheduler.h"
#include "core/thread/WorkThread.h"
#include "core/ui/ElasticZoomPanController.hpp"

#include <tgfx/core/Stream.h>
#include <tgfx/platform/Print.h>

namespace kk::ui {
SeatCraftViewCore::SeatCraftViewCore(std::shared_ptr<kk::SeatCraftCoreApp> app,
                                     std::unique_ptr<kk::renderer::RendererBackend> backend,
                                     std::unique_ptr<kk::ui::ElasticZoomPanController> zoomPanController,
                                     std::shared_ptr<kk::thread::UIThreadScheduler> uiScheduler)
    : _app(app)
    , _renderer(std::make_unique<kk::renderer::SeatCraftCoreRenderer>(app, std::move(backend)))
    , _zoomPanController(std::move(zoomPanController))
    , _uiScheduler(std::move(uiScheduler)) {
    _workThread = std::make_unique<kk::thread::WorkThread>();
    tgfx::PrintLog("SeatCraftViewCore::SeatCraftViewCore()");
}

SeatCraftViewCore::~SeatCraftViewCore() {
    tgfx::PrintLog("SeatCraftViewCore::~SeatCraftViewCore()");
}

void SeatCraftViewCore::replaceBackend(std::unique_ptr<kk::renderer::RendererBackend> backend) {
    _renderer->replaceBackend(std::move(backend));
}

std::shared_ptr<kk::SeatCraftCoreApp> SeatCraftViewCore::getApp() const {
    return _app;
}

bool SeatCraftViewCore::updateSize() {
    auto sizeChanged = _renderer->updateSize();
    _zoomPanController->setBounds(_app->getBoundsSize());
    if (sizeChanged) {
        updateContentSize();
        return true;
    }
    return false;
}

void SeatCraftViewCore::setMaxWidth(float maxWidth) {
    _maxWidth = maxWidth;
    updateSvgScale();
    updateContentSize();
}

float SeatCraftViewCore::getMaxWidth() const {
    return _maxWidth;
}

float SeatCraftViewCore::getSvgScale() const {
    return _svgScale;
}

void SeatCraftViewCore::invalidateContent() {
    _renderer->invalidateContent();
}

void SeatCraftViewCore::draw(bool force) {
    _renderer->draw(force);
}

void SeatCraftViewCore::updateAreaAvailable() {
    updateSvgScale();
    updateContentSize();
}
void SeatCraftViewCore::updateSeatStatusAvailable() {
    _renderer->invalidateContent();
}

// 手势处理方法，由平台层调用
void SeatCraftViewCore::handlePan(const tgfx::Point &delta) {
    _zoomPanController->handlePan(delta);
    updateZoomPanControllerState();
}

void SeatCraftViewCore::handlePinch(float scale, const tgfx::Point &center) {
    _zoomPanController->handlePinch(scale, center);
    updateZoomPanControllerState();
}

float SeatCraftViewCore::getMinimumZoomScale() const {
    return _zoomPanController->getMinimumZoomScale();
}

float SeatCraftViewCore::getMaximumZoomScale() const {
    return _zoomPanController->getMaximumZoomScale();
}

float SeatCraftViewCore::getZoomScale() const {
    return _zoomPanController->getZoomScale();
}

void SeatCraftViewCore::setZoomScale(float zoomScale) {
    _zoomPanController->setZoomScale(zoomScale);
    updateZoomPanControllerState();
}

const tgfx::Point &SeatCraftViewCore::getContentOffset() const {
    return _zoomPanController->getContentOffset();
}

void SeatCraftViewCore::setContentOffset(const tgfx::Point &contentOffset) {
    _zoomPanController->setContentOffset(contentOffset);
    updateZoomPanControllerState();
}

void SeatCraftViewCore::postWork(std::function<void()> task) {
    _workThread->post(std::move(task));
}

void SeatCraftViewCore::postUI(std::function<void()> task) {
    if (_uiScheduler) {
        _uiScheduler->schedule(std::move(task));
    }
}

void SeatCraftViewCore::updateSvgScale() {
    auto svgSize = _app->getOriginSize();
    if (!svgSize.isEmpty() && svgSize.width > _maxWidth) {
        _svgScale = _maxWidth / svgSize.width;
    } else {
        _svgScale = 1.0f;
    }
}

void SeatCraftViewCore::updateContentSize() {
    auto svgSize = _app->getOriginSize();
    auto density = _app->density();
    if (svgSize.isEmpty()) {
        _zoomPanController->setContentSize({});
        _app->updateContentSize({});
    } else {
        tgfx::Size contentSize{
            static_cast<float>(svgSize.width * _svgScale * density),
            static_cast<float>(svgSize.height * _svgScale * density),
        };
        _zoomPanController->setContentSize(contentSize);
        _app->updateContentSize(contentSize);
    }
    updateMaxMinZoomScalesForCurrentBounds();
}

void SeatCraftViewCore::updateMaxMinZoomScalesForCurrentBounds() {
    auto boundsSize = _zoomPanController->getBounds();
    auto contentSize = _zoomPanController->getContentSize();

    if (boundsSize.isEmpty() || contentSize.isEmpty()) {
        _zoomPanController->setMinimumZoomScale(1.0f);
        _zoomPanController->setMaximumZoomScale(1.0f);
        _zoomPanController->setZoomScale(1.0f);

        _zoomScale9 = 1.0f;
        _zoomScale18 = 1.0f;
        _zoomScale30 = 1.0f;
        _zoomScale50 = 1.0f;

        updateZoomPanControllerState();
        return;
    }

    float svgScale = _svgScale;
    float svgModelScale = _svgModelScale;

    float minimumBounds = std::min(boundsSize.width, boundsSize.height);

    _zoomScale9 = minimumBounds / ((svgScale * 36.0f) / svgModelScale * 9.0f);
    _zoomScale18 = minimumBounds / ((svgScale * 36.0f) / svgModelScale * 18.0f);
    _zoomScale30 = minimumBounds / ((svgScale * 36.0f) / svgModelScale * 30.0f);
    _zoomScale50 = minimumBounds / ((svgScale * 36.0f) / svgModelScale * 50.0f);

    float minimumZoomScale = std::min(boundsSize.width / contentSize.width, boundsSize.height / contentSize.height);
    float baseScale = 1.0f / (svgScale / svgModelScale);
    float maximumZoomScale = _zoomScale9;
    if (maximumZoomScale < baseScale) {
        maximumZoomScale = baseScale;
    }

    _zoomPanController->setMinimumZoomScale(static_cast<float>(minimumZoomScale));
    _zoomPanController->setMaximumZoomScale(static_cast<float>(maximumZoomScale));
    _zoomPanController->setZoomScale(static_cast<float>(minimumZoomScale));

    updateZoomPanControllerState();
}

void SeatCraftViewCore::updateZoomPanControllerState() {
    auto currentZoom = _zoomPanController->getZoomScale();
    auto currentOffset = _zoomPanController->getContentOffset();
    auto changed = _app->updateZoomAndOffset(currentZoom, currentOffset);
    if (changed) {
        _renderer->invalidateContent();
    }
}

}  // namespace kk::ui
