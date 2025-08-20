//
//  SVGSerialize.cpp
//  SeatCraft
//
//  Created by king on 2025/8/20.
//

#include <SeatCraftDesignModel/serialize/SVGSerialize.hpp>

#include <tgfx/core/Path.h>
#include <tgfx/gpu/opengl/GLDevice.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/Layer.h>
#include <tgfx/svg/SVGExporter.h>

#include <nlohmann/json.hpp>

namespace kk::design {

std::shared_ptr<SVGSerialize> SVGSerialize::Make(std::shared_ptr<tgfx::GLDevice> device) {
    return std::shared_ptr<SVGSerialize>(new SVGSerialize(device));
}

SVGSerialize::SVGSerialize(std::shared_ptr<tgfx::GLDevice> device)
    : _device(device) {
}

std::string SVGSerialize::serializeImpl(std::shared_ptr<tgfx::Layer> layer) {
    if (_device == nullptr) {
        return "";
    }

    auto context = _device->lockContext();
    if (!context) {
        return "";
    }

    auto viewBox = tgfx::Rect::MakeWH(400, 400);
    auto SVGStream = tgfx::MemoryWriteStream::Make();
    auto exporter = tgfx::SVGExporter::Make(SVGStream, context, viewBox, tgfx::SVGExportFlags::DisablePrettyXML);
    auto *canvas = exporter->getCanvas();
    if (canvas == nullptr) {
        return "";
    }

    auto *surface = canvas->getSurface();
    if (surface == nullptr) {
        return "";
    }
    tgfx::DisplayList displayList;
    displayList.setRenderMode(tgfx::RenderMode::Direct);
    displayList.root()->addChild(layer);
    displayList.render(surface, false);
    auto svgString = SVGStream->readString();
    return svgString;
}

};  // namespace kk::design
