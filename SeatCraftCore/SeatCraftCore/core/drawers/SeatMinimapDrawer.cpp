//
//  SeatMinimapDrawer.cpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/7.
//

#include "SeatMinimapDrawer.hpp"

#include "../SeatCraftCoreApp.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

static std::shared_ptr<tgfx::Layer> CreateMinimapBackground(const kk::SeatCraftCoreApp *app) {
    auto density = app->density();
    auto width = 140.0f * density;
    auto height = 140.0f * density;
//    auto cornerRadius = 10.0f * density;
    auto background = tgfx::ShapeLayer::Make();
    tgfx::Rect displayRect = tgfx::Rect::MakeWH(width, height);
    auto backPath = tgfx::Path();
    //    backPath.addRoundRect(displayRect, cornerRadius, cornerRadius);
    backPath.addRect(displayRect);
    background->setFillStyle(tgfx::SolidColor::Make(tgfx::Color::FromRGBA(0, 0, 0, 153)));
    background->setPath(backPath);
    return background;
}

SeatMinimapDrawer::SeatMinimapDrawer()
    : kk::drawers::Drawer("SeatMinimapDrawer")
    , root(nullptr)
    , displayList(std::make_unique<tgfx::DisplayList>()) {
}

SeatMinimapDrawer::~SeatMinimapDrawer() {
}

std::shared_ptr<tgfx::Layer> SeatMinimapDrawer::buildLayerTree(const kk::SeatCraftCoreApp *app) {
    auto root = tgfx::Layer::Make();
    auto background = CreateMinimapBackground(app);
    root->addChild(background);
    return root;
}

void SeatMinimapDrawer::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    if (root == nullptr) {
        root = buildLayerTree(app);
        displayList->root()->addChild(root);
        displayList->setRenderMode(tgfx::RenderMode::Tiled);
    }

    auto rootBounds = root->getBounds();
    auto surface = canvas->getSurface();
    auto width = surface->width();
    auto transX = width - rootBounds.width();
    auto matrix = tgfx::Matrix::MakeTrans(transX, 0);
    root->setMatrix(matrix);

    displayList->render(surface, false);
}

}  // namespace kk::drawers
