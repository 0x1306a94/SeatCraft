import QtQuick
import QtQuick.Controls
import QtQuick.Window
import SeatCraftEditorCore

Window {
    id: editerWindow
    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720

    StackView {
        id: editerPageRootStack
        anchors.fill: parent

        SeatCraftEditorCoreView {
            id: editorCoreView
            anchors.fill: parent
        }

        Rectangle {
            id: editerPageLeftExpand
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            width: 200
            height: 48
            radius: 16
            visible: true
            color: '#2c2c2c'

            Button {
                id: expandButton
                anchors.centerIn: parent
                text: editerPageLeftPanel.visible ? "←" : "→"
                onClicked: {
                    editerPageLeftPanel.visible = !editerPageLeftPanel.visible
                    text = editerPageLeftPanel.visible ? "←" : "→"
                }
            }
        }

        Rectangle {
            id: editerPageLeftPanel
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 200 + editerPageLeftExpand.x
            visible: false
            color: '#2c2c2c'

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
            }

            Button {
                id: collapseButton
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 10
                anchors.rightMargin: 10
                text: "×"
                onClicked: {
                    editerPageLeftPanel.visible = false
                }
            }
        }

        Rectangle {
            id: editerPageBottomPanel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            width: 300
            height: 48
            color: '#2c2c2c'
            radius: 16
        }

        Rectangle {
            id: editerPageRightPanel
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 200
            visible: false
            color: '#2c2c2c'
        }
    }

    onClosing: {
        appWindow.show()
        Qt.callLater(function() { destroy(); })
    }
}
