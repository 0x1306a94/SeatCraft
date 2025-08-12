//
//  SeatCraftCoreApp.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "SeatCraftCoreApp.hpp"

#include "svg/SVGLoader.hpp"

#include <tgfx/platform/Print.h>
#include <tgfx/svg/SVGDOM.h>

#include <filesystem>

namespace kk {
SeatCraftCoreApp::SeatCraftCoreApp(const tgfx::Size &boundSize, const tgfx::Size &contentSize, float density)
    : _boundSize(boundSize)
    , _contentSize(contentSize)
    , _density(density) {
}

SeatCraftCoreApp::~SeatCraftCoreApp() {
    tgfx::PrintLog("SeatCraftCoreApp::~SeatCraftCoreApp()");
}

tgfx::Size SeatCraftCoreApp::getBoundsSize() const {
    return _boundSize;
}

tgfx::Size SeatCraftCoreApp::getContentSize() const {
    return _contentSize;
}

tgfx::Size SeatCraftCoreApp::getOriginSize() const {
    tgfx::Size size{};
    if (_svgDom) {
        size = _svgDom->getContainerSize();
    }
    return size;
}

std::shared_ptr<tgfx::SVGDOM> SeatCraftCoreApp::getSvgDom() const {
    return _svgDom;
}

std::string SeatCraftCoreApp::getAreaSvgPath() const {
    return _areaSvgPath;
}

const kk::SeatStatusSVGPathMap &SeatCraftCoreApp::getSeatStatusSvgMap() const {
    return _seatStatusSvgMap;
}

bool SeatCraftCoreApp::updateContentSize(const tgfx::Size &contentSize) {
    if (contentSize.width <= 0 || contentSize.height <= 0) {
        tgfx::PrintError("SeatCraftCoreApp::updateContentSize() width or height is invalid!");
        return false;
    }

    if (contentSize == _contentSize) {
        return false;
    }
    _contentSize = contentSize;
    return true;
}

bool SeatCraftCoreApp::updateScreen(const tgfx::Size &boundSize, float density) {
    if (boundSize.width <= 0 || boundSize.height <= 0) {
        tgfx::PrintError("SeatCraftCoreApp::updateScreen() width or height is invalid!");
        return false;
    }
    if (density < 1.0) {
        tgfx::PrintError("SeatCraftCoreApp::updateScreen() density is invalid!");
        return false;
    }
    if (boundSize == _boundSize && density == _density) {
        return false;
    }
    _boundSize = boundSize;
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

bool SeatCraftCoreApp::updateAreaSvgPath(const std::string &path) {
    if (_areaSvgPath == path) {
        _svgDom = nullptr;
        return false;
    }

    namespace fs = std::filesystem;
    if (!fs::exists(path)) {
        tgfx::PrintError("SeatCraftCoreApp::updateAreaSvgPath() path does not exist!");
        _svgDom = nullptr;
        return false;
    }

    _areaSvgPath = path;
    _svgDom = kk::svg::loadSvgDom(path);
    return true;
}

bool SeatCraftCoreApp::updateSeatStatusSVGPathMap(SeatStatusSVGPathMap map) {
    if (_seatStatusSvgMap.empty() && map.empty()) {
        return false;
    }
    _seatStatusSvgMap.clear();
    _seatStatusSvgMap.insert(map.begin(), map.end());
    return true;
}

};  // namespace kk
