pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: dynItem
  implicitWidth: parent.implicitWidth
  implicitHeight: bufferLayout.implicitHeight + 20
  required property string tagName
  required property int tagKey
  property var total: 0.

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
        text: "Total: " + dynItem.total.toFixed(2)
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
          model: ListModel{
            id: bufferModel
          }

          delegate: BufferBox{
            required property string tag
            required property string value
            required property int index
            
            tagText: tag
            valueText: value

            onTagChanged: {
              bufferModel.set(index, { "tag": tagText }) // fix only add tag or value issue
            }

            onValueChanged: {
              bufferModel.set(index, { "value": valueText })
              dynItem.updateTotal()
            }

            onRemoveClicked: {
              dynItem.removeBufferBox(index);
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
      visible: bufferModel.count > 0
    }
  }

  function addBufferBox(args)
  {
    bufferModel.append({"tag": args[0], "value": args[1]});
  }

  function removeBufferBox(index)
  {
    bufferModel.remove(index);
    updateTotal()
  }

  function updateTotal()
  {
    var sum = 0.0;
    for(var i = 0; i < bufferModel.count; i++)
    {
      var val = parseFloat(bufferModel.get(i).value);
      if(!isNaN(val))
      {
        sum += val;
      }
    }
    total = sum;
  }

  function addBufferByConfig()
  {
    bufferModel.clear()
    let sz=States.getBufferSize(tagKey);
    for(let i=0;i<sz;i++)
    {
      let buf=States.getBufferData(tagKey,i);
      addBufferBox([buf.tag, buf.value.toString()]);
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
    States.clearBufferData(tagKey);
    for(var i=0; i<bufferModel.count; i++)
    {
      let buf=bufferModel.get(i);
      States.updateBufferData(tagKey, buf.tag, parseFloat(buf.value));
    }
  }

  Connections {
    target: States
    function onUpdateBuffer(){
      dynItem.updateStates()
    }
  }
}