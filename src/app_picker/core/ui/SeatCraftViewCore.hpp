//
//  SeatCraftViewCore.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/14.
//
#ifndef SeatCraftViewCore_hpp
#define SeatCraftViewCore_hpp

#include <memory>

#include <tgfx/core/Point.h>
#include <tgfx/core/Size.h>

#include "core/SeatStatusKey.h"

namespace tgfx {
class Stream;
};  // namespace tgfx

namespace kk {
class SeatCraftCoreApp;
};

namespace kk::renderer {
class RendererBackend;
class SeatCraftCoreRenderer;
};  // namespace kk::renderer

namespace kk::ui {
class ElasticZoomPanController;

class SeatCraftViewCore {
  public:
    explicit SeatCraftViewCore(std::shared_ptr<kk::SeatCraftCoreApp> app,
                               std::unique_ptr<kk::renderer::RendererBackend> backend,
                               std::unique_ptr<kk::ui::ElasticZoomPanController> zoomPanController);
    ~SeatCraftViewCore();

    std::shared_ptr<kk::SeatCraftCoreApp> getApp() const;
    // 视图尺寸更新，由平台层调用
    void updateSize();

    void setMaxWidth(float maxWidth);
    float getMaxWidth() const;

    float getSvgScale() const;

    void invalidateContent();

    // 绘制方法，由平台层渲染循环调用
    void draw(bool force = false);

    // 更新区域SVG数据
    void updateAreaSvgData(std::unique_ptr<tgfx::Stream> data);
    // 更新区域SVG路径
    void updateAreaSvgPath(const std::string &path);

    // 手势处理方法，由平台层调用
    void handlePan(const tgfx::Point &delta);
    void handlePinch(float scale, const tgfx::Point &center);
    float getMinimumZoomScale() const;
    float getMaximumZoomScale() const;
    float getZoomScale() const;
    void setZoomScale(float zoomScale);
    const tgfx::Point &getContentOffset() const;
    void setContentOffset(const tgfx::Point &contentOffset);

    // 更新座位状态SVG路径映射
    void updateSeatStatusSVGPathMap(kk::SeatStatusSVGPathMap map);

  private:
    // 内部方法，更新SVG缩放比例
    void updateSvgScale();
    // 内部方法，更新内容尺寸
    void updateContentSize();
    // 内部方法，更新最大最小缩放比例
    void updateMaxMinZoomScalesForCurrentBounds();
    // 内部方法，更新ZoomPanController状态并通知App
    void updateZoomPanControllerState();

  private:
    std::shared_ptr<kk::SeatCraftCoreApp> _app{nullptr};
    std::unique_ptr<kk::renderer::SeatCraftCoreRenderer> _renderer{nullptr};
    std::unique_ptr<kk::ui::ElasticZoomPanController> _zoomPanController{nullptr};
    float _maxWidth{1000.f};
    float _svgScale{1.0f};
    float _svgModelScale{1.0f};
    float _zoomScale9{1.0f};
    float _zoomScale18{1.0f};
    float _zoomScale30{1.0f};
    float _zoomScale50{1.0f};
};

}  // namespace kk::ui

#endif /* SeatCraftViewCore_hpp */
