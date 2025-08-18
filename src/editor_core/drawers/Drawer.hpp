//
//  Drawer.hpp
//  SeatCraftEditor
//
//  Created by king on 2025/8/18.
//

#ifndef Drawer_hpp
#define Drawer_hpp

#include <string>

namespace tgfx {
class Canvas;
};

namespace kk {
class SeatCraftEditorCoreApp;
};

namespace kk::drawers {
class Drawer {
  public:
    explicit Drawer(const std::string &name);
    virtual ~Drawer() = default;
    std::string name() const {
        return _name;
    }

    virtual void prepare(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app, bool force = false) = 0;
    void draw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app);

  protected:
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app) = 0;

  private:
    std::string _name;
};
};  // namespace kk::drawers

#endif /* Drawer_hpp */
