#ifndef QTRENDERERBACKEND_H
#define QTRENDERERBACKEND_H

#include <QQuickItem>

#include <SeatCraftEditorCore/renderer/RendererBackend.hpp>

namespace kk::renderer {
class QTRendererBackend : public kk::renderer::RendererBackend {
  public:
    explicit QTRendererBackend(QQuickItem *quickItem);

    virtual ~QTRendererBackend();

    virtual std::shared_ptr<tgfx::Window> getWindow() override;
    virtual int getWidth() override;
    virtual int getHeight() override;
    virtual float getDensity() override;

  private:
    QQuickItem *_quickItem{nullptr};
    std::shared_ptr<tgfx::Window> _window{nullptr};
};
}  // namespace kk::renderer

#endif  // QTRENDERERBACKEND_H
