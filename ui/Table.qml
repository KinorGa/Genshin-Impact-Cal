import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: parent.width
    height: parent.height
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
                    model: ListModel {
                        ListElement {
                            value: "option1"
                        }
                        ListElement {
                            value: "option2"
                        }
                        ListElement {
                            value: "option3"
                        }
                        ListElement {
                            value: "option4"
                        }
                        ListElement {
                            value: "option5"
                        }
                        ListElement {
                            value: "option6"
                        }
                        ListElement {
                            value: "option7"
                        }
                        ListElement {
                            value: "option8"
                        }
                        ListElement {
                            value: "option9"
                        }
                        ListElement {
                            value: "option1"
                        }
                        ListElement {
                            value: "option2"
                        }
                        ListElement {
                            value: "option3"
                        }
                        ListElement {
                            value: "option4"
                        }
                        ListElement {
                            value: "option5"
                        }
                        ListElement {
                            value: "option6"
                        }
                        ListElement {
                            value: "option7"
                        }
                        ListElement {
                            value: "option8"
                        }
                        ListElement {
                            value: "option9"
                        }
                        ListElement {
                            value: "option1"
                        }
                        ListElement {
                            value: "option2"
                        }
                        ListElement {
                            value: "option3"
                        }
                        ListElement {
                            value: "option4"
                        }
                        ListElement {
                            value: "option5"
                        }
                        ListElement {
                            value: "option6"
                        }
                        ListElement {
                            value: "option7"
                        }
                        ListElement {
                            value: "option8"
                        }
                        ListElement {
                            value: "option9"
                        }
                        ListElement {
                            value: "option1"
                        }
                        ListElement {
                            value: "option2"
                        }
                        ListElement {
                            value: "option3"
                        }
                        ListElement {
                            value: "option4"
                        }
                        ListElement {
                            value: "option5"
                        }
                        ListElement {
                            value: "option6"
                        }
                        ListElement {
                            value: "option7"
                        }
                        ListElement {
                            value: "option8"
                        }
                        ListElement {
                            value: "option9"
                        }
                        ListElement {
                            value: "option1"
                        }
                        ListElement {
                            value: "option2"
                        }
                        ListElement {
                            value: "option3"
                        }
                        ListElement {
                            value: "option4"
                        }
                        ListElement {
                            value: "option5"
                        }
                        ListElement {
                            value: "option6"
                        }
                        ListElement {
                            value: "option7"
                        }
                        ListElement {
                            value: "option8"
                        }
                        ListElement {
                            value: "option9"
                        }
                        ListElement {
                            value: "option1"
                        }
                        ListElement {
                            value: "option2"
                        }
                        ListElement {
                            value: "option3"
                        }
                        ListElement {
                            value: "option4"
                        }
                        ListElement {
                            value: "option5"
                        }
                        ListElement {
                            value: "option6"
                        }
                        ListElement {
                            value: "option7"
                        }
                        ListElement {
                            value: "option8"
                        }
                        ListElement {
                            value: "option9"
                        }
                    }
                    delegate: CheckBox {
                        required property string value
                        text: value
                        checked: true
                        width: 100
                        onCheckedChanged: {
                            // giTableModel.updateFilterSelection(modelData, checked)
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
                text: "clear filter"
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
        }
    }

    Rectangle {
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        // width: 500
        // height: 600
        anchors.margins: 10
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
                color: mouseArea.containsMouse ? "#444444" : "#333333"
                border.color: "#555555"
                border.width: 1

                Text {
                    id: headerText
                    anchors.centerIn: parent
                    text: giTableModel.headerData(index, Qt.Horizontal, 0)
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        console.log("Header clicked: column", index);
                        // giTableModel.sortByColumn(index);
                    }
                }
            }
        }

        // // ✅ Vertical header
        VerticalHeaderView {
            id: verticalHeader
            anchors.left: parent.left
            anchors.right: horizontalHeader.left
            anchors.top: horizontalHeader.bottom
            syncView: tableView
            clip: true
            // implicitWidth: 60
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

                delegate: Rectangle {
                    required property int row
                    required property int column
                    required property var display

                    implicitWidth: 100
                    implicitHeight: 35
                    border.width: 1
                    border.color: "#ddd"
                    color: row % 2 === 0 ? "#f9f9f9" : "white"

                    Text {
                        id: st
                        anchors.centerIn: parent
                        text: display
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
