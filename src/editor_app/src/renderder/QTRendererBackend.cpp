#include "QTRendererBackend.h"

#include <tgfx/gpu/opengl/qt/QGLWindow.h>
#include <tgfx/platform/Print.h>
namespace kk::renderer {
QTRendererBackend::QTRendererBackend(QQuickItem *quickItem)
    : _quickItem(quickItem) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

QTRendererBackend::~QTRendererBackend() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

std::shared_ptr<tgfx::Window> QTRendererBackend::getWindow() {
    if (_quickItem == nullptr) {
        return nullptr;
    }

    if (_window == nullptr) {
        _window = tgfx::QGLWindow::MakeFrom(_quickItem, true);
    }

    return _window;
}

int QTRendererBackend::getWidth() {
    if (_quickItem == nullptr) {
        return 0;
    }

    auto window = _quickItem->window();
    if (window == nullptr) {
        return 0;
    }

    auto density = getDensity();
    auto width = static_cast<int>(ceil(static_cast<float>(_quickItem->width() * density)));
    return width;
}

int QTRendererBackend::getHeight() {
    if (_quickItem == nullptr) {
        return 0;
    }

    auto window = _quickItem->window();
    if (window == nullptr) {
        return 0;
    }

    auto density = getDensity();
    auto height = static_cast<int>(ceil(static_cast<float>(_quickItem->height() * density)));
    return height;
}

float QTRendererBackend::getDensity() {

    if (_quickItem == nullptr) {
        return 1.0f;
    }

    auto window = _quickItem->window();
    if (window == nullptr) {
        return 1.0f;
    }

    auto pixelRatio = window->devicePixelRatio();
    return static_cast<float>(pixelRatio);
}

};  // namespace kk::renderer
