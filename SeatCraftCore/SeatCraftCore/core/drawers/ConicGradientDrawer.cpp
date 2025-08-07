//
//  ConicGradientDrawer.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "ConicGradientDrawer.hpp"

#include "../SeatCraftCoreApp.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

ConicGradientDrawer::ConicGradientDrawer()
    : kk::drawers::Drawer("ConicGradientDrawer") {
}

ConicGradientDrawer::~ConicGradientDrawer() {
    tgfx::PrintLog("ConicGradientDrawer::~ConicGradientDrawer");
}

void ConicGradientDrawer::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    auto scale = app->density();
    auto width = app->width();
    auto height = app->height();
    tgfx::Color cyan = {0.0f, 1.0f, 1.0f, 1.0f};
    tgfx::Color magenta = {1.0f, 0.0f, 1.0f, 1.0f};
    tgfx::Color yellow = {1.0f, 1.0f, 0.0f, 1.0f};
    auto shader = tgfx::Shader::MakeConicGradient(tgfx::Point::Make(width / 2, height / 2), 0, 360,
                                                  {cyan, magenta, yellow, cyan}, {});
    tgfx::Paint paint = {};
    paint.setShader(shader);
    auto screenSize = std::min(width, height);
    auto size = screenSize - static_cast<int>(150 * scale);
    size = std::max(size, 50);
    auto rect = tgfx::Rect::MakeXYWH((width - size) / 2, (height - size) / 2, size, size);
    tgfx::Path path = {};
    path.addRoundRect(rect, 20 * scale, 20 * scale);
    canvas->translate(app->contentOffset().x, app->contentOffset().y);
    canvas->scale(app->zoomScale(), app->zoomScale());
    canvas->drawPath(path, paint);
}

};  // namespace kk::drawers
