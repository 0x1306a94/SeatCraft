//
//  ElasticZoomPanController.hpp
//  SeatCraftCore
//
//  Created by KK on 2025/8/10.
//

#ifndef ElasticZoomPanController_hpp
#define ElasticZoomPanController_hpp

namespace kk::ui {
class ElasticZoomPanController {
  public:
    ElasticZoomPanController(float svgWidth, float svgHeight, float canvasWidth, float canvasHeight);

    // 更新 SVG（内容）尺寸
    void updateSVGSize(float newSvgWidth, float newSvgHeight);

    // 更新 Canvas（显示区域）尺寸
    void updateCanvasSize(float newCanvasWidth, float newCanvasHeight);

    // 设置缩放范围
    void setScaleRange(float minS, float maxS);

    // 设置缩放
    void setScale(float scale);

    // 设置弹性阻尼系数
    void setBounceFactor(float factor);

    // 手势开始
    void beginGesture();

    // 处理平移手势输入
    void onPan(float deltaX, float deltaY);

    // 处理缩放手势输入
    void onZoom(float scaleFactor, float anchorX, float anchorY);

    // 手势结束
    void endGesture();

    // 手动设置当前状态（动画插值时调用）
    void setState(float s, float ox, float oy);

    // 获取当前状态
    float getScale() const;
    float getOffsetX() const;
    float getOffsetY() const;

    // 获取合法状态（用于动画目标值）
    void getClampedState(float &outScale, float &outOffsetX, float &outOffsetY) const;

    // 重置到初始状态（fit center）
    void reset();

  private:
    float bounceDelta(float over) const;
    float applyBounceOffsetX(float delta);
    float applyBounceOffsetY(float delta);
    float clampOffsetX(float x, float s) const;
    float clampOffsetY(float y, float s) const;
    void clampScale();
    void clampOffset();

  private:
    float _svgW, _svgH;
    float _canvasW, _canvasH;

    float _minScale;
    float _maxScale;

    float _scale;
    float _offsetX;
    float _offsetY;

    float _bounceFactor;

    bool _gestureActive;
    float _gestureStartScale;
};
};  // namespace kk::ui

#endif /* ElasticZoomPanController_hpp */
