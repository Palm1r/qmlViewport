import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import ViewportItem 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qml Viewport")

    Rectangle {
        anchors.fill: parent
        color: "gray"
    }

    SplitView {
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15

        Viewport {
            implicitWidth: parent.width / 2
            height: parent.height
        }

        Viewport {
            implicitWidth: parent.width / 2
            height: parent.height
        }
    }
}
