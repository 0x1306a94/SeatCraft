#ifndef DOCKHANDLER_H
#define DOCKHANDLER_H

#include <QGuiApplication>
#include <QObject>

namespace kk::event {
class DockHandler : public QObject {
    Q_OBJECT
  public:
    explicit DockHandler(QObject *parent = nullptr);

  Q_SIGNALS:
    void dockIconClicked();

  private Q_SLOTS:
    void handleStateChange(Qt::ApplicationState state);
};
};  // namespace kk

#endif  // DOCKHANDLER_H
