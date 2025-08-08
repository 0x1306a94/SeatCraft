//
//  Drawer.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#include "Drawer.hpp"

#include "../SeatCraftCoreApp.hpp"

#include <tgfx/platform/Print.h>
#include <tgfx/core/Canvas.h>

namespace kk::drawers {
Drawer::Drawer(const std::string &name)
    : _name(name) {
}

void Drawer::draw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    if (canvas == nullptr) {
        tgfx::PrintError("Drawer::draw() canvas is nullptr!");
        return;
    }
    if (app == nullptr) {
        tgfx::PrintError("Drawer::draw() app is nullptr!");
        return;
    }
    tgfx::AutoCanvasRestore autoRestore(canvas);
    onDraw(canvas, app);
}
};  // namespace kk::drawers
