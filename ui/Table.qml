import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: parent.width
    height: parent.height
    property alias prevPageBtn: prevPageBtn
    Popup {
        id: filterPopup
        width: 600
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        ScrollView {
            width: filterPopup.width
            height: filterPopup.height
            clip: true

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            Flow {
                width: filterPopup.width
                anchors.margins: 10
                spacing: 10

                Repeater {
                    model: columnModel
                    delegate: CheckBox {
                        required property string name
                        required property bool show
                        required property var model
                        text: name
                        checked: show // TOTO
                        width: 150
                        onClicked: {
                            console.log("check")
                            // giTableModel.updateFilterSelection(modelData, checked)
                            model.show=checked
                        }
                    }
                }
            }
        }
    }

    ToolBar {
        id: toolBar
        height: 30

        RowLayout {
            anchors.fill: parent
            spacing: 0

            ToolButton {
                text: "filter1"
                height: parent.height
                onClicked: {
                    giTableModel.filter1();
                }
            }

            ToolSeparator {}

            ToolButton {
                text: "filter2"
                height: parent.height
                onClicked: {
                    giTableModel.filter2();
                }
            }

            ToolSeparator {}

            ToolButton {
                text: "reset"
                height: parent.height
                onClicked: {
                    giTableModel.resetData();
                }
            }

            ToolSeparator {}

            ToolButton {
                text: "filter"
                height: parent.height
                onClicked: {
                    filterPopup.open();
                }
            }

            ToolSeparator{}

            ToolButton{
              text: "recal"
              height: parent.height
              onClicked:{
                States.test_generate()
              }
            }

            ToolSeparator{}

            ToolButton{
              text: "<"
              id: prevPageBtn
            }

            ToolSeparator{}

            ToolButton{
              id: nextPageBtn
              text: ">"
            }
        }
    }

    Rectangle {
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        // width: 500
        // height: 600
        anchors.margins: 0
        color: "white"
        border.color: "#ddd"
        border.width: 1

        // ✅ Horizontal header with column visibility
        HorizontalHeaderView {
            id: horizontalHeader
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 60
            anchors.right: parent.right
            height: 40
            syncView: tableView
            clip: true
            // resizableColumns: true
            movableColumns: true

            delegate: Rectangle {
                required property int index
                required property int modelData

                // ✅ Return 0 width for hidden columns
                // implicitWidth: columnModel.get(index).visible ? 100 : 0
                implicitHeight: 40
                implicitWidth: 100
                // color: mouseArea.containsMouse ? "#444444" : "#333333"
                color: "#333333"
                border.color: "#555555"
                border.width: 1

                Text {
                    id: headerText
                    anchors.centerIn: parent
                    text: giTableModel.headerData(parent.index, Qt.Horizontal, 0)
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                }

                TapHandler {
                    onTapped: {
                        console.log("Header tapped: column", parent.index);
                        // giTableModel.sortByColumn(index);
                        giTableModel.sortByColumn(parent.index);
                    }
                }

                TapHandler {
                    acceptedButtons: Qt.RightButton
                    onTapped: {
                        console.log("Header right-tapped: column", parent.index);
                    }
                }
            }
        }

        // // ✅ Vertical header
        VerticalHeaderView {
            id: verticalHeader
            anchors.left: parent.left
            // anchors.right: horizontalHeader.left
            anchors.top: horizontalHeader.bottom
            syncView: tableView
            clip: true
            width: 60
            // implicitWidth: 60

            delegate: Rectangle{
              required property int index
              required property int modelData

              implicitWidth: 60
              implicitHeight: 35
              border.width: 1
              border.color: "#ddd"
              color: "#333333"

              Text {
                  anchors.centerIn: parent
                  text: giTableModel.headerData(parent.index, Qt.Vertical, 0)
                  color: "white"
                  font.bold: true
                  font.pixelSize: 12
              }
            }
        }

        // ✅ Data table with columnWidthProvider
        // ✅ ScrollView wraps the TableView
        ScrollView {
            anchors.left: verticalHeader.right
            anchors.top: horizontalHeader.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true

            // ✅ Add scrollbars
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
            ScrollBar.vertical.width: 20

            TableView {
                id: tableView
                resizableColumns: true
                model: giTableModel
                clip: true
                columnSpacing: 1
                rowSpacing: 1
                interactive: true

                // ========== 新增1：打印模型列数/行数，定位QML侧认知的列数 ==========
                Component.onCompleted: {
                    console.log("[QML] 模型列数:", giTableModel.columnCount())
                    console.log("[QML] 模型行数:", giTableModel.rowCount())
                    console.log("[QML] TableView列数:", tableView.columns)
                }

                // ========== 新增2：监控列数变化，定位异常列数来源 ==========
                onColumnsChanged: {
                    console.warn("[QML] TableView列数变化为:", tableView.columns)
                }


                delegate: Rectangle {
                    required property int row
                    required property int column
                    required property var display

                    implicitWidth: 120
                    implicitHeight: 35
                    border.width: 1
                    border.color: "#ddd"
                    color: row % 2 === 0 ? "#f9f9f9" : "white"

                    Text {
                        id: st
                        anchors.centerIn: parent
                        text: {
                          if(parent.display === undefined || parent.display === null){
                            console.log("undefined data at row "+parent.row+" column "+parent.column)
                          }return parent.display
                        }
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
