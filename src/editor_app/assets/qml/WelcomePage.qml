import QtQuick
import QtQuick.Controls

Rectangle {
    id: welcomePage
    width: 640
    height: 800
    color: 'yellow'

    property bool dragging: false
    property point mouseLast: Qt.point(0, 0)

    MouseArea {
        id: welcomePageMoveArea
        anchors.fill: parent
        onPressed: function(mouse) {
            welcomePage.mouseLast = Qt.point(mouse.x, mouse.y)
            welcomePage.dragging = true
        }

        onPositionChanged: function(mouse) {
            if (welcomePage.dragging) {
                appWindow.x += (mouse.x - welcomePage.mouseLast.x)
                appWindow.y += (mouse.y - welcomePage.mouseLast.y)
            }
        }

        onReleased: {
            welcomePage.dragging = false
        }
    }

    StackView {
        anchors.fill: parent

        Button {
            id: welcomePageOpenButton
            anchors.centerIn: parent
            text: "Open Project"
            onClicked: {
                console.log('Open Project')
                // pageLoader.source = "EditerPage.qml"
                var component = Qt.createComponent("EditerPage.qml")
                if (component.status === Component.Ready) {
                    var newWindow = component.createObject(appWindow)
                    newWindow.show()
                    appWindow.hide()
                }
            }
        }
    }
}
