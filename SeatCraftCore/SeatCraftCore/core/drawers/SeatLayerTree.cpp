//
//  SeatLayerTree.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#include "SeatLayerTree.hpp"

#include "../SeatCraftCoreApp.hpp"
#include "../svg/ConvertSVGLayer.hpp"
#include "../svg/SVGLoader.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/core/Path.h>
#include <tgfx/core/Rect.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ImageLayer.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/layers/TextLayer.h>
#include <tgfx/platform/Print.h>
#include <tgfx/svg/SVGDOM.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace kk::drawers {

SeatLayerTree::SeatLayerTree()
    : kk::drawers::Drawer("SeatLayerTree")
    , _root(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>()) {
}

SeatLayerTree::~SeatLayerTree() {
    tgfx::PrintLog("SeatLayerTree::~SeatLayerTree");
}

bool SeatLayerTree::hasContentChanged() const {
    if (_root == nullptr) {
        return true;
    }
    return _displayList->hasContentChanged();
}

void SeatLayerTree::prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    if (updateContentSize(app) && _root != nullptr) {
        _root->removeFromParent();
        _root = nullptr;
    }

    updateRootMatrix(canvas, app);
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

bool SeatLayerTree::prebuildSeatStatusBitmap(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    if (!_seatStatusImageMap.empty()) {
        return false;
    }

    const auto &seatStatusSvgMap = app->getSeatStatusSvgMap();
    if (seatStatusSvgMap.empty()) {
        return false;
    }
    auto statusSize = seatStatusSvgMap.size();
    auto columns = 4;
    auto rows = static_cast<int>(std::ceil(static_cast<float>(statusSize) / static_cast<float>(columns)));
    auto lineSpacing = 10;
    auto itemSpacing = 10;

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
        auto seatSvgDom = kk::svg::loadSvgDom(value);
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

        if ((startX + itemWidth + itemSpacing) >= (surfaceWidth - itemSpacing)) {
            startX = itemSpacing;
        } else {
            startX += itemWidth + itemSpacing;
        }

        if ((startY + itemHeight + lineSpacing) >= (surfaceHeight - itemHeight)) {
            startY = lineSpacing;
        } else {
            startY += itemHeight + lineSpacing;
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

    auto root = tgfx::Layer::Make();
    auto areaSvgDom = kk::svg::loadSvgDom(app->getAreaSvgPath());
    auto areaDomSize = areaSvgDom->getContainerSize();
    auto areaLayer = kk::svg::convertSVGDomToLayer(areaSvgDom);
    if (areaLayer) {
        _areaDomSize = areaDomSize;
        root->addChild(areaLayer);
    }

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
                float itemX = seatStartX + row * lineSpacing + row * seatDomSize.width;
                float itemY = seatStartY + col * itemSpacing + col * seatDomSize.height;

                auto seatLayer = kk::svg::convertSVGDomToLayer(seatSvgDom);
                if (seatLayer) {
                    auto matrix = tgfx::Matrix::MakeTrans(itemX, itemY);
                    seatLayer->setMatrix(matrix);
                    seatRootLayer->addChild(seatLayer);
                }
            }
        }

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
        auto bitmapHeight = bitmap->height();

        auto seatDomSize = tgfx::Size::Make(32, 32);
        float seatStartX = areaDomSize.width * 0.1;
        float seatStartY = areaDomSize.height * 0.1;
        float lineSpacing = 10.0f;
        float itemSpacing = 10.0f;
        int rows = 100, columns = 200;

        auto scale = seatDomSize.width / static_cast<float>(bitmapWidth);

        auto seatRootLayer = tgfx::Layer::Make();

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                float itemX = seatStartX + col * lineSpacing + col * seatDomSize.width;
                float itemY = seatStartY + row * itemSpacing + row * seatDomSize.height;

                auto matrix = tgfx::Matrix::MakeScale(scale);
                matrix.postTranslate(itemX, itemY);
                auto seatLayer = tgfx::ImageLayer::Make();
                seatLayer->setImage(bitmap);
                seatLayer->setMatrix(matrix);
                seatRootLayer->addChild(seatLayer);
            }
        }

        root->addChild(seatRootLayer);

    } while (0);
#endif

    return root;
}

void SeatLayerTree::updateRootMatrix(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    auto limitSize = app->getContentSize();
    if (_root == nullptr) {
        return;
    }

    if (limitSize.isEmpty() || _areaDomSize.isEmpty()) {
        _root->setMatrix(tgfx::Matrix::I());
        return;
    }

    auto surface = canvas->getSurface();
    auto surfaceWidth = surface->width();
    auto surfaceHeight = surface->height();

    const float scaleX = limitSize.width / _areaDomSize.width;
    const float scaleY = limitSize.height / _areaDomSize.height;
    const float fitScale = std::min(scaleX, scaleY);

    const tgfx::Point svgCenter(_areaDomSize.width * 0.5f, _areaDomSize.height * 0.5f);
    const tgfx::Point canvasCenter(surfaceWidth * 0.5f, surfaceHeight * 0.5f);

    tgfx::Matrix matrix = tgfx::Matrix::I();
    matrix.postTranslate(-svgCenter.x, -svgCenter.y);      // 移动 SVG 到原点中心
    matrix.postScale(fitScale, fitScale);                  // 缩放
    matrix.postTranslate(canvasCenter.x, canvasCenter.y);  // 移动到 Canvas 中心

    _root->setMatrix(matrix);
}

void SeatLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    prebuildSeatStatusBitmap(canvas, app);

    if (_root == nullptr) {
        _root = buildLayerTree(canvas, app);
        _displayList->root()->addChild(_root);
        _displayList->setRenderMode(tgfx::RenderMode::Tiled);
        // Zoom blur is currently disabled because the Hello2D demo doesn't yet support animation frame
        // rendering with displayList:
        _displayList->setAllowZoomBlur(true);
        _displayList->setMaxTileCount(512);
    }

    auto surface = canvas->getSurface();
    _displayList->render(surface, false);
}

}  // namespace kk::drawers
