
#ifndef __AndroidRendererBackend_h__
#define __AndroidRendererBackend_h__

#include "core/renderer/RendererBackend.hpp"

#include <android/native_window_jni.h>

namespace kk::renderer {
class AndroidRendererBackend : public RendererBackend {
  public:
    explicit AndroidRendererBackend(ANativeWindow *nativeWindow, float density);
    ~AndroidRendererBackend();

    std::shared_ptr<tgfx::Window> getWindow() override;
    int getWidth() override;
    int getHeight() override;
    float getDensity() override;

  private:
    std::shared_ptr<tgfx::Window> _window{nullptr};
    ANativeWindow *_nativeWindow{nullptr};
    float _density{1.0f};
};

};  // namespace kk::renderer

#endif /* __AndroidRendererBackend_h__ */