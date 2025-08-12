//
//  ConvertSVGLayer.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/11.
//

#ifndef ConvertSVGLayer_hpp
#define ConvertSVGLayer_hpp

#include <memory>

namespace tgfx {
class SVGDOM;
class SVGNode;
class SVGCircle;
class SVGPath;
class SVGPoly;
class SVGRect;
class SVGText;
class SVGGroup;

class Layer;
class ShapeLayer;
class TextLayer;
};  // namespace tgfx

namespace kk::svg {
std::shared_ptr<tgfx::Layer> convertSVGDomToLayer(std::shared_ptr<tgfx::SVGDOM> dom);
std::shared_ptr<tgfx::Layer> convertSVGNodeToLayer(tgfx::SVGNode *node);
std::shared_ptr<tgfx::Layer> convertGroup(tgfx::SVGGroup *node);
std::shared_ptr<tgfx::ShapeLayer> convertCircle(tgfx::SVGCircle *node);
std::shared_ptr<tgfx::ShapeLayer> convertPath(tgfx::SVGPath *node);
std::shared_ptr<tgfx::ShapeLayer> converPoly(tgfx::SVGPoly *node);
std::shared_ptr<tgfx::ShapeLayer> convertRect(tgfx::SVGRect *node);
std::shared_ptr<tgfx::TextLayer> convertText(tgfx::SVGText *node);
};  // namespace kk::svg

#endif /* ConvertSVGLayer_hpp */
