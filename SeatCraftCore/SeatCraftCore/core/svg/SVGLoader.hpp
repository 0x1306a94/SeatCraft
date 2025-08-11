//
//  SVGLoader.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/11.
//

#ifndef SVGLoader_hpp
#define SVGLoader_hpp

#include <memory>
#include <string>

namespace tgfx {
class SVGDOM;
};  // namespace tgfx

namespace kk::svg {
std::shared_ptr<tgfx::SVGDOM> loadSvgDom(const std::string &path);
};  // namespace kk::svg

#endif /* SVGLoader_hpp */
