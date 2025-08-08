//
//  SeatLayerTree.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#include "SeatLayerTree.hpp"

#include "../SeatCraftCoreApp.hpp"

#include "SeatAreaLayer.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

SeatLayerTree::SeatLayerTree()
    : kk::drawers::Drawer("SeatLayerTree")
    , _root(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>())
    , _areaLayer(nullptr) {
}

SeatLayerTree::~SeatLayerTree() {
    tgfx::PrintLog("SeatLayerTree::~SeatLayerTree");
}

bool SeatLayerTree::updateContentSize(const kk::SeatCraftCoreApp *app) {
    auto contentSize = app->getContentSize();
    if (_contentSize == contentSize) {
        return false;
    }

    _contentSize = contentSize;
    return true;
}

void SeatLayerTree::updateRootMatrix(const kk::SeatCraftCoreApp *app) {
    auto zoomScale = app->zoomScale();
    auto contentOffset = app->contentOffset();
}

std::shared_ptr<tgfx::Layer> SeatLayerTree::buildLayerTree(const kk::SeatCraftCoreApp *app) {
    auto root = tgfx::Layer::Make();
    _areaLayer = SeatAreaLayer::Make(app);
    _areaLayer->setContentSize(_contentSize);
    root->addChild(_areaLayer);
    return root;
}

void SeatLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    updateContentSize(app);

    if (_root == nullptr) {
        _root = buildLayerTree(app);
        _displayList->root()->addChild(_root);
        _displayList->setRenderMode(tgfx::RenderMode::Tiled);
        // Zoom blur is currently disabled because the Hello2D demo doesn't yet support animation frame
        // rendering with displayList:
        _displayList->setAllowZoomBlur(true);
        _displayList->setMaxTileCount(512);
    }

    if (_areaLayer != nullptr) {
        _areaLayer->setContentSize(_contentSize);
        _areaLayer->setAreaSvgPath(app->getAreaSvgPath());
    }

    //    auto rootBounds = _root->getBounds();
    auto surface = canvas->getSurface();
    //    auto width = surface->width();
    //    auto height = surface->height();

    updateRootMatrix(app);
    auto zoomScale = app->zoomScale();
    auto contentOffset = app->contentOffset();
    _displayList->setZoomScale(zoomScale);
    _displayList->setContentOffset(contentOffset.x, contentOffset.y);
    _displayList->render(surface, false);
}

}  // namespace kk::drawers
