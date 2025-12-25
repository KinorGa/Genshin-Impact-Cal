import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar{
  id: menuBar
  width: parent.width
  height: 40

  RowLayout{
    anchors.fill: parent

    ToolButton{
      text: "📁"
      width: 40
      onClicked:{
        States.loadYaml("D:/QtProject/GiCal/test2_output.yaml")
      }
    }

    ToolSeparator{}

    ToolButton{
      text: "💾"
      width: 40
      onClicked:{
        States.saveYaml("D:/QtProject/GiCal/test2_output.yaml")
      }
    }

    ToolSeparator{}

    ToolButton{
      text: "📤"
      width: 200
      onClicked:{
        States.loadBufferConfig()
        States.loadRelicConfig()
      }
    }

    ToolSeparator{}

    Item{Layout.fillWidth: true}
  }
}