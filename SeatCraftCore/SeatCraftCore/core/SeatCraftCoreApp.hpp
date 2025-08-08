//
//  SeatCraftCoreApp.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#ifndef SeatCraftCoreApp_hpp
#define SeatCraftCoreApp_hpp

#include <string>
#include <unordered_map>

#include <tgfx/core/Point.h>
#include <tgfx/core/Size.h>

#include "SeatStatusKey.h"

namespace kk {
class SeatCraftCoreApp {
  public:
    explicit SeatCraftCoreApp(const tgfx::Size &boundSize = tgfx::Size{1280, 720}, const tgfx::Size &contentSize = tgfx::Size{0, 0}, float density = 1.0f);

    ~SeatCraftCoreApp();

    tgfx::Size getBoundsSize() const;

    tgfx::Size getContentSize() const;

    std::string getAreaSvgPath() const;

    const kk::SeatStatusSVGPathMap &getSeatStatusSvgMap() const;

    float density() const {
        return _density;
    }

    float zoomScale() const {
        return _zoomScale;
    }

    const tgfx::Point &contentOffset() const {
        return _contentOffset;
    }

    bool updateContentSize(const tgfx::Size &contentSize);
    bool updateScreen(const tgfx::Size &boundSize, float density);
    bool updateZoomAndOffset(float zoomScale, const tgfx::Point &contentOffset);
    bool updateAreaSvgPath(const std::string &path);
    bool updateSeatStatusSVGPathMap(kk::SeatStatusSVGPathMap map);

  private:
    tgfx::Size _boundSize{1280, 720};
    tgfx::Size _contentSize{0, 0};
    float _density = 1.0f;
    float _zoomScale = 1.0f;
    tgfx::Point _contentOffset = {};
    std::string _areaSvgPath{};
    kk::SeatStatusSVGPathMap _seatStatusSvgMap{};
};

};  // namespace kk

#endif /* SeatCraftCoreApp_hpp */
