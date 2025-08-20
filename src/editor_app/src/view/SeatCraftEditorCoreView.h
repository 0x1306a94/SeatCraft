#ifndef SEATCRAFTEDITORCOREVIEW_H
#define SEATCRAFTEDITORCOREVIEW_H

#include <QQuickItem>

#include <memory>

namespace kk {
class SeatCraftEditorCoreApp;

namespace renderer {
class SeatCraftEditorCoreRenderer;
};
};  // namespace kk

namespace kk::view {
class SeatCraftEditorCoreView : public QQuickItem {
    Q_OBJECT
  public:
    explicit SeatCraftEditorCoreView(QQuickItem *parent = nullptr);
    ~SeatCraftEditorCoreView();
    Q_INVOKABLE bool updateSize();

    Q_INVOKABLE void invalidateContent();

    Q_INVOKABLE void handlePan(float deltaX, float deltaY);
    Q_INVOKABLE void handlePinch(float scale, float centerX, float centerY);

  protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  private:
    void calculateTransform(float scaleFactor);

  private Q_SLOTS:
    void onBackendWindowCreated();
    void onSceneGraphInvalidated();

  private:
    std::shared_ptr<kk::SeatCraftEditorCoreApp> _app{nullptr};
    std::shared_ptr<kk::renderer::SeatCraftEditorCoreRenderer> _renderer{nullptr};
    bool _connectSceneGraphInvalidated{false};
    float _minimumZoomScale{0.001f};
    float _maximumZoomScale{100.0f};
    float _mouseScaleRatio{300.0f};
    float _mouseScrollRatio{0.8f};
    QPointF _translate{0.0f, 0.0f};
    QPointF _mousePosition{0.0f, 0.0f};
};
}  // namespace kk::view

#endif  // SEATCRAFTEDITORCOREVIEW_H
