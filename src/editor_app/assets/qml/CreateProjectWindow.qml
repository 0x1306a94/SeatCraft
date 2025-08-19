import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Qt5Compat.GraphicalEffects
import com.seatcraft.editor

ApplicationWindow {
    id: createProjectWindow
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    maximumWidth: 800
    maximumHeight: 600
    visible: false
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"

    property bool dragging: false
    property point mouseLast: Qt.point(0, 0)

    onClosing: {
        Qt.callLater(function() { destroy(); })
    }

    Rectangle {
        id: root
        anchors.fill: parent
        color: '#191919'
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: 16
            }
        }

        MouseArea {
            id: rootMoveArea
            anchors.fill: parent
            onPressed: function(mouse) {
                createProjectWindow.mouseLast = Qt.point(mouse.x, mouse.y)
                createProjectWindow.dragging = true
            }

            onPositionChanged: function(mouse) {
                if (createProjectWindow.dragging) {
                    createProjectWindow.x += (mouse.x - createProjectWindow.mouseLast.x)
                    createProjectWindow.y += (mouse.y - createProjectWindow.mouseLast.y)
                }
            }

            onReleased: {
                createProjectWindow.dragging = false
            }
        }

        Button {
            id: closeButton
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 10
            text: "×"
            onClicked: {
                // Qt.quit()
                createProjectWindow.close()
                Qt.callLater(function() { destroy(); })
            }
        }
    }
}
