//
//  ElasticZoomPanController.cpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/10.
//

#include "ElasticZoomPanController.hpp"

#include <algorithm>
#include <cmath>

namespace kk::ui {

ElasticZoomPanController::ElasticZoomPanController(float svgWidth, float svgHeight, float canvasWidth, float canvasHeight)
    : _svgW(svgWidth)
    , _svgH(svgHeight)
    , _canvasW(canvasWidth)
    , _canvasH(canvasHeight)
    , _minScale(1.0f)
    , _maxScale(1.0f)
    , _scale(1.0f)
    , _offsetX(0)
    , _offsetY(0)
    , _bounceFactor(0.4f)
    , _gestureActive(false)
    , _gestureStartScale(1.0f) {
    reset();
}

void ElasticZoomPanController::updateSVGSize(float newSvgWidth, float newSvgHeight) {
    _svgW = newSvgWidth;
    _svgH = newSvgHeight;
    clampScale();
    clampOffset();
}

void ElasticZoomPanController::updateCanvasSize(float newCanvasWidth, float newCanvasHeight) {
    _canvasW = newCanvasWidth;
    _canvasH = newCanvasHeight;
    clampScale();
    clampOffset();
}

void ElasticZoomPanController::setScaleRange(float minS, float maxS) {
    _minScale = std::min(minS, maxS);
    _maxScale = std::max(minS, maxS);
    clampScale();
    clampOffset();
}

void ElasticZoomPanController::setScale(float scale) {
    _scale = scale;
    clampScale();
    clampOffset();
}

void ElasticZoomPanController::setBounceFactor(float factor) {
    _bounceFactor = std::clamp(factor, 0.0f, 1.0f);
}

void ElasticZoomPanController::beginGesture() {
    _gestureActive = true;
    _gestureStartScale = _scale;
}

void ElasticZoomPanController::onPan(float deltaX, float deltaY) {
    _offsetX += applyBounceOffsetX(deltaX);
    _offsetY += applyBounceOffsetY(deltaY);
}

void ElasticZoomPanController::onZoom(float scaleFactor, float anchorX, float anchorY) {
    float newScale = _gestureStartScale * scaleFactor;

    if (newScale < _minScale) {
        newScale = _minScale - bounceDelta(_minScale - newScale);
    } else if (newScale > _maxScale) {
        newScale = _maxScale + bounceDelta(newScale - _maxScale);
    }

    float scaleChange = newScale / _scale;
    _offsetX = anchorX - (anchorX - _offsetX) * scaleChange;
    _offsetY = anchorY - (anchorY - _offsetY) * scaleChange;

    _scale = newScale;
}

void ElasticZoomPanController::endGesture() {
    _gestureActive = false;
}

void ElasticZoomPanController::setState(float s, float ox, float oy) {
    _scale = s;
    _offsetX = ox;
    _offsetY = oy;
}

float ElasticZoomPanController::getScale() const {
    return _scale;
}

float ElasticZoomPanController::getOffsetX() const {
    return _offsetX;
}

float ElasticZoomPanController::getOffsetY() const {
    return _offsetY;
}

void ElasticZoomPanController::getClampedState(float &outScale, float &outOffsetX, float &outOffsetY) const {
    outScale = std::clamp(_scale, _minScale, _maxScale);
    outOffsetX = clampOffsetX(_offsetX, outScale);
    outOffsetY = clampOffsetY(_offsetY, outScale);
}

void ElasticZoomPanController::reset() {
    float scaleFit = std::min(_canvasW / _svgW, _canvasH / _svgH);
    _scale = scaleFit;
    _offsetX = (_canvasW - _svgW * _scale) / 2.0f;
    _offsetY = (_canvasH - _svgH * _scale) / 2.0f;
    _minScale = scaleFit * 0.5f;
    _maxScale = scaleFit * 3.0f;
    _gestureActive = false;
}

float ElasticZoomPanController::bounceDelta(float over) const {
    return (over * _bounceFactor) / (over * _bounceFactor + 1.0f);
}

float ElasticZoomPanController::applyBounceOffsetX(float delta) {
    if (_svgW * _scale <= _canvasW) return 0;

    float minOffsetX = _canvasW - _svgW * _scale;
    if (_offsetX > 0 && delta > 0) {
        return bounceDelta(delta);
    }
    if (_offsetX < minOffsetX && delta < 0) {
        return bounceDelta(delta);
    }
    return delta;
}

float ElasticZoomPanController::applyBounceOffsetY(float delta) {
    if (_svgH * _scale <= _canvasH) return 0;

    float minOffsetY = _canvasH - _svgH * _scale;
    if (_offsetY > 0 && delta > 0) {
        return bounceDelta(delta);
    }
    if (_offsetY < minOffsetY && delta < 0) {
        return bounceDelta(delta);
    }
    return delta;
}

float ElasticZoomPanController::clampOffsetX(float x, float s) const {
    if (_svgW * s <= _canvasW) {
        return (_canvasW - _svgW * s) / 2.0f;
    }
    float minOffset = _canvasW - _svgW * s;
    return std::clamp(x, minOffset, 0.0f);
}

float ElasticZoomPanController::clampOffsetY(float y, float s) const {
    if (_svgH * s <= _canvasH) {
        return (_canvasH - _svgH * s) / 2.0f;
    }
    float minOffset = _canvasH - _svgH * s;
    return std::clamp(y, minOffset, 0.0f);
}

void ElasticZoomPanController::clampScale() {
    _scale = std::clamp(_scale, _minScale, _maxScale);
}

void ElasticZoomPanController::clampOffset() {
    _offsetX = clampOffsetX(_offsetX, _scale);
    _offsetY = clampOffsetY(_offsetY, _scale);
}
};  // namespace kk::ui
