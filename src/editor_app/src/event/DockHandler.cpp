#include "DockHandler.h"

namespace kk::event {
DockHandler::DockHandler(QObject *parent)
    : QObject{parent} {

   connect(qApp, &QGuiApplication::applicationStateChanged, this, &DockHandler::handleStateChange);
}

void DockHandler::handleStateChange(Qt::ApplicationState state) {
    if (state == Qt::ApplicationActive) {
        Q_EMIT dockIconClicked();
    }
}
};  // namespace kk
