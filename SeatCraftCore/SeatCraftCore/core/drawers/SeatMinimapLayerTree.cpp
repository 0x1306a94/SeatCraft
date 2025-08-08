//
//  SeatMinimapLayerTree.cpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/7.
//

#include "SeatMinimapLayerTree.hpp"

#include "../SeatCraftCoreApp.hpp"

#include <tgfx/core/Canvas.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

class SeatMinimapContainerLayer : public tgfx::Layer {

  public:
    SeatMinimapContainerLayer(const tgfx::Size &size, const tgfx::Color &color)
        : _size(size)
        , _color(color) {

        invalidateContent();
    }

    void updateSize(const tgfx::Size &size) {
        if (_size == size) {
            return;
        }
        _size = size;
        invalidateContent();
    }

    void updateColor(const tgfx::Color &color) {
        if (_color == color) {
            return;
        }
        _color = color;
        invalidateContent();
    }

  protected:
    void onUpdateContent(tgfx::LayerRecorder *recorder) override {
        if (_size.isEmpty()) {
            return;
        }
        auto canvas = recorder->getCanvas(tgfx::LayerContentType::Default);

        tgfx::Rect displayRect = tgfx::Rect::MakeWH(_size.width, _size.height);
        auto rectPath = tgfx::Path();
        rectPath.addRect(displayRect);

        tgfx::Paint paint = {};
        paint.setColor(_color);
        paint.setStyle(tgfx::PaintStyle::Fill);
        canvas->drawPath(rectPath, paint);
    }

  private:
    tgfx::Size _size{};
    tgfx::Color _color{0.0f, 0.0f, 0.0f, 0.6f};
};

class SeatMinimapLineBoxLayer : public tgfx::Layer {

  public:
    SeatMinimapLineBoxLayer(const tgfx::Rect &bounds, float lineWidth)
        : _bounds(bounds)
        , _lineWidth(lineWidth) {
        invalidateContent();
    }

    void updateBounds(const tgfx::Rect &bounds) {
        if (_bounds == bounds) {
            return;
        }
        _bounds = bounds;
        invalidateContent();
    }

    void updateLineWidth(float lineWidth) {
        if (_lineWidth == lineWidth) {
            return;
        }
        _lineWidth = lineWidth;
        invalidateContent();
    }

  protected:
    void onUpdateContent(tgfx::LayerRecorder *recorder) override {
        if (_bounds.isEmpty()) {
            return;
        }
        auto canvas = recorder->getCanvas(tgfx::LayerContentType::Default);

        tgfx::Paint paint = {};
        paint.setColor(tgfx::Color::Red());
        paint.setStyle(tgfx::PaintStyle::Stroke);
        paint.setStrokeWidth(_lineWidth);
        canvas->drawRect(_bounds, paint);
    }

  private:
    tgfx::Rect _bounds{};
    float _lineWidth{6};
};

SeatMinimapLayerTree::SeatMinimapLayerTree()
    : kk::drawers::Drawer("SeatMinimapLayerTree")
    , _root(nullptr)
    , _lineBox(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>()) {
}

SeatMinimapLayerTree::~SeatMinimapLayerTree() {
    tgfx::PrintLog("SeatMinimapLayerTree::~SeatMinimapLayerTree");
}

void SeatMinimapLayerTree::updateLineBox(const kk::SeatCraftCoreApp *app) {
    if (_lineBox == nullptr) {
        return;
    }

    // TODO: 需要应用缩放和位移
    auto zoomScale = app->zoomScale();
    _lineBox->updateBounds(tgfx::Rect::MakeSize(_contaierSize));
}

bool SeatMinimapLayerTree::updateContaierSize(const kk::SeatCraftCoreApp *app) {
    auto density = app->density();
    tgfx::Size newSize{140.0f * density, 140.0f * density};
    if (_contaierSize == newSize) {
        return false;
    }

    _contaierSize = newSize;
    return true;
}

std::shared_ptr<SeatMinimapContainerLayer> SeatMinimapLayerTree::buildLayerTree(const kk::SeatCraftCoreApp *app) {
    auto root = std::make_shared<SeatMinimapContainerLayer>(_contaierSize, tgfx::Color{0.0f, 0.0f, 0.0f, 0.6f});
    _lineBox = std::make_shared<SeatMinimapLineBoxLayer>(tgfx::Rect::MakeSize(_contaierSize), 6);
    root->addChild(_lineBox);
    return root;
}

void SeatMinimapLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    if (updateContaierSize(app) && _root != nullptr) {
        _root->updateSize(_contaierSize);
        updateLineBox(app);
    }

    if (_root == nullptr) {
        _root = buildLayerTree(app);
        _displayList->root()->addChild(_root);
        _displayList->setRenderMode(tgfx::RenderMode::Direct);
    }

    auto rootBounds = _root->getBounds();
    auto surface = canvas->getSurface();
    auto width = surface->width();
    auto transX = width - _contaierSize.width;
    auto matrix = tgfx::Matrix::MakeTrans(transX, 0);
    _root->setMatrix(matrix);

    _displayList->render(surface, false);
}

}  // namespace kk::drawers
