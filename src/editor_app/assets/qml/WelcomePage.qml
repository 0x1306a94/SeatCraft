import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: welcomePage
    anchors.fill: parent
    color: '#191919'
    // radius: 16
    // clip: true
    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: welcomePage.width
            height: welcomePage.height
            radius: 16
        }
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
                // Qt.quit()
                appWindow.hide()
            }
        }


        Rectangle {
            id: recentProjectsContaier
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: 300
            color: '#2a2a2a'

            ColumnLayout {
                id: recentProjectsLayout
                anchors.fill: parent

            }
        }


        ColumnLayout {
            id: menuLayout
            anchors.right: recentProjectsContaier.left
            anchors.rightMargin: 40
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            spacing: 8

            Rectangle {
                id: createProject
                Layout.alignment: Qt.AlignLeading
                Layout.fillWidth: true
                height: 40
                radius: 10
                color: '#282828'


                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    text: 'Create New Project...'
                    color: 'white'
                    font.pointSize: 16
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var component = Qt.createComponent("EditorPage.qml")
                        if (component.status === Component.Ready) {
                            var newWindow = component.createObject(appWindow)
                            newWindow.show()
                            appWindow.hide()
                        }
                    }
                }
            }

            Rectangle {
                id: openProject
                Layout.alignment: Qt.AlignLeading
                Layout.fillWidth: true
                height: 40
                radius: 10
                color: '#282828'


                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    text: 'Open Existing Project...'
                    color: 'white'
                    font.pointSize: 16
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
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

    }
}
