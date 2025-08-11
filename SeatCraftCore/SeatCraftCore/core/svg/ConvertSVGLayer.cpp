//
//  ConvertSVGLayer.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/11.
//

#include "ConvertSVGLayer.hpp"

#include <tgfx/core/Path.h>
#include <tgfx/core/Rect.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ImageLayer.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/layers/TextLayer.h>
#include <tgfx/svg/SVGDOM.h>
#include <tgfx/svg/node/SVGCircle.h>
#include <tgfx/svg/node/SVGGroup.h>
#include <tgfx/svg/node/SVGPath.h>
#include <tgfx/svg/node/SVGPoly.h>
#include <tgfx/svg/node/SVGRect.h>
#include <tgfx/svg/node/SVGText.h>

namespace kk::svg {
static void applyShapeLayerStyle(tgfx::ShapeLayer *shape, tgfx::SVGNode *node) {
    if (auto attribute = node->getFill().get(); attribute) {
        if (attribute->type() == tgfx::SVGPaint::Type::Color) {
            auto color = attribute->color().color();
            shape->addFillStyle(tgfx::SolidColor::Make(color));
        }
    }

    if (auto attribute = node->getStroke().get(); attribute) {
        if (attribute->type() == tgfx::SVGPaint::Type::Color) {
            auto color = attribute->color().color();
            shape->addStrokeStyle(tgfx::SolidColor::Make(color));
        }
    }

    if (auto attribute = node->getStrokeDashArray().get(); attribute) {
        std::vector<float> dash{};
        for (const auto &item : attribute.value().dashArray()) {
            dash.push_back(item.value());
        }
        shape->setLineDashPattern(dash);
    }

    if (auto attribute = node->getStrokeDashOffset().get(); attribute) {
        shape->setLineDashPhase(attribute.value().value());
    }

    if (auto attribute = node->getStrokeLineCap().get(); attribute) {
        switch (attribute.value()) {
            case tgfx::SVGLineCap::Butt:
                shape->setLineCap(tgfx::LineCap::Butt);
                break;
            case tgfx::SVGLineCap::Round:
                shape->setLineCap(tgfx::LineCap::Round);
                break;
            case tgfx::SVGLineCap::Square:
                shape->setLineCap(tgfx::LineCap::Square);
                break;
            default:
                break;
        }
    }

    if (auto attribute = node->getStrokeLineJoin().get(); attribute) {
        switch (attribute.value().type()) {
            case tgfx::SVGLineJoin::Type::Miter:
                shape->setLineJoin(tgfx::LineJoin::Miter);
                break;
            case tgfx::SVGLineJoin::Type::Round:
                shape->setLineJoin(tgfx::LineJoin::Round);
                break;
            case tgfx::SVGLineJoin::Type::Bevel:
                shape->setLineJoin(tgfx::LineJoin::Bevel);
                break;
            default:
                break;
        }
    }

    if (auto attribute = node->getStrokeMiterLimit().get(); attribute) {
        shape->setMiterLimit(attribute.value());
    }

    if (auto attribute = node->getStrokeWidth().get(); attribute) {
        shape->setLineWidth(attribute.value().value());
    }
}

std::shared_ptr<tgfx::Layer> convertSVGDomToLayer(std::shared_ptr<tgfx::SVGDOM> dom) {
    if (dom == nullptr) {
        return nullptr;
    }

    auto &rootNode = dom->getRoot();
    if (!rootNode->hasChildren()) {
        return nullptr;
    }

    auto root = tgfx::Layer::Make();
    auto &childrens = rootNode->getChildren();
    for (const auto &child : childrens) {
        auto layer = convertSVGNodeToLayer(child.get());
        if (layer) {
            root->addChild(layer);
        }
    }
    return root;
}

std::shared_ptr<tgfx::Layer> convertSVGNodeToLayer(tgfx::SVGNode *node) {

    auto tag = node->tag();
    switch (tag) {
        case tgfx::SVGTag::G:
            return convertGroup(static_cast<tgfx::SVGGroup *>(node));
        case tgfx::SVGTag::Circle:
            return convertCircle(static_cast<tgfx::SVGCircle *>(node));
        case tgfx::SVGTag::Rect:
            return convertRect(static_cast<tgfx::SVGRect *>(node));
        case tgfx::SVGTag::Path:
            return convertPath(static_cast<tgfx::SVGPath *>(node));
        case tgfx::SVGTag::Polygon:
            return converPoly(static_cast<tgfx::SVGPoly *>(node));
        case tgfx::SVGTag::Text:
            return convertText(static_cast<tgfx::SVGText *>(node));
        default:
            break;
    }

    return nullptr;
}

std::shared_ptr<tgfx::Layer> convertGroup(tgfx::SVGGroup *node) {
    if (!node->hasChildren()) {
        return nullptr;
    }
    auto root = tgfx::Layer::Make();
    for (auto const &child : node->getChildren()) {
        auto layer = convertSVGNodeToLayer(child.get());
        if (layer) {
            root->addChild(layer);
        }
    }
    return root;
}

std::shared_ptr<tgfx::ShapeLayer> convertCircle(tgfx::SVGCircle *node) {

    return nullptr;
}

std::shared_ptr<tgfx::ShapeLayer> convertPath(tgfx::SVGPath *node) {
    auto shape = tgfx::ShapeLayer::Make();
    shape->setPath(node->getShapePath());
    applyShapeLayerStyle(shape.get(), node);
    return shape;
}

std::shared_ptr<tgfx::ShapeLayer> converPoly(tgfx::SVGPoly *node) {
    auto points = node->getPoints();
    if (points.empty()) {
        return nullptr;
    }
    auto shape = tgfx::ShapeLayer::Make();
    tgfx::Path path{};
    path.moveTo(points[0]);
    for (uint32_t i = 1; i < points.size(); i++) {
        path.lineTo(points[i]);
    }
    path.close();

    applyShapeLayerStyle(shape.get(), node);
    return shape;
}

std::shared_ptr<tgfx::ShapeLayer> convertRect(tgfx::SVGRect *node) {

    auto shape = tgfx::ShapeLayer::Make();
    auto x = node->getX().value();
    auto y = node->getY().value();
    auto width = node->getWidth().value();
    auto height = node->getHeight().value();

    tgfx::Path rectPath{};
    rectPath.addRect(tgfx::Rect::MakeXYWH(x, y, width, height));
    shape->setPath(rectPath);

    applyShapeLayerStyle(shape.get(), node);

    return shape;
}

std::shared_ptr<tgfx::TextLayer> convertText(tgfx::SVGText *node) {
    //    auto layer = tgfx::TextLayer::Make();

    return nullptr;
}

};  // namespace kk::svg
