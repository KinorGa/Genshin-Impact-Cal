pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: dynItem
  implicitWidth: 400
  implicitHeight: bufferLayout.implicitHeight + 20
 
  ColumnLayout{
    id: bufferLayout
    anchors.fill: parent
    anchors.margins: 6
    spacing: 6

    RowLayout {
      Layout.fillWidth: true
      spacing: 0

      // Left: Buffers label
      Text {
        text: "Buffers"
        font.pixelSize: 13
        font.bold: true
        font.family: "Microsoft YaHei"
        color: "#ffffff"
        Layout.fillWidth: false
        Layout.preferredWidth: 70
      }

      // Separator
      Rectangle {
        Layout.preferredWidth: 1
        Layout.preferredHeight: 28
        color: "#404040"
        Layout.leftMargin: 12
        Layout.rightMargin: 12
      }

      // Button: Add
      Rectangle {
        id: addBtn
        Layout.preferredHeight: 36
        Layout.preferredWidth: 120
        color: "#1a1a1a"
        radius: 6
        border.color: "#21be2b"
        border.width: 1

        RowLayout {
          anchors.fill: parent
          anchors.margins: 8
          spacing: 8

          Text {
            text: "+"
            font.pixelSize: 18
            font.bold: true
            color: "#21be2b"
            Layout.fillWidth: false
          }

          Text {
            text: "Add"
            font.pixelSize: 12
            font.bold: true
            color: "#ffffff"
            Layout.fillWidth: true
          }
        }

        MouseArea {
          anchors.fill: parent
          cursorShape: Qt.PointingHandCursor
          hoverEnabled: true

          onClicked: {
            dynItem.addBufferBox(["", ""])
          }

          onEntered: {
            addBtn.color = "#2a2a2a"
            addBtn.border.color = "#ffffff"
          }

          onExited: {
            addBtn.color = "#1a1a1a"
            addBtn.border.color = "#21be2b"
          }
        }
      }

      Item { Layout.fillWidth: true } // Spacer

      // Separator
      Rectangle {
        Layout.preferredWidth: 1
        Layout.preferredHeight: 28
        color: "#404040"
        Layout.leftMargin: 12
        Layout.rightMargin: 12
      }

      // Right: Total count
      Text {
        text: "Total: " + repeater.count
        font.pixelSize: 12
        font.bold: true
        font.family: "Microsoft YaHei"
        color: "#21be2b"
        Layout.fillWidth: false
      }
    }
    
    Item{
      Layout.fillWidth: true
      Layout.preferredHeight: flow.implicitHeight
      // anchors.top: addButton.bottom
      // anchors.left: parent.left
      // anchors.right: parent.right
      // anchors.bottom: parent.bottom

      Flow {
        id: flow
        width: parent.width
        anchors.fill: parent // fix add new BufferBox in vertical direction issue
        spacing: 10

        Repeater{
          id: repeater
          model: ListModel{
            id: bufferModel
          }

          delegate: BufferBox{
            required property string tag
            required property string value
            required property int index
            
            tagText: tag
            valueText: value

            onRemoveClicked: {
              dynItem.removeBufferBox(index);
            }
          }
        }
      }
    }
  }

  function addBufferBox(args)
  {
    bufferModel.append({"tag": args[0], "value": args[1]});
  }

  function removeBufferBox(index)
  {
    bufferModel.remove(index);
  }
}