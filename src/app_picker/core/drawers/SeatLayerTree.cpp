//
//  SeatLayerTree.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#include "SeatLayerTree.hpp"

#include "core/FileReader.h"
#include "core/SeatCraftCoreApp.hpp"
#include "core/svg/ConvertSVGLayer.hpp"
#include "core/svg/SVGLoader.hpp"

#include <SeatCraft/common/common_macro.h>

#include <tgfx/core/Canvas.h>
#include <tgfx/core/Data.h>
#include <tgfx/core/Path.h>
#include <tgfx/core/Rect.h>
#include <tgfx/core/Stream.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ImageLayer.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/layers/TextLayer.h>
#include <tgfx/platform/Print.h>
#include <tgfx/svg/SVGDOM.h>

#include <filesystem>

#define SHOW_MOCK_SEAT 0

namespace fs = std::filesystem;

namespace kk::drawers {

SeatLayerTree::SeatLayerTree()
    : kk::drawers::Drawer("SeatLayerTree")
    , _root(nullptr)
    , _seatLayer(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>()) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
};

SeatLayerTree::~SeatLayerTree() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

bool SeatLayerTree::hasContentChanged() const {
    if (_root == nullptr) {
        return true;
    }
    return _displayList->hasContentChanged();
}

void SeatLayerTree::prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app, bool force) {
    prebuildSeatStatusImage(canvas, app);

    if (updateContentSize(app) && _root != nullptr) {
        if (_seatLayer) {
            _seatLayer->removeFromParent();
            _seatLayer = nullptr;
        }

        _root->removeFromParent();
        _root = nullptr;
    }

    if (force) {
        if (_seatLayer) {
            _seatLayer->removeFromParent();
            _seatLayer = nullptr;
        }

        if (_root) {
            _root->removeFromParent();
            _root = nullptr;
        }
    }

    auto zoomScale = app->zoomScale();
    auto contentOffset = app->contentOffset();
    _displayList->setZoomScale(zoomScale);
    _displayList->setContentOffset(contentOffset.x, contentOffset.y);
}

bool SeatLayerTree::updateContentSize(const kk::SeatCraftCoreApp *app) {
    auto contentSize = app->getContentSize();
    if (_contentSize == contentSize) {
        return false;
    }

    _contentSize = contentSize;
    return true;
}

bool SeatLayerTree::prebuildSeatStatusImage(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    if (!_seatStatusImageMap.empty()) {
        return false;
    }

    auto fileReader = app->getFileReader();

    if (fileReader == nullptr) {
        return false;
    }

    const auto &seatStatusSvgMap = app->getSeatStatusSvgMap();
    if (seatStatusSvgMap.empty()) {
        return false;
    }
    auto statusSize = seatStatusSvgMap.size();
    auto columns = 4;
    auto rows = static_cast<int>(std::ceil(static_cast<float>(statusSize) / static_cast<float>(columns)));
    int lineSpacing = 10;
    int itemSpacing = 10;

    auto density = app->density();
    auto itemWidth = static_cast<int>(32 * density);
    auto itemHeight = static_cast<int>(32 * density);
    auto surfaceWidth = itemWidth * columns + (columns + 1) * itemSpacing;
    auto surfaceHeight = itemHeight * rows + (rows + 1) * lineSpacing;

    auto context = canvas->getSurface()->getContext();
    auto surface = tgfx::Surface::Make(context, surfaceWidth, surfaceHeight, tgfx::ColorType::RGBA_8888);
    auto tempCanvas = surface->getCanvas();
    tempCanvas->clear();
    std::unordered_map<kk::SeatStatusKey, tgfx::Rect> rectsMap{};
    auto startX = static_cast<float>(itemSpacing), startY = static_cast<float>(lineSpacing);
    for (const auto &[key, value] : seatStatusSvgMap) {
        auto data = fileReader->readData(value);
        if (data == nullptr) {
            continue;
        }
        auto stream = tgfx::Stream::MakeFromData(data);
        if (stream == nullptr) {
            continue;
        }
        auto seatSvgDom = kk::svg::loadSvgDom(stream.get());
        if (seatSvgDom == nullptr) {
            continue;
        }

        auto domSize = seatSvgDom->getContainerSize();
        auto scale = static_cast<float>(itemWidth) / domSize.width;
        auto matrix = tgfx::Matrix::MakeScale(scale);
        matrix.postTranslate(startX, startY);

        tempCanvas->save();
        tempCanvas->concat(matrix);
        seatSvgDom->render(tempCanvas);
        tempCanvas->restore();

        rectsMap.emplace(key, tgfx::Rect::MakeXYWH(startX, startY, static_cast<float>(itemWidth), static_cast<float>(itemHeight)));

        if ((startX + static_cast<float>(itemWidth + itemSpacing)) >= static_cast<float>(surfaceWidth - itemSpacing)) {
            startX = static_cast<float>(itemSpacing);
        } else {
            startX += static_cast<float>(itemWidth + itemSpacing);
        }

        if ((startY + static_cast<float>(itemHeight + lineSpacing)) >= static_cast<float>(surfaceHeight - itemHeight)) {
            startY = static_cast<float>(lineSpacing);
        } else {
            startY += static_cast<float>(itemHeight + lineSpacing);
        }
    }

    if (rectsMap.empty()) {
        return false;
    }

    _seatStatusImageMap.clear();
    auto image = surface->makeImageSnapshot();
    for (const auto &[key, rect] : rectsMap) {
        auto subimage = image->makeSubset(rect);
        _seatStatusImageMap.emplace(key, subimage);
    }

    return true;
}

std::shared_ptr<tgfx::Layer> SeatLayerTree::buildLayerTree(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    UNUSED_PARAM(canvas);
    auto root = tgfx::Layer::Make();
    auto areaSvgDom = app->getSvgDom();
    if (areaSvgDom == nullptr) {
        return root;
    }

    auto areaDomSize = areaSvgDom->getContainerSize();
    auto areaLayer = kk::svg::convertSVGDomToLayer(areaSvgDom);
    if (areaLayer) {
        _areaDomSize = areaDomSize;
        root->addChild(areaLayer);
    }

#if SHOW_MOCK_SEAT

#if 0
    do {
        // 渲染座位 Mock
        const auto &seatStatusSvgMap = app->getSeatStatusSvgMap();
        auto iter = seatStatusSvgMap.find(1);
        if (iter == seatStatusSvgMap.end()) {
            break;
        }

        auto seatSvgDom = kk::svg::loadSvgDom(iter->second);
        if (seatSvgDom == nullptr) {
            break;
        }

        auto areaDomSize = areaSvgDom->getContainerSize();
        auto seatDomSize = seatSvgDom->getContainerSize();

        float seatStartX = areaDomSize.width * 0.1;
        float seatStartY = areaDomSize.height * 0.1;
        float lineSpacing = 10.0f;
        float itemSpacing = 10.0f;
        int rows = 100, columns = 150;

        auto seatRootLayer = tgfx::Layer::Make();

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                float itemX = seatStartX + col * lineSpacing + col * seatDomSize.width;
                float itemY = seatStartY + row * itemSpacing + row * seatDomSize.height;

                auto seatLayer = kk::svg::convertSVGDomToLayer(seatSvgDom);
                if (seatLayer) {
                    seatLayer->setShouldRasterize(true);
                    auto matrix = tgfx::Matrix::MakeTrans(itemX, itemY);
                    seatLayer->setMatrix(matrix);
                    seatRootLayer->addChild(seatLayer);
                }
            }
        }
        _seatLayer = seatRootLayer;
        root->addChild(seatRootLayer);

    } while (0);
#else
    do {
        // 渲染座位 Mock
        auto iter = _seatStatusImageMap.find(1);
        if (iter == _seatStatusImageMap.end()) {
            break;
        }

        auto bitmap = iter->second;
        auto bitmapWidth = bitmap->width();

        auto seatDomSize = tgfx::Size::Make(32, 32);
        float seatStartX = areaDomSize.width * 0.1f;
        float seatStartY = areaDomSize.height * 0.1f;
        float lineSpacing = 10.0f;
        float itemSpacing = 10.0f;
#if ANDROID
        int rows = 10, columns = 10;
#else
        int rows = 100, columns = 100;
#endif

        auto scale = seatDomSize.width / static_cast<float>(bitmapWidth);

        auto seatRootLayer = tgfx::Layer::Make();

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                float itemX = seatStartX + static_cast<float>(col) * lineSpacing + static_cast<float>(col) * seatDomSize.width;
                float itemY = seatStartY + static_cast<float>(row) * itemSpacing + static_cast<float>(row) * seatDomSize.height;

                auto matrix = tgfx::Matrix::MakeScale(scale);
                matrix.postTranslate(itemX, itemY);
                auto seatLayer = tgfx::ImageLayer::Make();
                seatLayer->setImage(bitmap);
                seatLayer->setMatrix(matrix);
                seatRootLayer->addChild(seatLayer);
            }
        }

        _seatLayer = seatRootLayer;
        root->addChild(seatRootLayer);

    } while (0);
#endif

#endif

    return root;
}

void SeatLayerTree::updateRootMatrix(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    UNUSED_PARAM(canvas);
    auto limitSize = app->getContentSize();
    if (_root == nullptr) {
        return;
    }

    if (limitSize.isEmpty() || _areaDomSize.isEmpty()) {
        _root->setMatrix(tgfx::Matrix::I());
        return;
    }

    // 计算缩放比例（等比例缩放，保证完整显示）
    const float scaleX = limitSize.width / _areaDomSize.width;
    const float scaleY = limitSize.height / _areaDomSize.height;
    const float fitScale = std::min(scaleX, scaleY);

    tgfx::Matrix finalMatrix = tgfx::Matrix::I();
    finalMatrix.postScale(fitScale, fitScale);

    _root->setMatrix(finalMatrix);
}

void SeatLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    if (_root == nullptr) {
        _root = buildLayerTree(canvas, app);
        updateRootMatrix(canvas, app);
        _displayList->root()->addChild(_root);
        _displayList->setRenderMode(tgfx::RenderMode::Tiled);
        _displayList->setAllowZoomBlur(true);
        _displayList->setMaxTileCount(512);
        _displayList->setMaxTilesRefinedPerFrame(512);
    }

    auto surface = canvas->getSurface();
    _displayList->render(surface, false);
}

}  // namespace kk::drawers
