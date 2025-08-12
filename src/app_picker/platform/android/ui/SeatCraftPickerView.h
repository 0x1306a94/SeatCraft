//
// Created by KK on 2025/8/12.
//

#ifndef SEATCRAFTAPPPICKEREXAMPLE_SEATCRAFTPICKERVIEW_H
#define SEATCRAFTAPPPICKEREXAMPLE_SEATCRAFTPICKERVIEW_H

#include <memory>

namespace tgfx {
class SVGDOM;
class Stream;
};  // namespace tgfx

namespace kk {
class SeatCraftCoreApp;
};

namespace kk::renderer {
class SeatCraftCoreRenderer;
};  // namespace kk::renderer
namespace kk::ui {
class ElasticZoomPanController;
class SeatCraftPickerView {
  public:
    explicit SeatCraftPickerView(std::shared_ptr<kk::SeatCraftCoreApp> app, std::shared_ptr<kk::renderer::SeatCraftCoreRenderer> renderer);
    ~SeatCraftPickerView();

    std::shared_ptr<kk::SeatCraftCoreApp> getApp();
    void updateSize();

    void draw(bool force = false);
    void updateAreaSvgData(std::unique_ptr<tgfx::Stream> data);

  private:
    void updateContentSize();
    void updateMaxMinZoomScalesForCurrentBounds();
    void updateZoomPanControllerState();

  private:
    std::shared_ptr<kk::SeatCraftCoreApp> _app{nullptr};
    std::shared_ptr<kk::renderer::SeatCraftCoreRenderer> _renderer{nullptr};
    std::shared_ptr<kk::ui::ElasticZoomPanController> _zoomPanController{nullptr};
    float _svgScale{1.0f};
    float _svgModelScale{1.0f};
    float _zoomScale9{1.0f};
    float _zoomScale18{1.0f};
    float _zoomScale30{1.0f};
    float _zoomScale50{1.0f};
};

}  // namespace kk::ui

#endif  // SEATCRAFTAPPPICKEREXAMPLE_SEATCRAFTPICKERVIEW_H
