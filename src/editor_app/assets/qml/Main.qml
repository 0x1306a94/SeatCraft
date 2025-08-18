import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
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

    // 页面加载
    Loader {
        id: pageLoader
        anchors.fill: parent
        source: "WelcomePage.qml"
    }
}
