//
//  ElasticZoomPanController.cpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/10.
//

#include "ElasticZoomPanController.hpp"

#include <algorithm>
#include <cmath>

#include <tgfx/platform/Print.h>

namespace kk::ui {

ElasticZoomPanController::ElasticZoomPanController() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

ElasticZoomPanController::~ElasticZoomPanController() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

void ElasticZoomPanController::setBounds(const tgfx::Size &bounds) {
    _bounds = bounds;
}

const tgfx::Size &ElasticZoomPanController::getBounds() const {
    return _bounds;
}

void ElasticZoomPanController::setContentSize(const tgfx::Size &contentSize) {
    _contentSize = contentSize;
}

const tgfx::Size &ElasticZoomPanController::getContentSize() const {
    return _contentSize;
}

void ElasticZoomPanController::setContentInset(const EdgeInsets &contentInset) {
    _contentInset = contentInset;
}

const EdgeInsets &ElasticZoomPanController::getContentInset() const {
    return _contentInset;
}

void ElasticZoomPanController::setMinimumZoomScale(float minimumZoomScale) {
    _minimumZoomScale = minimumZoomScale;
}

float ElasticZoomPanController::getMinimumZoomScale() const {
    return _minimumZoomScale;
}

void ElasticZoomPanController::setMaximumZoomScale(float maximumZoomScale) {
    _maximumZoomScale = maximumZoomScale;
}

float ElasticZoomPanController::getMaximumZoomScale() const {
    return _maximumZoomScale;
}

void ElasticZoomPanController::setZoomScale(float zoomScale) {
    _zoomScale = std::clamp(zoomScale, _minimumZoomScale, _maximumZoomScale);
    revalidateContentOffset();
}

float ElasticZoomPanController::getZoomScale() const {
    return _zoomScale;
}

void ElasticZoomPanController::setContentOffset(const tgfx::Point &contentOffset) {
    _contentOffset = contentOffset;
    revalidateContentOffset();
}

tgfx::Point ElasticZoomPanController::getContentOffset() const {
    return _contentOffset;
}

// --- Transformation Matrix ---
tgfx::Matrix ElasticZoomPanController::getMatrix() const {
    tgfx::Matrix matrix;
    matrix.setScale(_zoomScale, _zoomScale);
    matrix.preTranslate(_contentOffset.x, _contentOffset.y);
    return matrix;
}

// --- Gesture Handlers (Simulating User Input) ---
void ElasticZoomPanController::handlePan(const tgfx::Point &delta) {
    _contentOffset.x += delta.x;
    _contentOffset.y += delta.y;
    revalidateContentOffset();
}

void ElasticZoomPanController::handlePinch(float scale, const tgfx::Point &center) {
    float contentPointX = (center.x - _contentOffset.x) / _zoomScale;
    float contentPointY = (center.y - _contentOffset.y) / _zoomScale;

    float newZoomScale = _zoomScale * scale;
    _zoomScale = std::clamp(newZoomScale, _minimumZoomScale, _maximumZoomScale);

    float newOffsetX = center.x - contentPointX * _zoomScale;
    float newOffsetY = center.y - contentPointY * _zoomScale;

    _contentOffset.x = newOffsetX;
    _contentOffset.y = newOffsetY;

    revalidateContentOffset();
}

// --- Internal Logic ---
void ElasticZoomPanController::revalidateContentOffset() {
    const float scaledWidth = _contentSize.width * _zoomScale;
    const float scaledHeight = _contentSize.height * _zoomScale;

    // 考虑 contentInset 后的视图有效边界
    const float effectiveBoundsWidth = _bounds.width - _contentInset.left - _contentInset.right;
    const float effectiveBoundsHeight = _bounds.height - _contentInset.top - _contentInset.bottom;

    // 计算水平方向的边界
    float minX, maxX;
    if (scaledWidth < effectiveBoundsWidth) {
        // 内容小于有效边界，居中对齐，并考虑 left/right inset
        minX = (_bounds.width - scaledWidth) / 2.0f;
        maxX = minX;
    } else {
        // 内容大于有效边界，允许滚动
        // 注意：平移的边界应该加上 contentInset
        minX = -scaledWidth + effectiveBoundsWidth + _contentInset.left;
        maxX = _contentInset.left;
    }

    // 计算垂直方向的边界
    float minY, maxY;
    if (scaledHeight < effectiveBoundsHeight) {
        // 内容小于有效边界，居中对齐，并考虑 top/bottom inset
        minY = (_bounds.height - scaledHeight) / 2.0f;
        maxY = minY;
    } else {
        // 内容大于有效边界，允许滚动
        minY = -scaledHeight + effectiveBoundsHeight + _contentInset.top;
        maxY = _contentInset.top;
    }

    _contentOffset.x = std::clamp(_contentOffset.x, minX, maxX);
    _contentOffset.y = std::clamp(_contentOffset.y, minY, maxY);
}
};  // namespace kk::ui
