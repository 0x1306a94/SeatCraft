//
//  SeatCraftCoreApp.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#ifndef SeatCraftCoreApp_hpp
#define SeatCraftCoreApp_hpp

#include <stdio.h>

#include <tgfx/core/Point.h>

namespace kk {
class SeatCraftCoreApp {
  public:
    explicit SeatCraftCoreApp(int width = 1280, int height = 720, float density = 1.0f);

    virtual ~SeatCraftCoreApp();

    int width() const {
        return _width;
    }

    int height() const {
        return _height;
    }

    float density() const {
        return _density;
    }

    float zoomScale() const {
        return _zoomScale;
    }

    const tgfx::Point &contentOffset() const {
        return _contentOffset;
    }

    bool updateScreen(int width, int height, float density);
    bool updateZoomAndOffset(float zoomScale, const tgfx::Point &contentOffset);

  private:
    int _width = 1280;
    int _height = 720;
    float _density = 1.0f;
    float _zoomScale = 1.0f;
    tgfx::Point _contentOffset = {};
};

};  // namespace kk

#endif /* SeatCraftCoreApp_hpp */
