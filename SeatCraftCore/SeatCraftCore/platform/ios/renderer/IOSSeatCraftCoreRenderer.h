//
//  xxIOSSeatCraftCoreRenderer.h
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "../../../core/renderer/SeatCraftCoreRenderer.hpp"

#import <QuartzCore/CAEAGLLayer.h>

namespace kk::renderer {
class IOSSeatCraftCoreRenderer : public SeatCraftCoreRenderer {
  public:
    explicit IOSSeatCraftCoreRenderer(const kk::SeatCraftCoreApp *app, CAEAGLLayer *eagLayer);
    virtual ~IOSSeatCraftCoreRenderer();

  protected:
    void initWindow() override;

  private:
    CAEAGLLayer *eagLayer;
};

};  // namespace kk::renderer
