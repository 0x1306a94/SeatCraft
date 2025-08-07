//
//  SeatCraftCoreRenderer.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#ifndef SeatCraftCoreRenderer_hpp
#define SeatCraftCoreRenderer_hpp

#include <memory>

namespace tgfx {
class Window;
};

namespace kk {
class SeatCraftCoreApp;
};

namespace kk::drawers {
class GridBackgroundDrawer;
class ConicGradientDrawer;
};  // namespace kk::drawers

namespace kk::renderer {
class SeatCraftCoreRenderer {
  public:
    explicit SeatCraftCoreRenderer(const kk::SeatCraftCoreApp *app);

    virtual ~SeatCraftCoreRenderer();

    virtual void initWindow();

    virtual void updateSize();

    virtual void invalidateContent();

    virtual void draw();

  protected:
    const kk::SeatCraftCoreApp *app;
    std::shared_ptr<tgfx::Window> window;
    bool invalidate;
    std::unique_ptr<kk::drawers::GridBackgroundDrawer> gridBackground;
    std::unique_ptr<kk::drawers::ConicGradientDrawer> conicGradient;
};
};  // namespace kk::renderer

#endif /* SeatCraftCoreRenderer_hpp */
