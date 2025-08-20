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

  private Q_SLOTS:
    void onBackendWindowCreated();
    void onSceneGraphInvalidated();

  private:
    std::shared_ptr<kk::SeatCraftEditorCoreApp> _app{nullptr};
    std::shared_ptr<kk::renderer::SeatCraftEditorCoreRenderer> _renderer{nullptr};
    bool _connectSceneGraphInvalidated{false};
};
}  // namespace kk::view

#endif  // SEATCRAFTEDITORCOREVIEW_H
