//
//  SeatCraftViewCore.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/14.
//
#ifndef SeatCraftViewCore_hpp
#define SeatCraftViewCore_hpp

#include <memory>
#include <functional>

#include <tgfx/core/Point.h>
#include <tgfx/core/Size.h>

#include "core/SeatStatusKey.h"

namespace tgfx {
class Stream;
};  // namespace tgfx

namespace kk {
class SeatCraftCoreApp;
};

namespace kk::thread {
class WorkThread;
class UIThreadScheduler;
};  // namespace kk::thread

namespace kk::renderer {
class RendererBackend;
class SeatCraftCoreRenderer;
};  // namespace kk::renderer

namespace kk::ui {
class ElasticZoomPanController;

class SeatCraftViewCore : public std::enable_shared_from_this<SeatCraftViewCore> {
  public:
    explicit SeatCraftViewCore(std::shared_ptr<kk::SeatCraftCoreApp> app,
                               std::unique_ptr<kk::renderer::RendererBackend> backend,
                               std::unique_ptr<kk::ui::ElasticZoomPanController> zoomPanController,
                               std::shared_ptr<kk::thread::UIThreadScheduler> uiScheduler);
    ~SeatCraftViewCore();

    SeatCraftViewCore(const SeatCraftViewCore &) = delete;
    SeatCraftViewCore &operator=(const SeatCraftViewCore &) = delete;

    void replaceBackend(std::unique_ptr<kk::renderer::RendererBackend> backend);

    std::shared_ptr<kk::SeatCraftCoreApp> getApp() const;
    // 视图尺寸更新，由平台层调用
    bool updateSize();

    void setMaxWidth(float maxWidth);
    float getMaxWidth() const;

    float getSvgScale() const;

    void invalidateContent();

    // 绘制方法，由平台层渲染循环调用
    void draw(bool force = false);

    // 更新区域SVG数据
    void updateAreaAvailable();
    void updateSeatStatusAvailable();

    // 手势处理方法，由平台层调用
    void handlePan(const tgfx::Point &delta);
    void handlePinch(float scale, const tgfx::Point &center);

    float getMinimumZoomScale() const;
    float getMaximumZoomScale() const;
    float getZoomScale() const;
    void setZoomScale(float zoomScale);
    const tgfx::Point &getContentOffset() const;
    void setContentOffset(const tgfx::Point &contentOffset);

    void postWork(std::function<void()> task);
    void postUI(std::function<void()> task);
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
    std::shared_ptr<kk::thread::UIThreadScheduler> _uiScheduler{nullptr};
    std::unique_ptr<kk::thread::WorkThread> _workThread{nullptr};
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
