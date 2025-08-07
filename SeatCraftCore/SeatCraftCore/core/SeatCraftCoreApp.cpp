//
//  SeatCraftCoreApp.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "SeatCraftCoreApp.hpp"

#include <tgfx/platform/Print.h>

namespace kk {
SeatCraftCoreApp::SeatCraftCoreApp(int width, int height, float density)
    : _width(width)
    , _height(height)
    , _density(density) {
}

SeatCraftCoreApp::~SeatCraftCoreApp() {
    tgfx::PrintLog("SeatCraftCoreApp::~SeatCraftCoreApp()");
}

bool SeatCraftCoreApp::updateScreen(int width, int height, float density) {
    if (width <= 0 || height <= 0) {
        tgfx::PrintError("SeatCraftCoreApp::updateScreen() width or height is invalid!");
        return false;
    }
    if (density < 1.0) {
        tgfx::PrintError("SeatCraftCoreApp::updateScreen() density is invalid!");
        return false;
    }
    if (width == _width && height == _height && density == _density) {
        return false;
    }
    _width = width;
    _height = height;
    _density = density;
    return true;
}

bool SeatCraftCoreApp::updateZoomAndOffset(float zoomScale, const tgfx::Point &contentOffset) {
    if (zoomScale == _zoomScale && contentOffset == _contentOffset) {
        return false;
    }
    _zoomScale = zoomScale;
    _contentOffset = contentOffset;
    return true;
}

};  // namespace kk
