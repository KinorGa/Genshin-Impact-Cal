import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: bufferRec
    implicitWidth: parent.implicitWidth
    Layout.topMargin: 20
    // implicitHeight: basicLayout.implicitHeight + 40
    implicitHeight: 300
    color: "#1a1a1a"
    radius: 6
    border.color: "#21be2b"
    border.width: 1
    required property var dynmodel

    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        clip: true

        ColumnLayout {
            id: basicLayout
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 20
            spacing: 15

            Repeater {
                model: bufferRec.dynmodel

                delegate: DynamicBox {
                    required property string name
                    required property int key
                    tagName: name
                    tagKey: key
                    Layout.fillWidth: true
                }
            }
        }
    }
}
