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
    explicit SeatCraftEditorCoreApp(int boundsWidth = 1280, int boundsHeight = 720, float density = 1.0f);
    ~SeatCraftEditorCoreApp();

    SeatCraftEditorCoreApp(const SeatCraftEditorCoreApp &) = delete;
    SeatCraftEditorCoreApp &operator=(const SeatCraftEditorCoreApp &) = delete;

    /// canvas 尺寸
    int getBoundsWidth() const;
    int getBoundsHeight() const;
    float density() const;

    bool updateBounds(int width, int height, float density);

  private:
    /// canvas 尺寸
    int _boundsWidth{1280};
    int _boundsHeight{720};
    float _density{1.0f};
};
};  // namespace kk

#endif /* SeatCraftEditorCoreApp_hpp */
