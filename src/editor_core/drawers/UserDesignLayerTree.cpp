//
//  UserDesignLayerTree.cpp
//  SeatCraftDesignModel
//
//  Created by king on 2025/8/20.
//

#include "UserDesignLayerTree.hpp"

#include <SeatCraft/common/common_macro.h>
#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>

#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/Layer.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>

namespace kk::drawers {
UserDesignLayerTree::UserDesignLayerTree()
    : kk::drawers::Drawer("UserDesignLayerTree")
    , _displayList(std::make_unique<tgfx::DisplayList>()) {
    _displayList->setRenderMode(tgfx::RenderMode::Tiled);
    _displayList->setAllowZoomBlur(true);
    _displayList->setMaxTileCount(512);
}

UserDesignLayerTree::~UserDesignLayerTree() {
}

std::vector<std::shared_ptr<tgfx::Layer>> UserDesignLayerTree::getLayersUnderPoint(float x, float y) {
    return _displayList->root()->getLayersUnderPoint(x, y);
}

bool UserDesignLayerTree::hasContentChanged() const {
    if (_root == nullptr) {
        return true;
    }
    return _displayList->hasContentChanged();
}

void UserDesignLayerTree::prepare(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app, bool force) {
    UNUSED_PARAM(canvas);
    UNUSED_PARAM(app);
    UNUSED_PARAM(force);

    auto zoomScale = app->zoomScale();
    auto contentOffset = app->contentOffset();
    _displayList->setZoomScale(zoomScale);
    _displayList->setContentOffset(contentOffset.x, contentOffset.y);
}

void UserDesignLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app) {
    UNUSED_PARAM(app);
    if (_root == nullptr) {
        _root = tgfx::Layer::Make();

        do {

            auto venueLayer = tgfx::ShapeLayer::Make();
            tgfx::Path venuePath;
            auto venueRect = tgfx::Rect::MakeWH(600, 600);
            venuePath.addRect(venueRect);
            venueLayer->setPath(venuePath);
            venueLayer->addFillStyle(tgfx::SolidColor::Make(tgfx::Color::Green()));
            venueLayer->addStrokeStyle(tgfx::SolidColor::Make(tgfx::Color::Black()));
            venueLayer->setLineWidth(2);
            venueLayer->setMatrix(tgfx::Matrix::MakeTrans(300, 300));

            tgfx::Path rectPath;
            tgfx::Rect rectRect = tgfx::Rect::MakeWH(300, 300);
            rectRect.offset(30, 30);
            rectPath.addRoundRect(rectRect, 20, 20);
            auto rectLayer = tgfx::ShapeLayer::Make();
            rectLayer->setPath(rectPath);
            rectLayer->addFillStyle(tgfx::SolidColor::Make(tgfx::Color::Red()));
            rectLayer->addStrokeStyle(tgfx::SolidColor::Make(tgfx::Color::Blue()));
            rectLayer->setLineWidth(10);

            venueLayer->addChild(rectLayer);

            _root->addChild(venueLayer);
        } while (0);

        _displayList->root()->addChild(_root);
    }

    auto surface = canvas->getSurface();
    _displayList->render(surface, false);
}

};  // namespace kk::drawers
