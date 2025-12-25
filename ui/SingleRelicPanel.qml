import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: relic
  implicitHeight: mainLayout.implicitHeight+20
  implicitWidth: parent.width/5
  required property int idx // Relic index

  Rectangle{
    anchors.fill: parent
    color: "#2a2a2a"
    radius: 8
    border.color: "#555555"
    border.width: 1
  
  ColumnLayout {
    id: mainLayout
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.margins: 10
    spacing: 0

    // Main stat section
    RowLayout {
      Layout.fillWidth: true
      spacing: 10

      Text {
        text: "主属性"
        font.pixelSize: 13
        font.bold: true
        color: "#21be2b"
        font.family: "Microsoft YaHei"
        Layout.preferredWidth: 60
      }

      RelicComboBox {
        id: r1
        idx: 5*relic.idx
        Layout.fillWidth: true
        Layout.preferredHeight: 36
      }
    }

    // Separator
    Rectangle {
      Layout.fillWidth: true
      height: 1
      color: "#3a3a3a"
      Layout.topMargin: 5
      Layout.bottomMargin: 5
    }

    // Sub stats section
    ColumnLayout {
      Layout.fillWidth: true
      spacing: 8

      Text {
        text: "副属性"
        font.pixelSize: 13
        font.bold: true
        color: "#999999"
        font.family: "Microsoft YaHei"
      }

      // Sub-stat 1
      RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Rectangle {
          Layout.preferredWidth: 24
          Layout.preferredHeight: 24
          color: "#3a3a3a"
          radius: 12
          border.color: "#555555"
          border.width: 1

          Text {
            anchors.centerIn: parent
            text: "1"
            font.pixelSize: 11
            font.bold: true
            color: "#999999"
          }
        }

        RelicComboBox {
          id: r2
          idx: 5*relic.idx+1
          Layout.fillWidth: true
          Layout.preferredHeight: 32
        }
      }

      // Sub-stat 2
      RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Rectangle {
          Layout.preferredWidth: 24
          Layout.preferredHeight: 24
          color: "#3a3a3a"
          radius: 12
          border.color: "#555555"
          border.width: 1

          Text {
            anchors.centerIn: parent
            text: "2"
            font.pixelSize: 11
            font.bold: true
            color: "#999999"
          }
        }

        RelicComboBox {
          id: r3
          idx: 5*relic.idx+2
          Layout.fillWidth: true
          Layout.preferredHeight: 32
        }
      }

      // Sub-stat 3
      RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Rectangle {
          Layout.preferredWidth: 24
          Layout.preferredHeight: 24
          color: "#3a3a3a"
          radius: 12
          border.color: "#555555"
          border.width: 1

          Text {
            anchors.centerIn: parent
            text: "3"
            font.pixelSize: 11
            font.bold: true
            color: "#999999"
          }
        }

        RelicComboBox {
          id: r4
          idx: 5*relic.idx+3
          Layout.fillWidth: true
          Layout.preferredHeight: 32
        }
      }

      // Sub-stat 4
      RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Rectangle {
          Layout.preferredWidth: 24
          Layout.preferredHeight: 24
          color: "#3a3a3a"
          radius: 12
          border.color: "#555555"
          border.width: 1

          Text {
            anchors.centerIn: parent
            text: "4"
            font.pixelSize: 11
            font.bold: true
            color: "#999999"
          }
        }

        RelicComboBox {
          id: r5
          idx: 5*relic.idx+4
          Layout.fillWidth: true
          Layout.preferredHeight: 32
        }
      }
    }

    Component.onCompleted: {
      // relic.setState()
    }
  }
  }
  function setState()
  {
    r1.setIndex(States.getState(5*relic.idx+0))
    r2.setIndex(States.getState(5*relic.idx+1))
    r3.setIndex(States.getState(5*relic.idx+2))
    r4.setIndex(States.getState(5*relic.idx+3))
    r5.setIndex(States.getState(5*relic.idx+4))
  }
}