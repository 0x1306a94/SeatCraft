import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Rectangle {
    id: welcomePage
    width: 640
    height: 800
    radius: 16
    clip: true

    Rectangle {
        id: backgroundLayer
        anchors.fill: parent
        color: "#99000000"
    }

    FastBlur {
        anchors.fill: parent
        source: backgroundLayer
        radius: 20
        transparentBorder: true
    }

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
            id: closeButton
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 10
            text: "×"
            onClicked: {
                Qt.quit()
            }
        }

        Button {
            id: welcomePageOpenButton
            anchors.centerIn: parent
            text: "Open Project"
            onClicked: {
                console.log('Open Project')
                // pageLoader.source = "EditorPage.qml"
                var component = Qt.createComponent("EditorPage.qml")
                if (component.status === Component.Ready) {
                    var newWindow = component.createObject(appWindow)
                    newWindow.show()
                    appWindow.hide()
                }
            }
        }
    }
}
