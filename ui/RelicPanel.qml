import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle{
  // implicitWidth: parent.width
  implicitHeight: parent.height
  color: "#1a1a1a"
  radius: 6
  border.color: "#21be2b"
  border.width: 1
  
  ScrollView{
    anchors.fill: parent
    anchors.margins: 10
    clip: true
  
  ColumnLayout{
    Layout.fillHeight: true
    Layout.preferredWidth: 600

    Repeater{
      model: 5

      delegate: SingleRelicPanel{
        required property int index
        Layout.preferredWidth: 300

        idx: index
      }
    }
  }
  }
  // RowLayout{
  //   anchors.top: parent.top
  //   anchors.left: parent.left
  //   anchors.right: parent.right
  //   spacing: 0

  //   Repeater{
  //     model: 5

  //     delegate: SingleRelicPanel{
  //       required property int index

  //       idx: index
  //     }
  //   }
  // }
}