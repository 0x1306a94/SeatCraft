//
//  GridBackgroundLayerTree.hpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#ifndef GridBackgroundLayerTree_hpp
#define GridBackgroundLayerTree_hpp

#include "Drawer.hpp"

namespace tgfx {
class Layer;
class DisplayList;
};  // namespace tgfx

namespace kk::drawers {
class GridBackgroundLayer;
class GridBackgroundLayerTree : public kk::drawers::Drawer {
  public:
    explicit GridBackgroundLayerTree();

    virtual ~GridBackgroundLayerTree();

    bool hasContentChanged() const;

    virtual void prepare(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app, bool force) override;

  protected:
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app) override;

  private:
    std::shared_ptr<GridBackgroundLayer> _root{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
};

}  // namespace kk::drawers

#endif /* GridBackgroundLayerTree_hpp */
