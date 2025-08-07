//
//  ConicGradientDrawer.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#ifndef ConicGradientDrawer_hpp
#define ConicGradientDrawer_hpp

#include "Drawer.hpp"

namespace kk::drawers {
class ConicGradientDrawer : public kk::drawers::Drawer {
  public:
    explicit ConicGradientDrawer();

    virtual ~ConicGradientDrawer();

  protected:
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;
};

}  // namespace kk::drawers

#endif /* ConicGradientDrawer_hpp */
