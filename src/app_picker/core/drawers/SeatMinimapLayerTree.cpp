//
//  SeatMinimapLayerTree.cpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/7.
//

#include "SeatMinimapLayerTree.hpp"

#include "core/EdgeInsets.h"
#include "core/SeatCraftCoreApp.hpp"

#include <SeatCraft/common/common_macro.h>

#include <tgfx/core/Canvas.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/platform/Print.h>

namespace kk::drawers {

SeatMinimapLayerTree::SeatMinimapLayerTree()
    : kk::drawers::Drawer("SeatMinimapLayerTree")
    , _root(nullptr)
    , _lineBox(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>()) {
}

SeatMinimapLayerTree::~SeatMinimapLayerTree() {
    tgfx::PrintLog("SeatMinimapLayerTree::~SeatMinimapLayerTree");
}

bool SeatMinimapLayerTree::hasContentChanged() const {
    if (_root == nullptr) {
        return true;
    }
    return _displayList->hasContentChanged();
}

void SeatMinimapLayerTree::prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app, bool force) {
    UNUSED_PARAM(canvas);
    UNUSED_PARAM(force);
    bool rebuild = updateContainerSize(app);
    auto viewSize = app->getBoundsSize();
    if (_viewSize != viewSize) {
        _viewSize = viewSize;
        rebuild = true;
    }

    if (rebuild) {
        if (_lineBox) {
            _lineBox->removeFromParent();
            _lineBox = nullptr;
        }

        if (_root) {
            _root->removeFromParent();
            _root = nullptr;
        }
    }

    updateLineBox(app);
}

void SeatMinimapLayerTree::updateLineBox(const kk::SeatCraftCoreApp *app) {
    UNUSED_PARAM(app);
    if (_lineBox == nullptr) {
        return;
    }
    auto viewSize = app->getBoundsSize();
    auto contentSize = app->getContentSize();
    if (viewSize.isEmpty() || contentSize.isEmpty()) {
        return;
    }

    kk::EdgeInsets inset{20.f, 20.f, 20.f, 20.f};

    // 计算考虑 inset 后的 minimapSize
    tgfx::Size minimapSize{
        _containerSize.width - inset.left - inset.right,
        _containerSize.height - inset.top - inset.bottom,
    };

    auto zoomScale = app->zoomScale();
    auto contentOffset = app->contentOffset();

    float rectWidth = viewSize.width / contentSize.width / zoomScale * minimapSize.width;
    float rectHeight = viewSize.height / contentSize.height / zoomScale * minimapSize.height;

    // 计算指示框在 minimap 中的位置（考虑 inset）
    float rectX = -contentOffset.x / contentSize.width / zoomScale * minimapSize.width + inset.left;
    float rectY = -contentOffset.y / contentSize.height / zoomScale * minimapSize.height + inset.top;

    // 边界处理
    if (rectWidth > minimapSize.width) {
        rectWidth = minimapSize.width;
        rectX = inset.left;
    } else {
        rectX = std::max(inset.left, std::min(rectX, minimapSize.width - rectWidth + inset.left));
    }

    if (rectHeight > minimapSize.height) {
        rectHeight = minimapSize.height;
        rectY = inset.top;
    } else {
        rectY = std::max(inset.top, std::min(rectY, minimapSize.height - rectHeight + inset.top));
    }

    auto mapRect = tgfx::Rect::MakeXYWH(rectX, rectY, rectWidth, rectHeight);
    tgfx::Path rectPath{};
    rectPath.addRect(mapRect);
    _lineBox->setPath(rectPath);
}

bool SeatMinimapLayerTree::updateContainerSize(const kk::SeatCraftCoreApp *app) {
    auto density = app->density();
    auto contentSize = app->getContentSize();
    float width = std::ceil(160.0f * density);
    tgfx::Size newSize{width, width};
    if (!contentSize.isEmpty()) {
        newSize.height = std::ceil(contentSize.height / contentSize.width * newSize.width);
    }

    if (_containerSize == newSize) {
        return false;
    }

    _containerSize = newSize;
    return true;
}

std::shared_ptr<tgfx::ShapeLayer> SeatMinimapLayerTree::buildLayerTree(const kk::SeatCraftCoreApp *app) {

    UNUSED_PARAM(app);
    auto root = tgfx::ShapeLayer::Make();
    tgfx::Path rectPath{};
    rectPath.addRoundRect(tgfx::Rect::MakeSize(_containerSize), 12, 12);
    root->setPath(rectPath);
    root->setFillStyle(tgfx::SolidColor::Make(tgfx::Color{0.0f, 0.0f, 0.0f, 0.6f}));

    _lineBox = tgfx::ShapeLayer::Make();

    _lineBox->setStrokeAlign(tgfx::StrokeAlign::Inside);
    _lineBox->setStrokeStyle(tgfx::SolidColor::Make(tgfx::Color::Red()));
    _lineBox->setLineWidth(6);

    updateLineBox(app);
    root->addChild(_lineBox);
    return root;
}

void SeatMinimapLayerTree::onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {

    auto surface = canvas->getSurface();

    if (_root == nullptr) {
        _root = buildLayerTree(app);
        auto bounds = _root->getBounds();
        auto width = surface->width();
        float inset = 20.f;
        auto transX = static_cast<float>(width) - bounds.width() - inset;
        auto matrix = tgfx::Matrix::MakeTrans(transX, inset);
        _root->setMatrix(matrix);

        _displayList->root()->addChild(_root);
        _displayList->setRenderMode(tgfx::RenderMode::Direct);
    }

    _displayList->render(surface, false);
}

}  // namespace kk::drawers
