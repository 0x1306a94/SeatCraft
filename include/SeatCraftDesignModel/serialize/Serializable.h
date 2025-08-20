//
//  Serializable.h
//  SeatCraft
//
//  Created by king on 2025/8/20.
//

#ifndef Serializable_h
#define Serializable_h

#include <memory>
#include <type_traits>

namespace tgfx {
class Layer;
};

namespace kk::design {
template <typename Derived, typename Output>
class Serializable {
  public:
    Output serialize(std::shared_ptr<tgfx::Layer> layer) {
        return static_cast<Derived *>(this)->serializeImpl(layer);
    }
};

};  // namespace kk::design

#endif /* Serializable_h */
