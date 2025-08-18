//
//  Drawer.cpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#include "Drawer.hpp"

#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>

#include <tgfx/core/Canvas.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {
Drawer::Drawer(const std::string &name)
    : _name(name) {
}

void Drawer::draw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app) {
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
