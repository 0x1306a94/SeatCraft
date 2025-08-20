//
//  UserDesignLayerTree.hpp
//  SeatCraftDesignModel
//
//  Created by king on 2025/8/20.
//

#ifndef UserDesignLayerTree_hpp
#define UserDesignLayerTree_hpp

#include "Drawer.hpp"

#include <vector>

namespace tgfx {
class Layer;
class DisplayList;
};  // namespace tgfx

namespace kk::drawers {
class UserDesignLayerTree : public kk::drawers::Drawer {

  public:
    explicit UserDesignLayerTree();

    virtual ~UserDesignLayerTree();

    std::vector<std::shared_ptr<tgfx::Layer>> getLayersUnderPoint(float x, float y);

    bool hasContentChanged() const;

    virtual void prepare(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app, bool force) override;

  protected:
    virtual void onDraw(tgfx::Canvas *canvas, const kk::SeatCraftEditorCoreApp *app) override;

  private:
    std::shared_ptr<tgfx::Layer> _root{nullptr};
    std::unique_ptr<tgfx::DisplayList> _displayList{nullptr};
};

};  // namespace kk::drawers

#endif /* UserDesignLayerTree_hpp */
