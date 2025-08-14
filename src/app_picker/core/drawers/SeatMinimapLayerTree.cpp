//
//  SeatMinimapLayerTree.cpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/7.
//

#include "SeatMinimapLayerTree.hpp"

#include "core/EdgeInsets.h"
#include "core/SeatCraftCoreApp.hpp"
#include "core/svg/ConvertSVGLayer.hpp"

#include <SeatCraft/common/common_macro.h>

#include <tgfx/core/Canvas.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ImageLayer.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/platform/Print.h>
#include <tgfx/svg/SVGDOM.h>

namespace kk::drawers {

SeatMinimapLayerTree::SeatMinimapLayerTree()
    : kk::drawers::Drawer("SeatMinimapLayerTree")
    , _inset(kk::EdgeInsets{20.f, 20.f, 20.f, 20.f})
    , _root(nullptr)
    , _lineBox(nullptr)
    , _displayList(std::make_unique<tgfx::DisplayList>())
    , _areaCacheImage(nullptr) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

SeatMinimapLayerTree::~SeatMinimapLayerTree() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
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
        prebuildAreaImage(canvas, app);
    }

    if (rebuild) {
        if (_areaLayer) {
            _areaLayer->removeFromParent();
            _areaLayer = nullptr;
        }

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

    // 计算考虑 inset 后的 minimapSize
    tgfx::Size minimapSize{
        _containerSize.width - _inset.left - _inset.right,
        _containerSize.height - _inset.top - _inset.bottom,
    };

    auto zoomScale = app->zoomScale();
    auto contentOffset = app->contentOffset();

    float rectWidth = viewSize.width / contentSize.width / zoomScale * minimapSize.width;
    float rectHeight = viewSize.height / contentSize.height / zoomScale * minimapSize.height;

    // 计算指示框在 minimap 中的位置（考虑 _inset）
    float rectX = -contentOffset.x / contentSize.width / zoomScale * minimapSize.width + _inset.left;
    float rectY = -contentOffset.y / contentSize.height / zoomScale * minimapSize.height + _inset.top;

    // 边界处理
    if (rectWidth > minimapSize.width) {
        rectWidth = minimapSize.width;
        rectX = _inset.left;
    } else {
        rectX = std::max(_inset.left, std::min(rectX, minimapSize.width - rectWidth + _inset.left));
    }

    if (rectHeight > minimapSize.height) {
        rectHeight = minimapSize.height;
        rectY = _inset.top;
    } else {
        rectY = std::max(_inset.top, std::min(rectY, minimapSize.height - rectHeight + _inset.top));
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

bool SeatMinimapLayerTree::prebuildAreaImage(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) {
    auto areaSvgDom = app->getSvgDom();
    if (areaSvgDom == nullptr) {
        _areaCacheImage = nullptr;
        return false;
    }

    auto areaDomSize = areaSvgDom->getContainerSize();
    auto areaLayer = kk::svg::convertSVGDomToLayer(areaSvgDom);
    if (areaLayer == nullptr) {
        _areaCacheImage = nullptr;
        return false;
    }

    auto surfaceWidth = static_cast<int>(std::ceil(_containerSize.width));
    auto surfaceHeight = static_cast<int>(std::ceil(_containerSize.height));
    auto context = canvas->getSurface()->getContext();
    auto surface = tgfx::Surface::Make(context, surfaceWidth, surfaceHeight, tgfx::ColorType::RGBA_8888);
    auto scaleX = static_cast<float>(surfaceWidth) / areaDomSize.width;
    auto scaleY = static_cast<float>(surfaceHeight) / areaDomSize.height;
    auto fitScale = std::min(scaleX, scaleY);
    areaLayer->setMatrix(tgfx::Matrix::MakeScale(fitScale));
    tgfx::DisplayList displayList{};
    displayList.root()->addChild(areaLayer);
    displayList.render(surface.get(), true);
    _areaCacheImage = surface->makeImageSnapshot();
    return true;
}

std::shared_ptr<tgfx::ShapeLayer> SeatMinimapLayerTree::buildLayerTree(const kk::SeatCraftCoreApp *app) {

    UNUSED_PARAM(app);
    auto root = tgfx::ShapeLayer::Make();
    tgfx::Path rectPath{};
    rectPath.addRoundRect(tgfx::Rect::MakeSize(_containerSize), 12, 12);
    root->setPath(rectPath);
    root->setFillStyle(tgfx::SolidColor::Make(tgfx::Color{0.0f, 0.0f, 0.0f, 0.6f}));

    if (_areaCacheImage) {
        _areaLayer = tgfx::ImageLayer::Make();
        _areaLayer->setImage(_areaCacheImage);

        tgfx::Size minimapSize{
            _containerSize.width - _inset.left - _inset.right,
            _containerSize.height - _inset.top - _inset.bottom,
        };
        const float scaleX = minimapSize.width / static_cast<float>(_areaCacheImage->width());
        const float scaleY = minimapSize.height / static_cast<float>(_areaCacheImage->height());
        const float fitScale = std::min(scaleX, scaleY);
        auto matrix = tgfx::Matrix::MakeTrans(_inset.left, _inset.top);
        matrix.postScale(fitScale, fitScale);
        _areaLayer->setMatrix(matrix);
    }

    _lineBox = tgfx::ShapeLayer::Make();

    _lineBox->setStrokeAlign(tgfx::StrokeAlign::Inside);
    _lineBox->setStrokeStyle(tgfx::SolidColor::Make(tgfx::Color::Red()));
    _lineBox->setLineWidth(6);

    updateLineBox(app);
    root->addChild(_areaLayer);
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
