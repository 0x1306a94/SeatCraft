//
//  Layer.hpp
//  SeatCraftDesignModel
//
//  Created by KK on 2025/8/19.
//

#ifndef Layer_hpp
#define Layer_hpp

#include <memory>
#include <string>
#include <vector>

#include "layer/LayerType.h"

namespace kk::design {
class Layer : public std::enable_shared_from_this<Layer> {
  public:
    static std::shared_ptr<Layer> Make();

    virtual ~Layer();

    virtual LayerType type() const {
        return LayerType::Layer;
    }

    const std::string &name() const {
        return _name;
    }

    void setName(const std::string &value) {
        _name = value;
    }

    bool visible() const {
        return bitFields.visible;
    }

    void setVisible(bool value);

    Layer *parent() const {
        return _parent;
    }

    const std::vector<std::shared_ptr<Layer>> &children() const {
        return _children;
    }

  protected:
    Layer();

  private:
    struct {
        bool visible : 1;
        bool shouldRasterize : 1;
        bool allowsEdgeAntialiasing : 1;
        bool allowsGroupOpacity : 1;
        bool excludeChildEffectsInLayerStyle : 1;
        uint8_t blendMode : 5;
        uint8_t maskType : 2;
    } bitFields = {};
    std::string _name;
    Layer *_parent = nullptr;
    std::vector<std::shared_ptr<Layer>> _children = {};
};
};  // namespace kk::design

#endif /* Layer_hpp */
