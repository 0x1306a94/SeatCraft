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
class SeatMinimapDrawer;
};  // namespace kk::drawers

namespace kk::renderer {
class RendererBackend;
class SeatCraftCoreRenderer {
  public:
    explicit SeatCraftCoreRenderer(std::shared_ptr<kk::SeatCraftCoreApp> app, std::unique_ptr<RendererBackend> backend);

    virtual ~SeatCraftCoreRenderer();

    virtual void updateSize();

    virtual void invalidateContent();

    virtual void draw(bool force = false);

  protected:
    std::shared_ptr<kk::SeatCraftCoreApp> app;
    std::unique_ptr<RendererBackend> backend;
    bool invalidate;
    std::unique_ptr<kk::drawers::GridBackgroundDrawer> gridBackground;
    std::unique_ptr<kk::drawers::ConicGradientDrawer> conicGradient;
    std::unique_ptr<kk::drawers::SeatMinimapDrawer> minimap;
};
};  // namespace kk::renderer

#endif /* SeatCraftCoreRenderer_hpp */
