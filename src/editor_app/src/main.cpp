#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "event/DockHandler.h"
#include "view/SeatCraftEditorCoreView.h"

static constexpr char QML_PACKAGE_NAME[] = "com.seatcraft.editor";

int main(int argc, char *argv[]) {

    QApplication::setApplicationName("SeatCraftEditor");
    QApplication::setOrganizationName("com.seatcraft");

    QSurfaceFormat defaultFormat = QSurfaceFormat();
    defaultFormat.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
    defaultFormat.setVersion(3, 2);
    defaultFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(defaultFormat);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#else
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);

    qmlRegisterType<kk::view::SeatCraftEditorCoreView>(QML_PACKAGE_NAME, 1, 0, "SeatCraftEditorCoreView");
    qmlRegisterType<kk::event::DockHandler>(QML_PACKAGE_NAME, 1, 0, "DockHandler");

    QQmlApplicationEngine engine;
    engine.loadFromModule("main", "Main");
    auto window = static_cast<QQuickWindow *>(engine.rootObjects().at(0));
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    window->setPersistentGraphics(true);
#else
    window->setPersistentOpenGLContext(true);
#endif
    window->setPersistentSceneGraph(true);
    return app.exec();
}
