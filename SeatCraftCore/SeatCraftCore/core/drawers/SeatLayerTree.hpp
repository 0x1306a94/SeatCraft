//
//  SeatLayerTree.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#ifndef SeatLayerTree_hpp
#define SeatLayerTree_hpp

#include "Drawer.hpp"

#include "../SeatStatusKey.h"

#include <memory>

#include <tgfx/core/Size.h>

namespace tgfx {
class Layer;
class DisplayList;
class Image;
};  // namespace tgfx

namespace kk::drawers {
class SeatLayerTree : public kk::drawers::Drawer {
  public:
    explicit SeatLayerTree();

    virtual ~SeatLayerTree();
    bool hasContentChanged() const;
    virtual void prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app, bool force) override;

  protected:
    bool updateContentSize(const kk::SeatCraftCoreApp *app);
    bool prebuildSeatStatusBitmap(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);
    std::shared_ptr<tgfx::Layer> buildLayerTree(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);
    void updateRootMatrix(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);
    void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;

  private:
    tgfx::Size _contentSize{};
    tgfx::Size _areaDomSize{};
    std::shared_ptr<tgfx::Layer> _root{nullptr};
    std::shared_ptr<tgfx::Layer> _seatLayer{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
    std::unordered_map<kk::SeatStatusKey, std::shared_ptr<tgfx::Image>> _seatStatusImageMap{};
};

}  // namespace kk::drawers

#endif /* SeatLayerTree_hpp */
