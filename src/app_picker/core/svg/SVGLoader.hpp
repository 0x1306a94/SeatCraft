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
class Stream;
};  // namespace tgfx

namespace kk::svg {
std::shared_ptr<tgfx::SVGDOM> loadSvgDom(const std::string &path);
std::shared_ptr<tgfx::SVGDOM> loadSvgDom(tgfx::Stream *stream);
};  // namespace kk::svg

#endif /* SVGLoader_hpp */
