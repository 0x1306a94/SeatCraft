//
// Created by king on 2025/8/14.
//

#ifndef SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_PLATFORM_IOS_UISCROLLVIEWZOOMPANCONTROLLER_H
#define SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_PLATFORM_IOS_UISCROLLVIEWZOOMPANCONTROLLER_H

#include "core/ui/ElasticZoomPanController.hpp"

namespace kk::ui {
class UIScrollViewZoomPanController : public ElasticZoomPanController {
  public:
    UIScrollViewZoomPanController();
    virtual ~UIScrollViewZoomPanController();
    virtual void setZoomScale(float zoomScale) override;
    virtual void setContentOffset(const tgfx::Point &contentOffset) override;
};

}  // namespace kk::ui

#endif  // SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_PLATFORM_IOS_UISCROLLVIEWZOOMPANCONTROLLER_H
