import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ToolBar{
  id: menuBar
  width: parent.width
  height: 50

  RowLayout{
    anchors.fill: parent

    ToolButton{
      // text: "📁"
      icon.source: "qrc:/Images/assets/icons/folder_open.png"
      width: 40
      onClicked:{
        // States.loadYaml("D:/QtProject/GiCal/test2_output.yaml")
        fileDialog.open()
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

  FileDialog {
    id: fileDialog
    currentFolder: {
      var parentDir = Qt.resolvedUrl("../")
      console.log(parentDir)
      return parentDir;
    }
    // onAccepted: image.source = selectedFile
    onAccepted: {
      console.log(selectedFile)
    }
  }
}