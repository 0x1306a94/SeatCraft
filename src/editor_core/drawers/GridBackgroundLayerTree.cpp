//
//  GridBackgroundLayerTree.cpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#include "GridBackgroundLayerTree.hpp"

#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>

#include <SeatCraft/common/common_macro.h>

#include <tgfx/core/Canvas.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/LayerRecorder.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

class GridBackgroundLayer : public tgfx::Layer {
  private:
    tgfx::Size _contentSize{};
    float _density{1.0};

  public:
    static std::shared_ptr<GridBackgroundLayer> Make() {
        return std::shared_ptr<GridBackgroundLayer>(new GridBackgroundLayer());
    }

    void setContentSize(const tgfx::Size &contentSize) {
        if (_contentSize == contentSize) {
            return;
        }
        _contentSize = contentSize;
        invalidateContent();
    }

    void setDensity(float density) {
        if (_density == density) {
            return;
        }
        _density = density;
        invalidateContent();
    }

  protected:
    GridBackgroundLayer() = default;
    void onUpdateContent(tgfx::LayerRecorder *recorder) override {
        if (_contentSize.isEmpty()) {
            return;
        }
        auto canvas = recorder->getCanvas();
        auto width = static_cast<int>(_contentSize.width);
        auto height = static_cast<int>(_contentSize.height);
        canvas->clear(tgfx::Color::White());
        tgfx::Paint paint = {};
        paint.setColor(tgfx::Color{0.8f, 0.8f, 0.8f, 1.f});
        int tileSize = 8 * static_cast<int>(_density);
        for (int y = 0; y < height; y += tileSize) {
            bool draw = (y / tileSize) % 2 == 1;
            for (int x = 0; x < width; x += tileSize) {
                if (draw) {
                    auto rect = tgfx::Rect::MakeXYWH(static_cast<float>(x), static_cast<float>(y), static_cast<float>(tileSize), static_cast<float>(tileSize));
                    canvas->drawRect(rect, paint);
                }
                draw = !draw;
            }
        }
    }
};

GridBackgroundLayerTree::GridBackgroundLayerTree()
    : kk::drawers::Drawer("GridBackgroundLayerTree")
    , _root(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>()) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

GridBackgroundLayerTree::~GridBackgroundLayerTree() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

bool GridBackgroundLayerTree::hasContentChanged() const {
    if (_root == nullptr) {
        return true;
    }
    return _displayList->hasContentChanged();
}

void GridBackgroundLayerTree::prepare(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app, bool force) {
    UNUSED_PARAM(canvas);
    UNUSED_PARAM(force);
    if (_root != nullptr) {
        _root->setContentSize(app->getBoundsSize());
        _root->setDensity(app->density());
    }
}

void GridBackgroundLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app) {
    UNUSED_PARAM(app);
    if (_root == nullptr) {
        _root = GridBackgroundLayer::Make();
        _root->setShouldRasterize(true);
        _displayList->root()->addChild(_root);
        _displayList->setRenderMode(tgfx::RenderMode::Direct);
    }

    auto surface = canvas->getSurface();
    _displayList->render(surface, false);
}

};  // namespace kk::drawers
