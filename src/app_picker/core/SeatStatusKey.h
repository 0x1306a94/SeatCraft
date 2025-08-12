//
//  SeatStatusKey.h
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#ifndef SeatStatusKey_h
#define SeatStatusKey_h

#include <cstdint>
#include <string>
#include <unordered_map>

namespace kk {
using SeatStatusKey = uint8_t;
using SeatStatusSVGPathMap = std::unordered_map<kk::SeatStatusKey, std::string>;
};  // namespace kk

#endif /* SeatStatusKey_h */
