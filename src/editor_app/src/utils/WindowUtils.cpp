#include "WindowUtils.h"

#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlContext>

namespace kk::utils {

WindowUtils::WindowUtils(QObject *parent)
    : QObject(parent) {
}

WindowUtils &WindowUtils::Instance() {
    static WindowUtils instance;
    return instance;
}

void WindowUtils::setQmlEngine(QQmlApplicationEngine *engine) {
    _engine = engine;
}

QQuickWindow *WindowUtils::openEditorWindow(const QUrl &projectFile) {
    if (_engine == nullptr) {
        return nullptr;
    }

    for (const auto &win : _editorWindows) {
        if (!win) {
            continue;
        }
        auto prop = win->property("projectFile");
        if (prop.isValid() && prop.toUrl() == projectFile) {
            return win;
        }
    }

    QQmlComponent component(_engine, QUrl("qrc:/qt/qml/main/assets/qml/EditorWindow.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qWarning() << "Failed to load EditorWindow.qml:" << component.errorString();
        return nullptr;
    }

    QQmlContext *context = new QQmlContext(_engine->rootContext());
    context->setContextProperty("projectFile", projectFile);

    QObject *obj = component.create(context);
    if (auto window = qobject_cast<QQuickWindow *>(obj)) {
        _editorWindows.append(window);
        return window;
    }
    return nullptr;
}

void WindowUtils::closeEditorWindow(const QUrl &projectFile) {
    for (const auto &win : _editorWindows) {
        if (!win) {
            continue;
        }
        auto prop = win->property("projectFile");
        if (prop.isValid() && prop.toUrl() == projectFile) {
            _editorWindows.removeAll(win);
            if (win->isVisible()) {
                win->close();
            }
            win->deleteLater();
            return;
        }
    }
}

int WindowUtils::windowCount() const {
    return static_cast<int>(QGuiApplication::topLevelWindows().size());
}

};  // namespace kk::utils
