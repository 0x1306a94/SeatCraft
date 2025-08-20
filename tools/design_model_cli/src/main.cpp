
#include <SeatCraft/common/common_macro.h>
#include <SeatCraftDesignModel/serialize/SVGSerialize.hpp>

#include <tgfx/gpu/opengl/GLDevice.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/SolidColor.h>
#include <tgfx/layers/SolidLayer.h>

#include <iostream>

int main(int argc, const char *argv[]) {
    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);

    auto root = tgfx::Layer::Make();

    do {
        tgfx::Path path;
        path.addRect(tgfx::Rect{100, 100, 200, 200});
        auto rectLayer = tgfx::ShapeLayer::Make();
        rectLayer->setPath(path);
        rectLayer->addFillStyle(tgfx::SolidColor::Make(tgfx::Color::Red()));
        rectLayer->addStrokeStyle(tgfx::SolidColor::Make(tgfx::Color::White()));
        rectLayer->setLineWidth(6);
        root->addChild(rectLayer);
    } while (0);
    auto device = tgfx::GLDevice::Make();
    auto serialize = kk::design::SVGSerialize::Make(device);
    auto svgString = serialize->serialize(root);

    std::cerr << svgString;
    return 0;
}
