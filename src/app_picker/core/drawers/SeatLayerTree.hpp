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
    void invalidateSeatStatusImage();
    bool hasContentChanged() const;
    virtual void prepare(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app, bool force) override;

  protected:
    /// 更新内容尺寸
    /// - Parameter app: app
    bool updateContentSize(const kk::SeatCraftCoreApp *app);
    bool prebuildSeatStatusImage(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);
    std::shared_ptr<tgfx::Layer> buildLayerTree(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);
    void updateRootMatrix(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app);
    void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftCoreApp *app) override;

  private:
    // 限制的内容尺寸，无论svg原始尺寸多大。都不能超过这个内容尺寸，如果超过了则等比例缩放
    tgfx::Size _contentSize{};
    // svg文件获取的原始尺寸
    tgfx::Size _areaDomSize{};
    std::shared_ptr<tgfx::Layer> _root{nullptr};
    std::shared_ptr<tgfx::Layer> _seatLayer{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
    std::unordered_map<kk::SeatStatusKey, std::shared_ptr<tgfx::Image>> _seatStatusImageMap{};
};

}  // namespace kk::drawers

#endif /* SeatLayerTree_hpp */
