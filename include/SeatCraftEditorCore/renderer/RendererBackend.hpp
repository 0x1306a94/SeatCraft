//
//  RendererBackend.hpp
//  SeatCraftEditorCore
//
//  Created by king on 2025/8/18.
//

#ifndef RendererBackend_hpp
#define RendererBackend_hpp

#include <memory>

#include <SeatCraftEditorCore/common/defines.h>

namespace tgfx {
class Window;
};

namespace kk::renderer {
class SEAT_CRAFT_EDITER_API RendererBackend {
  public:
    virtual ~RendererBackend() = default;
    virtual std::shared_ptr<tgfx::Window> getWindow() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual float getDensity() = 0;
};

};  // namespace kk::renderer

#endif /* RendererBackend_hpp */
