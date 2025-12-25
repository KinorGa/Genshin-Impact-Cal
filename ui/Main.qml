import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Genshin Impact Calculator")
    color: "white"

    ColumnLayout{
      anchors.fill: parent
      spacing: 0
      
      MenuRec{
        Layout.fillWidth: true
        Layout.preferredHeight: 40
        z: 10
      }

      RowLayout{
        // anchors.fill: parent
        spacing: 0
        RelicPanel{
          Layout.fillHeight: true
          Layout.preferredWidth: 320
          z: 10
        }
        BufferPage{
          Layout.fillHeight: true
          Layout.fillWidth: true  
        }
      }
    }

}