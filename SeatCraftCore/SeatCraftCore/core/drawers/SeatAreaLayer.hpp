//
//  SeatAreaLayer.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#ifndef SeatAreaLayer_hpp
#define SeatAreaLayer_hpp

#include <tgfx/core/Size.h>
#include <tgfx/layers/Layer.h>

namespace tgfx {
class SVGDOM;
}

namespace kk {
class SeatCraftCoreApp;
};

namespace kk::drawers {
class SeatAreaLayer : public tgfx::Layer {
  public:
    static std::shared_ptr<SeatAreaLayer> Make(const kk::SeatCraftCoreApp *app);

    void setContentSize(const tgfx::Size &contentSize) {
        if (_contentSize == contentSize) {
            return;
        }
        _contentSize = contentSize;
        invalidateContent();
    }

    void setAreaSvgPath(const std::string &areaSvgPath) {
        if (_areaSvgPath == areaSvgPath) {
            return;
        }
        _areaSvgPath = areaSvgPath;
        invalidateContent();
    }

  protected:
    SeatAreaLayer(const kk::SeatCraftCoreApp *app);

    std::shared_ptr<tgfx::SVGDOM> loadSvgDom(const std::string &path);

    void onUpdateContent(tgfx::LayerRecorder *recorder) override;

  private:
    const kk::SeatCraftCoreApp *_app{nullptr};
    tgfx::Size _contentSize{};
    std::string _areaSvgPath{};
};

}  // namespace kk::drawers

#endif /* SeatAreaLayer_hpp */
