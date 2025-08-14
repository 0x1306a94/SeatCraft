//
//  ElasticZoomPanController.hpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/10.
//

#ifndef ElasticZoomPanController_hpp
#define ElasticZoomPanController_hpp

#include <algorithm>

#include <tgfx/core/Matrix.h>
#include <tgfx/core/Point.h>
#include <tgfx/core/Rect.h>
#include <tgfx/core/Size.h>

#include "core/EdgeInsets.h"

namespace kk::ui {

class ElasticZoomPanController {
  public:
    ElasticZoomPanController();
    virtual ~ElasticZoomPanController();
    // --- State Setters ---
    void setBounds(const tgfx::Size &bounds);

    const tgfx::Size &getBounds() const;

    void setContentSize(const tgfx::Size &contentSize);

    const tgfx::Size &getContentSize() const;

    void setContentInset(const EdgeInsets &contentInset);

    const EdgeInsets &getContentInset() const;

    void setMinimumZoomScale(float minimumZoomScale);

    float getMinimumZoomScale() const;

    void setMaximumZoomScale(float maximumZoomScale);

    float getMaximumZoomScale() const;

    virtual void setZoomScale(float zoomScale);

    float getZoomScale() const;

    virtual void setContentOffset(const tgfx::Point &contentOffset);

    const tgfx::Point &getContentOffset() const;

    // --- Transformation Matrix ---
    tgfx::Matrix getMatrix() const;

    // --- Gesture Handlers (Simulating User Input) ---
    virtual void handlePan(const tgfx::Point &delta);

    virtual void handlePinch(float scale, const tgfx::Point &center);

  protected:
    // --- Internal Logic ---
    virtual void revalidateContentOffset();

  protected:
    // --- Private Members ---
    float _minimumZoomScale{1.0f};
    float _maximumZoomScale{1.0f};
    float _zoomScale{1.0f};
    tgfx::Size _bounds{0.0f, 0.0f};
    tgfx::Size _contentSize{0.0f, 0.0f};
    tgfx::Point _contentOffset{0.0f, 0.0f};
    kk::EdgeInsets _contentInset{0.0f, 0.0f, 0.0f, 0.0f};
};

}  // namespace kk::ui

#endif /* ElasticZoomPanController_hpp */
