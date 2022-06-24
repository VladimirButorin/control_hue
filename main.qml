import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    Rectangle {
        anchors.centerIn: parent
        color: "red"
        height: 250
        width: 250
        SequentialAnimation on rotation {
                loops: Animation.Infinite

                PropertyAnimation { duration: 6000
                                    from: 0
                                    to: 360 }
                PropertyAnimation { duration: 6000
                                    from: 360
                                    to: 0 }
            }
    }
}
