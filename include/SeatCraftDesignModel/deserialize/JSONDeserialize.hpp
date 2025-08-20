//
//  JSONDeserialize.hpp
//  SeatCraftDesignModel
//
//  Created by king on 2025/8/20.
//

#ifndef JSONDeserialize_hpp
#define JSONDeserialize_hpp

#include <SeatCraftDesignModel/deserialize/Deserializeable.h>

namespace kk::design {

class JSONDeserialize : public Deserializeable<JSONDeserialize, std::string> {
  public:
  private:
    std::shared_ptr<tgfx::Layer> deserializeImpl(const std::string &data);
};

};  // namespace kk::design

#endif /* JSONDeserialize_hpp */
