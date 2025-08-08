//
//  SeatLayerTree.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#ifndef SeatLayerTree_hpp
#define SeatLayerTree_hpp

#include "Drawer.hpp"

#include <memory>

#include <tgfx/core/Size.h>

namespace tgfx {
class Layer;
class DisplayList;
};  // namespace tgfx

namespace kk::drawers {
class SeatAreaLayer;
class SeatLayerTree : public kk::drawers::Drawer {
  public:
    explicit SeatLayerTree();

    virtual ~SeatLayerTree();

  protected:
    void updateRootMatrix(const kk::SeatCraftCoreApp *app);
    bool updateContentSize(const kk::SeatCraftCoreApp *app);
    std::shared_ptr<tgfx::Layer> buildLayerTree(const kk::SeatCraftCoreApp *app);
    void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;

  private:
    tgfx::Size _contentSize{};
    std::shared_ptr<tgfx::Layer> _root{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
    std::shared_ptr<SeatAreaLayer> _areaLayer{nullptr};
};

}  // namespace kk::drawers

#endif /* SeatLayerTree_hpp */
