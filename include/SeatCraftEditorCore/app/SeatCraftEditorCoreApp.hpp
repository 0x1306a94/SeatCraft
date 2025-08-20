//
//  SeatCraftEditorCoreApp.hpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#ifndef SeatCraftEditorCoreApp_hpp
#define SeatCraftEditorCoreApp_hpp

#include <tgfx/core/Point.h>
#include <tgfx/core/Size.h>

#include <SeatCraftEditorCore/common/defines.h>

namespace kk {
class SEAT_CRAFT_EDITER_API SeatCraftEditorCoreApp {
  public:
    explicit SeatCraftEditorCoreApp(const tgfx::Size &boundsSize = {1280, 720}, float density = 1.0f);
    ~SeatCraftEditorCoreApp();

    SeatCraftEditorCoreApp(const SeatCraftEditorCoreApp &) = delete;
    SeatCraftEditorCoreApp &operator=(const SeatCraftEditorCoreApp &) = delete;

    /// canvas 尺寸
    const tgfx::Size &getBoundsSize() const;

    float density() const;

    /// 当前缩放比例
    float zoomScale() const;

    /// 当前滑动偏移
    const tgfx::Point &contentOffset() const;

    bool updateBounds(const tgfx::Size &boundsSize, float density);
    bool updateZoomAndOffset(float zoomScale, const tgfx::Point &contentOffset);

  private:
    /// canvas 尺寸
    tgfx::Size _boundsSize{1280, 720};
    float _density{1.0f};
    float _zoomScale = 1.0f;
    tgfx::Point _contentOffset = {};
};
};  // namespace kk

#endif /* SeatCraftEditorCoreApp_hpp */
