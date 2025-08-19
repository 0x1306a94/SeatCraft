import QtQuick
import QtQuick.Controls
import QtQuick.Window
import com.seatcraft.editor

Window {
    id: createProjectWindow
    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    visible: false

    onClosing: {
        Qt.callLater(function() { destroy(); })
    }
}
