
#ifndef __AndroidRendererBackend_h__
#define __AndroidRendererBackend_h__

#include "../../../core/renderer/RendererBackend.hpp"

#include <android/native_window_jni.h>

namespace kk::renderer {
class AndroidRendererBackend : public RendererBackend {
  public:
    explicit AndroidRendererBackend(ANativeWindow *nativeWindow);
    virtual ~AndroidRendererBackend();

    virtual std::shared_ptr<tgfx::Window> getWindow() override;
    virtual int getWidth() override;
    virtual int getHeight() override;
    virtual float getDensity() override;

  private:
    std::shared_ptr<tgfx::Window> _window{nullptr};
    ANativeWindow *_nativeWindow{nullptr};
};

};  // namespace kk::renderer

#endif /* __AndroidRendererBackend_h__ */