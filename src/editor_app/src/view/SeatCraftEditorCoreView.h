#ifndef SEATCRAFTEDITORCOREVIEW_H
#define SEATCRAFTEDITORCOREVIEW_H

#include <QQuickItem>

#include <memory>

namespace kk::renderer {
class SeatCraftEditorCoreRenderer;
};

namespace kk::view {
class SeatCraftEditorCoreView : public QQuickItem {
    Q_OBJECT
  public:
    explicit SeatCraftEditorCoreView(QQuickItem *parent = nullptr);
    ~SeatCraftEditorCoreView();
    Q_INVOKABLE bool updateSize();

    Q_INVOKABLE void invalidateContent();

    Q_INVOKABLE void draw();

  protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

  private Q_SLOTS:
    void onBackendWindowCreated();
    void onSceneGraphInvalidated();

  private:
    std::shared_ptr<kk::renderer::SeatCraftEditorCoreRenderer> _renderer{nullptr};
    bool _connectSceneGraphInvalidated{false};
};
}  // namespace kk::view

#endif  // SEATCRAFTEDITORCOREVIEW_H
