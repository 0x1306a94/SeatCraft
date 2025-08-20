//
//  Deserializeable.h
//  SeatCraft
//
//  Created by king on 2025/8/20.
//

#ifndef Deserializeable_h
#define Deserializeable_h

#include <memory>
#include <type_traits>

namespace tgfx {
class Layer;
};

namespace kk::design {

template <typename Derived, typename Input>
class Deserializeable {
  public:
    std::shared_ptr<tgfx::Layer> deserialize(const Input &input) {
        return static_cast<Derived *>(this)->deserializeImpl(input);
    }
};

};  // namespace kk::design

#endif /* Deserializeable_h */
