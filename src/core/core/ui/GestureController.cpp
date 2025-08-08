//
//  GestureController.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#include "GestureController.hpp"

#include <algorithm>

namespace kk::ui {
GestureController::GestureController()
    : minimumZoomScale(1.0f)
    , maximumZoomScale(1.0f)
    , zoomScale(1.0f)
    , currentZoom(1.0f) {
}

void GestureController::setBoundsSize(const tgfx::Size &size) {
    boundsSize = size;
    // 尺寸改变时，需要重新计算边界并约束
    clampContentOffset();
}

void GestureController::setContentSize(const tgfx::Size &size) {
    contentSize = size;
    clampContentOffset();
}

void GestureController::setZoomScales(float minScale, float maxScale) {
    minimumZoomScale = minScale;
    maximumZoomScale = maxScale;
    // 缩放范围改变时，重新约束当前缩放
    zoomScale = std::min(std::max(zoomScale, minimumZoomScale), maximumZoomScale);
    clampContentOffset();
}

void GestureController::setZoomScale(float scale) {
    float newScale = std::min(std::max(scale, minimumZoomScale), maximumZoomScale);
    if (newScale == zoomScale) {
        return;
    }

    // 计算缩放中心，这里我们假设以画布中心为缩放中心
    tgfx::Point center = {boundsSize.width * 0.5f, boundsSize.height * 0.5f};

    // 计算新的偏移量，以保持画布中心位置不变
    tgfx::Point offset;
    offset.x = (contentOffset.x - center.x) * newScale / zoomScale + center.x;
    offset.y = (contentOffset.y - center.y) * newScale / zoomScale + center.y;

    zoomScale = newScale;
    contentOffset = offset;

    // 重新约束偏移量
    clampContentOffset();
}

void GestureController::pan(const tgfx::Point &translation, bool isBegan) {
    if (isBegan) {
        currentPanOffset = translation;
    }

    tgfx::Point newTranslation = translation - currentPanOffset;
    contentOffset += newTranslation;
    currentPanOffset = translation;

    clampContentOffset();
}

void GestureController::pinch(float scale, const tgfx::Point &center, bool isBegan) {
    if (isBegan) {
        currentZoom = zoomScale;
        currentPinchOffset = contentOffset;
        pinchCenter = center;
    }

    float newZoomScale = std::min(std::max(currentZoom * scale, minimumZoomScale), maximumZoomScale);

    // 计算新的偏移量，确保缩放以手势中心进行
    tgfx::Point offset;
    offset.x = (currentPinchOffset.x - pinchCenter.x) * newZoomScale / currentZoom + pinchCenter.x;
    offset.y = (currentPinchOffset.y - pinchCenter.y) * newZoomScale / currentZoom + pinchCenter.y;

    zoomScale = newZoomScale;
    contentOffset = offset;

    clampContentOffset();
}

// 核心边界约束逻辑
void GestureController::clampContentOffset() {
    float scaledContentWidth = contentSize.width * zoomScale;
    float scaledContentHeight = contentSize.height * zoomScale;

    float minOffsetX, maxOffsetX, minOffsetY, maxOffsetY;

    if (scaledContentWidth < boundsSize.width) {
        minOffsetX = maxOffsetX = (boundsSize.width - scaledContentWidth) / 2.0f;
    } else {
        minOffsetX = boundsSize.width - scaledContentWidth;
        maxOffsetX = 0.0f;
    }

    if (scaledContentHeight < boundsSize.height) {
        minOffsetY = maxOffsetY = (boundsSize.height - scaledContentHeight) / 2.0f;
    } else {
        minOffsetY = boundsSize.height - scaledContentHeight;
        maxOffsetY = 0.0f;
    }

    contentOffset.x = std::min(std::max(contentOffset.x, minOffsetX), maxOffsetX);
    contentOffset.y = std::min(std::max(contentOffset.y, minOffsetY), maxOffsetY);
}

float GestureController::getZoomScale() const {
    return zoomScale;
}

tgfx::Point GestureController::getContentOffset() const {
    return contentOffset;
}

};  // namespace kk::ui
