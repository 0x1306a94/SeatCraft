//
//  SVGSerialize.hpp
//  SeatCraft
//
//  Created by king on 2025/8/20.
//

#ifndef SVGSerialize_hpp
#define SVGSerialize_hpp

#include <SeatCraftDesignModel/serialize/Serializable.h>

namespace tgfx {
class GLDevice;
}

namespace kk::design {
class SVGSerialize : public Serializable<SVGSerialize, std::string> {
  public:
    static std::shared_ptr<SVGSerialize> Make(std::shared_ptr<tgfx::GLDevice> device);

    std::string serializeImpl(std::shared_ptr<tgfx::Layer> node);

  protected:
    SVGSerialize(std::shared_ptr<tgfx::GLDevice> device);

  private:
    std::shared_ptr<tgfx::GLDevice> _device{nullptr};
};

};  // namespace kk::design

#endif /* SVGSerialize_hpp */
