//
// Created by king on 2025/8/14.
//

#include "UIScrollViewZoomPanController.h"

#include <tgfx/platform/Print.h>

namespace kk::ui {

UIScrollViewZoomPanController::UIScrollViewZoomPanController() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

UIScrollViewZoomPanController::~UIScrollViewZoomPanController() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

void UIScrollViewZoomPanController::setZoomScale(float zoomScale) {
    _zoomScale = zoomScale;
}

void UIScrollViewZoomPanController::setContentOffset(const tgfx::Point &contentOffset) {
    _contentOffset = contentOffset;
}
}  // namespace kk::ui
