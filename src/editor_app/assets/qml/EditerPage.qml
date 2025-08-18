import QtQuick
import QtQuick.Controls
import SeatCraftEditorCore

ApplicationWindow {
    id: editerWindow
    width: 1200
    height: 800



    Rectangle {
        id: editerPage
        anchors.fill: parent
        color: 'red'

        SeatCraftEditorCoreView {
            id: editorCoreView
            anchors.fill: parent
        }


    }


    onClosing: {
        appWindow.show()
    }
}
