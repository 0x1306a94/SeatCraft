#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QUrl>

namespace kk::utils {
class WindowUtils : public QObject {
    Q_OBJECT
  public:
    explicit WindowUtils(QObject *parent = nullptr);

    static WindowUtils &Instance();

    void setQmlEngine(QQmlApplicationEngine *engine);

    Q_INVOKABLE QQuickWindow *openEditorWindow(const QUrl &projectFile);
    Q_INVOKABLE void closeEditorWindow(const QUrl &projectFile);
    Q_INVOKABLE int windowCount() const;

  private:
    QQmlApplicationEngine *_engine{nullptr};
    QList<QPointer<QQuickWindow>> _editorWindows;
};
};  // namespace kk::utils

#endif  // DOCKHANDLER_H
