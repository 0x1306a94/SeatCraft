import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: appWindow
    width: 600
    height: 800
    visible: true
    flags: Qt.Window | Qt.FramelessWindowHint
    title: qsTr("Hello World")

    Loader {
        id: pageLoader
        anchors.fill: parent
        source: 'WelcomePage.qml'
    }
    
}
