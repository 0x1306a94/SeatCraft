//
//  GestureController.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#ifndef GestureController_hpp
#define GestureController_hpp

#include <tgfx/core/Point.h>
#include <tgfx/core/Size.h>

namespace kk::ui {
class GestureController {
  public:
    GestureController();
    // 更新视图的边界尺寸，这会影响缩放和滑动范围
    void setBoundsSize(const tgfx::Size &size);

    // 设置内容的原始尺寸
    void setContentSize(const tgfx::Size &size);

    // 设置最小和最大缩放比例
    void setZoomScales(float minScale, float maxScale);

    // 设置缩放比例
    void setZoomScale(float scale);

    // 更新手势平移量
    void pan(const tgfx::Point &translation, bool isBegan);

    // 更新手势缩放量
    void pinch(float scale, const tgfx::Point &center, bool isBegan);

    // 获取当前缩放比例
    float getZoomScale() const;
    // 获取当前偏移量
    tgfx::Point getContentOffset() const;

  private:
    // 计算并约束平移量
    void clampContentOffset();

    // 状态
    tgfx::Size boundsSize;
    tgfx::Size contentSize;
    float minimumZoomScale;
    float maximumZoomScale;

    float zoomScale;
    tgfx::Point contentOffset;

    // 用于手势计算的临时变量
    float currentZoom;
    tgfx::Point currentPanOffset;
    tgfx::Point currentPinchOffset;
    tgfx::Point pinchCenter;
};
};  // namespace kk::ui

#endif /* GestureController_hpp */
