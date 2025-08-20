//
//  SeatCraftEditorCoreRenderer.hpp
//  SeatCraftEditorCore
//
//  Created by king on 2025/8/18.
//

#ifndef SeatCraftEditorCoreRenderer_hpp
#define SeatCraftEditorCoreRenderer_hpp

#include <memory>

#include <SeatCraftEditorCore/common/defines.h>

namespace tgfx {
class Window;
};

namespace kk {
class SeatCraftEditorCoreApp;

namespace drawers {
class GridBackgroundLayerTree;
class UserDesignLayerTree;
};  // namespace drawers
};  // namespace kk

namespace kk::renderer {
class RendererBackend;
class SEAT_CRAFT_EDITER_API SeatCraftEditorCoreRenderer {
  public:
    explicit SeatCraftEditorCoreRenderer(std::shared_ptr<kk::SeatCraftEditorCoreApp> app, std::shared_ptr<RendererBackend> backend);

    ~SeatCraftEditorCoreRenderer();

    std::shared_ptr<RendererBackend> getRendererBackend();

    void replaceBackend(std::shared_ptr<RendererBackend> backend);

    bool updateSize();

    void invalidateContent();

    void draw(bool force = false);

  private:
    std::shared_ptr<kk::SeatCraftEditorCoreApp> _app;
    std::shared_ptr<RendererBackend> _backend;
    std::unique_ptr<kk::drawers::GridBackgroundLayerTree> _gridLayer;
    std::unique_ptr<kk::drawers::UserDesignLayerTree> _designLayerTree;
    bool _invalidate;
};
};  // namespace kk::renderer

#endif /* SeatCraftEditorCoreRenderer_hpp */
