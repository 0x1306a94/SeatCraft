//
// Created by king on 2025/8/13.
//

#ifndef SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_CORE_EDGEINSETS_H
#define SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_CORE_EDGEINSETS_H

namespace kk {

struct EdgeInsets {
    float top;
    float left;
    float bottom;
    float right;
    EdgeInsets()
        : EdgeInsets(0.0f, 0.0f, 0.0f, 0.0f) {}
    EdgeInsets(float top, float left, float bottom, float right)
        : top(top)
        , left(left)
        , bottom(bottom)
        , right(right) {}
};

}  // namespace kk

#endif  // SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_CORE_EDGEINSETS_H
