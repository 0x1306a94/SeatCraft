
#ifndef __WebRendererBackend_h__
#define __WebRendererBackend_h__

#include "core/renderer/RendererBackend.hpp"

#include <string>

namespace kk::renderer {
class WebRendererBackend : public RendererBackend {
  public:
    explicit WebRendererBackend(const std::string &canvasId, float density);
    ~WebRendererBackend();

    std::shared_ptr<tgfx::Window> getWindow() override;
    int getWidth() override;
    int getHeight() override;
    float getDensity() override;

  private:
    std::shared_ptr<tgfx::Window> _window{nullptr};
    std::string _canvasId{};
    float _density{1.0f};
};

};  // namespace kk::renderer

#endif /* __WebRendererBackend_h__ */