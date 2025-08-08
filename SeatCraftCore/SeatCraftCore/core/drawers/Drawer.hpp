//
//  Drawer.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#ifndef Drawer_hpp
#define Drawer_hpp

#include <string>

namespace tgfx {
class Canvas;
};

namespace kk {
class SeatCraftCoreApp;
};

namespace kk::drawers {
class Drawer {
  public:
    explicit Drawer(const std::string &name);

    virtual ~Drawer() = default;

    std::string name() const {
        return _name;
    }

    void draw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);

  protected:
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) = 0;

  private:
    std::string _name;
};
};  // namespace kk::drawers

#endif /* Drawer_hpp */
