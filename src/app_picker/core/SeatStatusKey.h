//
//  SeatStatusKey.h
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#ifndef SeatStatusKey_h
#define SeatStatusKey_h

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace tgfx {
class Data;
class SVGDOM;
}  // namespace tgfx

namespace kk {
using SeatStatusKey = uint32_t;
using SeatStatusSVGDataMap = std::unordered_map<kk::SeatStatusKey, std::shared_ptr<tgfx::Data>>;
using SeatStatusSVGDOMMap = std::unordered_map<kk::SeatStatusKey, std::shared_ptr<tgfx::SVGDOM>>;
};  // namespace kk

#endif /* SeatStatusKey_h */