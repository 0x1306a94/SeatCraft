import QtQuick
import QtQuick.Controls
import QtQuick.Window
import com.seatcraft.editor

ApplicationWindow {
    id: editorWindow
    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    visible: false

    // 项目路径
    property url projectFile

    StackView {
        id: editorPageRootStack
        anchors.fill: parent

        SeatCraftEditorCoreView {
            id: editorCoreView
            anchors.fill: parent
        }

        Rectangle {
            id: editorPageLeftExpand
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
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: "←"
                onClicked: {
                    editorPageLeftPanel.visible = !editorPageLeftPanel.visible
                }
            }
        }

        Rectangle {
            id: editorPageLeftPanel
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 200 + editorPageLeftExpand.x
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
                    editorPageLeftPanel.visible = false
                }
            }
        }

        Rectangle {
            id: editorPageBottomPanel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            width: 300
            height: 48
            color: '#2c2c2c'
            radius: 16
        }

        Rectangle {
            id: editorPageRightPanel
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 200
            visible: false
            color: '#2c2c2c'
        }
    }

    onClosing: {
        WindowUtils.closeEditorWindow(editorWindow.projectFile)
    }
}
