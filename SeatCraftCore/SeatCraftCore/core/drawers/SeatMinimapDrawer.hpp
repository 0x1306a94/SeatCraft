//
//  SeatMinimapDrawer.hpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/7.
//

#ifndef SeatMinimapDrawer_hpp
#define SeatMinimapDrawer_hpp

#include "Drawer.hpp"

#include <memory>

namespace tgfx {
class Layer;
class DisplayList;
};  // namespace tgfx

namespace kk::drawers {
class SeatMinimapDrawer : public kk::drawers::Drawer {
  public:
    explicit SeatMinimapDrawer();

    virtual ~SeatMinimapDrawer();

  protected:
  protected:
    virtual std::shared_ptr<tgfx::Layer> buildLayerTree(const kk::SeatCraftCoreApp *app);
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;

  private:
    std::shared_ptr<tgfx::Layer> root{nullptr};
    std::unique_ptr<tgfx::DisplayList> displayList{nullptr};
};

}  // namespace kk::drawers

#endif /* SeatMinimapDrawer_hpp */
