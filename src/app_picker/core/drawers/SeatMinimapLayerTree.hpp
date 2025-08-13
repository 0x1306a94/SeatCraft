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
class ShapeLayer;
class DisplayList;
};  // namespace tgfx

namespace kk::drawers {
class SeatMinimapLayerTree : public kk::drawers::Drawer {
  public:
    explicit SeatMinimapLayerTree();

    ~SeatMinimapLayerTree() override;
    bool hasContentChanged() const;
    void prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app, bool force) override;

  protected:
    void updateLineBox(const kk::SeatCraftCoreApp *app);
    bool updateContainerSize(const kk::SeatCraftCoreApp *app);
    virtual std::shared_ptr<tgfx::ShapeLayer> buildLayerTree(const kk::SeatCraftCoreApp *app);
    void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;

  private:
    tgfx::Size _viewSize{};
    tgfx::Size _containerSize{};
    std::shared_ptr<tgfx::ShapeLayer> _root{nullptr};
    std::shared_ptr<tgfx::ShapeLayer> _lineBox{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
};

}  // namespace kk::drawers

#endif /* SeatMinimapLayerTree_hpp */
