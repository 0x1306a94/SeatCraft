//
//  SeatMinimapLayerTree.hpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/7.
//

#ifndef SeatMinimapLayerTree_hpp
#define SeatMinimapLayerTree_hpp

#include "Drawer.hpp"

#include <memory>

#include <tgfx/core/Size.h>

namespace tgfx {
class Layer;
class DisplayList;
};  // namespace tgfx

namespace kk::drawers {
class SeatMinimapContainerLayer;
class SeatMinimapLineBoxLayer;
class SeatMinimapLayerTree : public kk::drawers::Drawer {
  public:
    explicit SeatMinimapLayerTree();

    virtual ~SeatMinimapLayerTree();
    bool hasContentChanged() const;
    virtual void prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app, bool force) override;

  protected:
    void updateLineBox(const kk::SeatCraftCoreApp *app);
    bool updateContaierSize(const kk::SeatCraftCoreApp *app);
    virtual std::shared_ptr<SeatMinimapContainerLayer> buildLayerTree(const kk::SeatCraftCoreApp *app);
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;

  private:
    tgfx::Size _contaierSize{};
    std::shared_ptr<SeatMinimapContainerLayer> _root{nullptr};
    std::shared_ptr<SeatMinimapLineBoxLayer> _lineBox{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
};

}  // namespace kk::drawers

#endif /* SeatMinimapLayerTree_hpp */
