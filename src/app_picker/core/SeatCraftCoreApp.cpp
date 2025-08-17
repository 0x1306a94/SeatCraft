//
//  SeatCraftCoreApp.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "SeatCraftCoreApp.hpp"

#include "FileReader.h"
#include "svg/SVGDataProvider.h"
#include "svg/SVGLoader.hpp"

#include <tgfx/core/Stream.h>
#include <tgfx/platform/Print.h>
#include <tgfx/svg/SVGDOM.h>

namespace kk {
SeatCraftCoreApp::SeatCraftCoreApp(const tgfx::Size &boundSize, const tgfx::Size &contentSize, float density)
    : _boundSize(boundSize)
    , _contentSize(contentSize)
    , _density(density)
    , _svgDataProvider(std::make_shared<kk::svg::SVGDataProvider>())
    , _fileReader(std::make_shared<kk::FileReader>()) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

SeatCraftCoreApp::~SeatCraftCoreApp() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

void SeatCraftCoreApp::setFileReader(std::shared_ptr<FileReader> fileReader) {
    _fileReader = fileReader;
}

std::shared_ptr<FileReader> SeatCraftCoreApp::getFileReader() const {
    return _fileReader;
}

tgfx::Size SeatCraftCoreApp::getBoundsSize() const {
    return _boundSize;
}

tgfx::Size SeatCraftCoreApp::getContentSize() const {
    return _contentSize;
}

tgfx::Size SeatCraftCoreApp::getOriginSize() const {
    tgfx::Size size{};
    auto svgDom = _svgDataProvider->getAreaSVGDOM();
    if (svgDom) {
        size = svgDom->getContainerSize();
    }
    return size;
}

float SeatCraftCoreApp::density() const {
    return _density;
}

/// 当前缩放比例
float SeatCraftCoreApp::zoomScale() const {
    return _zoomScale;
}

/// 当前滑动偏移
const tgfx::Point &SeatCraftCoreApp::contentOffset() const {
    return _contentOffset;
}

std::shared_ptr<tgfx::SVGDOM> SeatCraftCoreApp::getSvgDom() const {
    auto svgDom = _svgDataProvider->getAreaSVGDOM();
    return svgDom;
}

std::shared_ptr<kk::svg::SVGDataProvider> SeatCraftCoreApp::getSvgDataProvider() const {
    return _svgDataProvider;
}

bool SeatCraftCoreApp::updateContentSize(const tgfx::Size &contentSize) {
    if (contentSize.width <= 0 || contentSize.height <= 0) {
        tgfx::PrintError("%s width or height is invalid!", __PRETTY_FUNCTION__);
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
        tgfx::PrintError("%s width or height is invalid!", __PRETTY_FUNCTION__);
        return false;
    }
    if (density < 1.0) {
        tgfx::PrintError("%s density is invalid!", __PRETTY_FUNCTION__);
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

};  // namespace kk
