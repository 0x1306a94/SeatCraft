//
//  SeatCraftEditorCoreApp.cpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>

#include <tgfx/platform/Print.h>

namespace kk {
SeatCraftEditorCoreApp::SeatCraftEditorCoreApp(int boundsWidth, int boundsHeight, float density)
    : _boundsWidth(boundsWidth)
    , _boundsHeight(boundsHeight)
    , _density(density) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

SeatCraftEditorCoreApp::~SeatCraftEditorCoreApp() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

int SeatCraftEditorCoreApp::getBoundsWidth() const {
    return _boundsWidth;
}
int SeatCraftEditorCoreApp::getBoundsHeight() const {
    return _boundsHeight;
}

float SeatCraftEditorCoreApp::density() const {
    return _density;
}

bool SeatCraftEditorCoreApp::updateBounds(int width, int height, float density) {
    if (width <= 0 || height <= 0) {
        tgfx::PrintError("%s width or height is invalid!", __PRETTY_FUNCTION__);
        return false;
    }

    if (density < 1.0) {
        tgfx::PrintError("%s density is invalid!", __PRETTY_FUNCTION__);
        return false;
    }

    if (_boundsWidth == width && _boundsHeight == height && _density == density) {
        return false;
    }

    _boundsWidth = width;
    _boundsHeight = height;
    _density = density;
    return true;
}

}  // namespace kk
