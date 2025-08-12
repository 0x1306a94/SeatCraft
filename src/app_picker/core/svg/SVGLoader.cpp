//
//  SVGLoader.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/11.
//

#include "SVGLoader.hpp"

#include <tgfx/svg/SVGDOM.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace kk::svg {
std::shared_ptr<tgfx::SVGDOM> loadSvgDom(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    if (!fs::exists(path)) {
        return nullptr;
    }

    auto stream = tgfx::Stream::MakeFromFile(path);
    if (!stream) {
        return nullptr;
    }

    auto svgDom = tgfx::SVGDOM::Make(*stream);
    return svgDom;
}

};  // namespace kk::svg
