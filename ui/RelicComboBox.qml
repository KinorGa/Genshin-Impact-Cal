pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: box
    required property int idx
    implicitWidth: 150
    implicitHeight: 36
    
    ComboBox {
        id: combb
        anchors.fill: parent
        
        textRole: "key"
        
        model: RelicStatModel{}
        
        currentIndex: 2
        
        // ✅ Custom styled button
        background: Rectangle {
            implicitWidth: 150
            implicitHeight: 36
            color: combb.pressed ? "#1a1a1a" : (combb.hovered ? "#2a2a2a" : "#252525")
            radius: 8
            border.color: combb.activeFocus ? "#21be2b" : "#404040"
            border.width: 2
            
            Behavior on color {
                ColorAnimation { duration: 150 }
            }
            
            Behavior on border.color {
                ColorAnimation { duration: 150 }
            }
            
            // Subtle inner glow
            Rectangle {
                anchors.fill: parent
                anchors.margins: 2
                radius: 6
                color: "transparent"
                border.color: combb.activeFocus ? Qt.rgba(0.13, 0.75, 0.17, 0.2) : "transparent"
                border.width: 1
            }
        }
        
        // ✅ Beautiful content with icon + text
        contentItem: Item {
            implicitWidth: contentRow.implicitWidth
            implicitHeight: contentRow.implicitHeight
            
            RowLayout {
                id: contentRow
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 32 // Space for dropdown arrow
                spacing: 10
                
                // Icon with background
                Rectangle {
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 28
                    color: Qt.rgba(0.13, 0.75, 0.17, 0.15)
                    radius: 6
                    border.color: "#21be2b"
                    border.width: 1
                    
                    Image {
                        anchors.centerIn: parent
                        source: combb.currentValue ? combb.currentValue.imgurl : ""
                        width: 20
                        height: 20
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                    }
                }
                
                // Text
                Text {
                    text: combb.currentValue ? combb.currentValue.key : ""
                    font.pixelSize: 10
                    font.family: "Microsoft YaHei"
                    font.bold: true
                    color: "#ffffff"
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
            }
            
            // Custom dropdown arrow
            Text {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: combb.popup.visible ? "▲" : "▼"
                font.pixelSize: 10
                color: "#21be2b"
                
                Behavior on rotation {
                    NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                }
            }
        }
        
        // ✅ Beautiful dropdown popup
        popup: Popup {
            y: combb.height + 4
            width: combb.width
            // height: contentItem.implicitHeight + 20
            implicitHeight: 220
            padding: 10
            modal: true
            
            // Popup background
            background: Rectangle {
                color: "#1e1e1e"
                radius: 10
                border.color: "#21be2b"
                border.width: 2
                
                // Shadow effect
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: -3
                    color: "transparent"
                    border.color: "#000000"
                    border.width: 3
                    radius: parent.radius
                    opacity: 0.3
                    z: -1
                }
                
                // Top accent line
                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 2
                    height: 3
                    radius: 2
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: "transparent" }
                        GradientStop { position: 0.5; color: "#21be2b" }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                }
            }
            
            contentItem: ListView {
                id: view
                clip: true
                // implicitHeight: {
                //   console.log("view contentHeight ", contentHeight)
                //   return contentHeight
                // }
                model: combb.popup.visible ? combb.delegateModel : null
                currentIndex: combb.highlightedIndex
                
                ScrollIndicator.vertical: ScrollIndicator {
                    contentItem: Rectangle {
                        implicitWidth: 6
                        radius: 3
                        color: "#21be2b"
                        opacity: 0.6
                    }
                }
            }
            
            // Enter animation
            enter: Transition {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
                NumberAnimation { property: "scale"; from: 0.9; to: 1.0; duration: 200; easing.type: Easing.OutBack }
            }
            
            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 150; easing.type: Easing.InCubic }
            }
        }
        
        // ✅ Beautiful dropdown items
        delegate: ItemDelegate {
            id: delegate
            width: combb.width - 20
            height: isDisabled ? 0 : 40
            visible: !isDisabled
            clip: true
            
            required property var model
            required property int index
            
            property bool isDisabled: (States.relicState(box.idx) & (1 << index)) === 0
            enabled: !isDisabled
            
            background: Rectangle {
                color: {
                    if (delegate.isDisabled) return "#1a1a1a"
                    if (delegate.pressed) return "#2a4d2c"
                    if (delegate.highlighted) return "#1e3a20"
                    if (delegate.hovered) return "#252525"
                    return "transparent"
                }
                radius: 6
                
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
                
                // Selection indicator
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: 3
                    color: "#21be2b"
                    radius: 1.5
                    visible: delegate.highlighted
                }
            }
            
            contentItem: RowLayout {
                spacing: 10
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                
                // Icon
                Rectangle {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    color: delegate.isDisabled ? "#2a2a2a" : Qt.rgba(0.13, 0.75, 0.17, 0.2)
                    radius: 6
                    border.color: delegate.isDisabled ? "#3a3a3a" : "#21be2b"
                    border.width: 1
                    
                    Image {
                        anchors.centerIn: parent
                        source: delegate.model.imgurl
                        width: 22
                        height: 22
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        opacity: delegate.isDisabled ? 0.3 : 1.0
                        
                        Behavior on opacity {
                            NumberAnimation { duration: 150 }
                        }
                    }
                    
                    // Disabled overlay
                    Rectangle {
                        anchors.fill: parent
                        radius: parent.radius
                        color: "#000000"
                        opacity: delegate.isDisabled ? 0.5 : 0
                        visible: delegate.isDisabled
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✗"
                            font.pixelSize: 16
                            font.bold: true
                            color: "#ff6b6b"
                        }
                    }
                }
                
                // Text
                Text {
                    text: delegate.model.key
                    font.pixelSize: 13
                    font.family: "Microsoft YaHei"
                    font.bold: delegate.highlighted
                    color: delegate.isDisabled ? "#666666" : (delegate.highlighted ? "#ffffff" : "#cccccc")
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                    
                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }
                
                // Checkmark for selected item
                Text {
                    text: "✓"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#21be2b"
                    visible: delegate.highlighted && !delegate.isDisabled
                    opacity: delegate.highlighted ? 1 : 0
                    
                    Behavior on opacity {
                        NumberAnimation { duration: 200 }
                    }
                }
            }
            
            highlighted: combb.highlightedIndex === index
        }
        
        onCurrentIndexChanged: {
            // console.log("currentIndex ", currentIndex)
            // console.log("currentValue key:", combb.model.get(currentIndex).key)
            // console.log("currentValue value:", combb.model.get(currentIndex).value)
            States.setRelicState(box.idx, combb.model.get(currentIndex).value)
            // States.setRelicState(box.idx, currentIndex)
        }
    }

    function setIndex(i)
    { 
      combb.currentIndex=i
    }
}
