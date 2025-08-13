//
// Created by KK on 2025/8/12.
//

#include "SeatCraftPickerView.h"

#include "core/SeatCraftCoreApp.hpp"
#include "core/renderer/SeatCraftCoreRenderer.hpp"
#include "core/ui/ElasticZoomPanController.hpp"

#include <tgfx/core/Stream.h>
#include <tgfx/platform/Print.h>

namespace kk::ui {
SeatCraftPickerView::SeatCraftPickerView(std::shared_ptr<kk::SeatCraftCoreApp> app, std::shared_ptr<kk::renderer::SeatCraftCoreRenderer> renderer)
    : _app(std::move(app)), _renderer(std::move(renderer)), _zoomPanController(std::make_shared<kk::ui::ElasticZoomPanController>()) {
    tgfx::PrintLog("SeatCraftPickerView::SeatCraftPickerView()");
}

SeatCraftPickerView::~SeatCraftPickerView() {
    tgfx::PrintLog("SeatCraftPickerView::~SeatCraftPickerView()");
}

std::shared_ptr<kk::SeatCraftCoreApp> SeatCraftPickerView::getApp() {
    return _app;
}
void SeatCraftPickerView::updateSize() {
    _renderer->updateSize();
    _zoomPanController->setBounds(_app->getBoundsSize());
    updateContentSize();
}

void SeatCraftPickerView::draw(bool force) {
    _renderer->draw(force);
}

void SeatCraftPickerView::updateAreaSvgData(std::unique_ptr<tgfx::Stream> data) {
    _app->updateAreaSvgData(std::move(data));

    auto svgSize = _app->getOriginSize();
    float maxWidth = 1000.0f;
    float scale = 1.0f;
    if (svgSize.width > 0.0f && svgSize.width > maxWidth) {
        scale = maxWidth / svgSize.width;
    }
    _svgScale = scale;
    updateContentSize();
}

void SeatCraftPickerView::handlePan(const tgfx::Point &delta) {
    _zoomPanController->handlePan(delta);
}

void SeatCraftPickerView::handlePinch(float scale, const tgfx::Point &center) {
    _zoomPanController->handlePinch(scale, center);
}

void SeatCraftPickerView::updateContentSize() {
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
    _renderer->draw(true);
}

void SeatCraftPickerView::updateMaxMinZoomScalesForCurrentBounds() {
    float boundsWidth = _zoomPanController->getBounds().width;
    float contentWidth = _zoomPanController->getContentSize().width;

    if (boundsWidth == 0.0 || contentWidth == 0.0) {

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

    _zoomScale9 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 9.0f);
    _zoomScale18 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 18.0f);
    _zoomScale30 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 36.0f);
    _zoomScale50 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 50.0f);

    float minimumZoomScale = boundsWidth / contentWidth;
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

void SeatCraftPickerView::updateZoomPanControllerState() {
    auto currentZoom = _zoomPanController->getZoomScale();
    auto currentOffset = _zoomPanController->getContentOffset();
    auto changed = _app->updateZoomAndOffset(currentZoom, currentOffset);
    if (changed) {
        _renderer->invalidateContent();
    }
}
}  // namespace kk::ui