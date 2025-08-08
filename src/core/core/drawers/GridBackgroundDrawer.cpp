//
//  GridBackgroundDrawer.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "GridBackgroundDrawer.hpp"

#include "../SeatCraftCoreApp.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

GridBackgroundDrawer::GridBackgroundDrawer()
    : kk::drawers::Drawer("GridBackgroundDrawer") {
}

GridBackgroundDrawer::~GridBackgroundDrawer() {
    tgfx::PrintLog("GridBackgroundDrawer::~GridBackgroundDrawer");
}

void GridBackgroundDrawer::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    auto boundsSize = app->getBoundsSize();
    auto width = boundsSize.width;
    auto height = boundsSize.height;
    canvas->clear(tgfx::Color::White());
    tgfx::Paint paint = {};
    paint.setColor(tgfx::Color{0.8f, 0.8f, 0.8f, 1.f});
    int tileSize = 8 * static_cast<int>(app->density());
    for (int y = 0; y < height; y += tileSize) {
        bool draw = (y / tileSize) % 2 == 1;
        for (int x = 0; x < width; x += tileSize) {
            if (draw) {
                auto rect = tgfx::Rect::MakeXYWH(static_cast<float>(x), static_cast<float>(y),
                                                 static_cast<float>(tileSize), static_cast<float>(tileSize));
                canvas->drawRect(rect, paint);
            }
            draw = !draw;
        }
    }
}

};  // namespace kk::drawers
