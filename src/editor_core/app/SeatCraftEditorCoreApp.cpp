//
//  SeatCraftEditorCoreApp.cpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>

#include <tgfx/platform/Print.h>

namespace kk {
SeatCraftEditorCoreApp::SeatCraftEditorCoreApp(const tgfx::Size &boundsSize, float density)
    : _boundsSize(boundsSize)
    , _density(density) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

SeatCraftEditorCoreApp::~SeatCraftEditorCoreApp() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

const tgfx::Size &SeatCraftEditorCoreApp::getBoundsSize() const {
    return _boundsSize;
}

float SeatCraftEditorCoreApp::density() const {
    return _density;
}

/// 当前缩放比例
float SeatCraftEditorCoreApp::zoomScale() const {
    return _zoomScale;
}

/// 当前滑动偏移
const tgfx::Point &SeatCraftEditorCoreApp::contentOffset() const {
    return _contentOffset;
}

bool SeatCraftEditorCoreApp::updateBounds(const tgfx::Size &boundsSize, float density) {
    if (boundsSize.width <= 0 || boundsSize.height <= 0) {
        tgfx::PrintError("%s width or height is invalid!", __PRETTY_FUNCTION__);
        return false;
    }

    if (density < 1.0) {
        tgfx::PrintError("%s density is invalid!", __PRETTY_FUNCTION__);
        return false;
    }

    if (_boundsSize == boundsSize && _density == density) {
        return false;
    }

    _boundsSize = boundsSize;
    _density = density;
    return true;
}

bool SeatCraftEditorCoreApp::updateZoomAndOffset(float zoomScale, const tgfx::Point &contentOffset) {
    if (zoomScale == _zoomScale && contentOffset == _contentOffset) {
        return false;
    }
    _zoomScale = zoomScale;
    _contentOffset = contentOffset;
    return true;
}

}  // namespace kk
