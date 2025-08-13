//
//  ElasticZoomPanController.hpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/10.
//

#ifndef ElasticZoomPanController_hpp
#define ElasticZoomPanController_hpp

#include <algorithm>

#include <tgfx/core/Matrix.h>
#include <tgfx/core/Point.h>
#include <tgfx/core/Rect.h>
#include <tgfx/core/Size.h>

#include "core/EdgeInsets.h"

namespace kk::ui {

class ElasticZoomPanController {
  public:
    ElasticZoomPanController() = default;

    // --- State Setters ---
    void setBounds(const tgfx::Size &bounds) {
        _bounds = bounds;
    }

    const tgfx::Size &getBounds() const {
        return _bounds;
    }

    void setContentSize(const tgfx::Size &contentSize) {
        _contentSize = contentSize;
    }

    const tgfx::Size &getContentSize() const {
        return _contentSize;
    }

    void setContentInset(const EdgeInsets &contentInset) {
        _contentInset = contentInset;
    }

    const EdgeInsets &getContentInset() const {
        return _contentInset;
    }

    void setMinimumZoomScale(float minimumZoomScale) {
        _minimumZoomScale = minimumZoomScale;
    }

    float getMinimumZoomScale() const {
        return _minimumZoomScale;
    }

    void setMaximumZoomScale(float maximumZoomScale) {
        _maximumZoomScale = maximumZoomScale;
    }

    float getMaximumZoomScale() const {
        return _maximumZoomScale;
    }

    void setZoomScale(float zoomScale) {
        _zoomScale = std::clamp(zoomScale, _minimumZoomScale, _maximumZoomScale);
        revalidateContentOffset();
    }

    float getZoomScale() const {
        return _zoomScale;
    }

    void setContentOffset(const tgfx::Point &contentOffset) {
        _contentOffset = contentOffset;
        revalidateContentOffset();
    }

    tgfx::Point getContentOffset() const {
        return _contentOffset;
    }

    // --- Transformation Matrix ---
    tgfx::Matrix getMatrix() const {
        tgfx::Matrix matrix;
        matrix.setScale(_zoomScale, _zoomScale);
        matrix.preTranslate(_contentOffset.x, _contentOffset.y);
        return matrix;
    }

    // --- Gesture Handlers (Simulating User Input) ---
    void handlePan(const tgfx::Point &delta) {
        _contentOffset.x += delta.x;
        _contentOffset.y += delta.y;
        revalidateContentOffset();
    }

    void handlePinch(float scale, const tgfx::Point &center) {
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

  private:
    // --- Internal Logic ---
    void revalidateContentOffset() {
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

    // --- Private Members ---
    float _minimumZoomScale{1.0f};
    float _maximumZoomScale{1.0f};
    float _zoomScale{1.0f};
    tgfx::Size _bounds{0.0f, 0.0f};
    tgfx::Size _contentSize{0.0f, 0.0f};
    tgfx::Point _contentOffset{0.0f, 0.0f};
    kk::EdgeInsets _contentInset{0.0f, 0.0f, 0.0f, 0.0f};
};

}  // namespace kk::ui

#endif /* ElasticZoomPanController_hpp */
