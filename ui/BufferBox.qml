import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
  id: box
  height: 40
  width: 160

  color: valueField.activeFocus || tagField.activeFocus ? "#2a2a2a" : "#1a1a1a"
  border.color: valueField.activeFocus || tagField.activeFocus ? "#21be2b" : "#3a3a3a"
  border.width: 2
  radius: 6

  property bool hovered: boxMouseArea.containsMouse || deleteArea.containsMouse
  property string tagText: ""
  property string valueText: ""

  // singals
  signal removeClicked
  signal tagChanged(string newTag)
  signal valueChanged(string newValue)

  RowLayout{
    anchors.fill: parent
    anchors.margins: 3
    anchors.rightMargin: 8
    spacing: 0

    TextField{
      id: tagField
      Layout.preferredWidth: 75
      Layout.fillHeight: true
      text: box.tagText

      placeholderText: "Tag"
      placeholderTextColor: "#444"
      color: "white"
      font.pixelSize: 12
      font.weight: Font.DemiBold
      font.family: "Consolas"
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignVCenter
      selectByMouse: true
      background: Rectangle {
        color: "transparent"
      }
      onTextChanged: {
        console.debug("labeltext ", text)
      }
    }

    Text{
      text: "|"
      color: "#888"
      font.pixelSize: 14
      font.weight: Font.Bold
    }

    TextField{
      id: valueField
      Layout.fillWidth: true
      Layout.fillHeight: true
      anchors.margins: 3
      anchors.rightMargin: 6
      text: box.valueText
      
      placeholderText: "0"
      placeholderTextColor: "#444"
      color: "white"
      font.pixelSize: 12
      font.weight: Font.DemiBold
      font.family: "Consolas"

      horizontalAlignment: Text.AlignRight
      verticalAlignment: Text.AlignVCenter
      selectByMouse: true

      background: Rectangle {
        color: "transparent"
      }
      validator: RegularExpressionValidator{
        regularExpression: /^-?[0-9]*\.?[0-9]*$/
      }

      onTextChanged: {
        console.debug("labelvalue ", text)
      }
    }
  }

  // delete button
  Rectangle{
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: -8
    width: 20
    height: 20
    radius: 10
    color: deleteArea.containsMouse ? "#ff4d4d" : "#cc3333"
    border.color: "#ff0000"
    border.width: 1
    visible: box.hovered
    z: 10
    

    Behavior on color{
      ColorAnimation {duration: 150}
    }

    Text{
      anchors.centerIn: parent
      text: "×"
      font.pixelSize: 14
      font.bold: true
      color: "white"
    }

    MouseArea{
      id: deleteArea
      anchors.fill: parent
      hoverEnabled: true
      cursorShape: Qt.PointingHandCursor
      onClicked: {box.removeClicked()}
      z: -1
    }
  }

  MouseArea {
    id: boxMouseArea
    anchors.fill: parent
    hoverEnabled: true
    acceptedButtons: Qt.NoButton  
    z: -1 
  }

  // expose get api and set api
  function getTag(){
    return tagField.text;
  }

  function getValue(){
    return valueField.text;
  }

  function setTag(newTag){
    tagField.text = newTag;
  }

  function setValue(newValue){
    valueField.text = newValue;
  }
}