//
//  SeatCraftEditorCoreApp.hpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#ifndef SeatCraftEditorCoreApp_hpp
#define SeatCraftEditorCoreApp_hpp

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

    bool updateBounds(const tgfx::Size &boundsSize, float density);

  private:
    /// canvas 尺寸
    tgfx::Size _boundsSize{1280, 720};
    float _density{1.0f};
};
};  // namespace kk

#endif /* SeatCraftEditorCoreApp_hpp */
