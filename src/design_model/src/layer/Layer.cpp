//
//  Layer.cpp
//  SeatCraftDesignModel
//
//  Created by KK on 2025/8/19.
//

#include "Layer.hpp"

namespace kk::design {
std::shared_ptr<Layer> Layer::Make() {
    return std::shared_ptr<Layer>(new Layer());
}

Layer::Layer() {
    memset(&bitFields, 0, sizeof(bitFields));
    bitFields.visible = true;
}

Layer::~Layer() {
}

void Layer::setVisible(bool value) {
    if (bitFields.visible == value) {
        return;
    }
    bitFields.visible = value;
}

}  // namespace kk::design
