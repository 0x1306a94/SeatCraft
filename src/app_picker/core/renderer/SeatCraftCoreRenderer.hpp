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
class GridBackgroundLayerTree;
class SeatLayerTree;
class SeatMinimapLayerTree;
};  // namespace kk::drawers

namespace kk::renderer {
class RendererBackend;
class SeatCraftCoreRenderer {
  public:
    explicit SeatCraftCoreRenderer(std::shared_ptr<kk::SeatCraftCoreApp> app, std::unique_ptr<RendererBackend> backend);

    ~SeatCraftCoreRenderer();

    void replaceBackend(std::unique_ptr<RendererBackend> backend);

    bool updateSize();

    void invalidateContent();

    void draw(bool force = false);

  private:
    std::shared_ptr<kk::SeatCraftCoreApp> _app;
    std::unique_ptr<RendererBackend> _backend;
    bool _invalidate;
    std::unique_ptr<kk::drawers::GridBackgroundLayerTree> _gridLayer;
    std::unique_ptr<kk::drawers::SeatLayerTree> _seatLayer;
    std::unique_ptr<kk::drawers::SeatMinimapLayerTree> _minimapLayer;
};
};  // namespace kk::renderer

#endif /* SeatCraftCoreRenderer_hpp */
