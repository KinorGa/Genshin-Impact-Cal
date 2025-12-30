pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GiCal.Models 1.0

Item {
  id: dynItem
  implicitWidth: parent.implicitWidth
  implicitHeight: bufferLayout.implicitHeight + 20
  required property string tagName
  required property int tagKey
  property var total: 0.

  BufferModel{
    id: bufferModel
    tagKey: dynItem.tagKey
  }

  // 绑定当前model实例到States (C++ to C++)
  Connections{
    target: bufferModel
    function onSenderBuffer(tagKey, data){
      States.receiveBuffer(tagKey, data);
    }
  }

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
        text: dynItem.tagName
        font.pixelSize: 13
        font.bold: true
        font.family: "Microsoft YaHei"
        color: "#ffffff"
        Layout.fillWidth: false
        Layout.preferredWidth: 100
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
        Layout.preferredWidth: 36
        color: "#1a1a1a"
        radius: 6
        border.color: "#21be2b"
        border.width: 1

        Text {
          anchors.centerIn: parent
          text: "+"
          font.pixelSize: 26
          font.bold: true
          color: "#21be2b"
          Layout.fillWidth: false
        }

        MouseArea {
          anchors.fill: parent
          cursorShape: Qt.PointingHandCursor
          hoverEnabled: true

          onClicked: {
            // dynItem.addBufferBox(["", ""])
            bufferModel.addBuffer("", 0.0)
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
        // text: "Total: " + dynItem.total.toFixed(2)
        text: "Total: "+bufferModel.total.toFixed(2)
        font.pixelSize: 12
        font.bold: true
        font.family: "Microsoft YaHei"
        color: "#21be2b"
        Layout.fillWidth: true
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
          model: bufferModel

          delegate: BufferBox{

            required property int index
            required property var modelData
            required property var model
            tagText: modelData.tag
            valueText: modelData.value

            onTagChanged: model.tag = tagText
            onValueChanged: model.value = valueText
            onRemoveClicked:{
              bufferModel.removeBuffer(index);
            }
          }
        }
      }
    }

    // ✨ Separator Line
    Rectangle {
      Layout.fillWidth: true
      Layout.preferredHeight: 1
      color: "#2d2d2d"
      visible: bufferModel.rowCount() > 0
    }
  }

  function addBufferByConfig()
  {
    bufferModel.clearBuffer();
    const itemsFromState = States.getBuffer(tagKey);

    for(let i = 0; i < itemsFromState.length; i++)
    {
      bufferModel.addBuffer(itemsFromState[i].tag, itemsFromState[i].value.toString());
    }
  }

  // Binding to States.loadBufferConfig (c++ class)
  Connections {
    target: States
    function onLoadBufferConfig(){
      dynItem.addBufferByConfig()
    }
    enabled: true
  }

  function updateStates()
  { 
    bufferModel.sender_buffer_()
  }

  Connections {
    target: States
    function onUpdateBuffer(){
      dynItem.updateStates()
    }
  }
}