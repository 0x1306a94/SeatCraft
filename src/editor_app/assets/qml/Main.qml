import QtQuick
import QtQuick.Window
import QtQuick.Controls
import com.seatcraft.editor

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    maximumWidth: 800
    maximumHeight: 600
    visible: true
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"

    DockHandler {
        id: dockHandler
        onDockIconClicked: {
            if (!appWindow.visible) {
                appWindow.show()
            }
        }
    }

    // 页面加载
    Loader {
        id: pageLoader
        anchors.fill: parent
        source: "WelcomePage.qml"
    }
}
