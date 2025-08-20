//
//  JSONDeserialize.cpp
//  SeatCraftDesignModel
//
//  Created by king on 2025/8/20.
//

#include <SeatCraftDesignModel/deserialize/JSONDeserialize.hpp>

#include <tgfx/layers/Layer.h>

namespace kk::design {

std::shared_ptr<tgfx::Layer> JSONDeserialize::deserializeImpl(const std::string &data) {
    (void)(data);
    return nullptr;
}

};  // namespace kk::design
