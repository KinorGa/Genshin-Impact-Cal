import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

// import QtCharts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Genshin Impact Calculator")
    color: "white"

    StackLayout {
        id: view

        currentIndex: 0
        anchors.fill: parent
        // interactive: false

        Item {
            id: firstPage
            ColumnLayout{
              anchors.fill: parent
              spacing: 0

              MenuRec{
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                // z: 10
                nextPageBtn.onClicked: {
                  view.currentIndex = 1
                }
              }

              RowLayout{
                // anchors.fill: parent
                spacing: 0
                RelicPanel{
                  Layout.fillHeight: true
                  Layout.preferredWidth: 320
                  // z: 10
                }
                BufferPage{
                  Layout.fillHeight: true
                  Layout.fillWidth: true
                }
              }
            }
        }
        Item {
            id: secondPage

            width: view.width
            height: view.height

            // ✅ CRITICAL: Table with explicit width/height
            Table {
                anchors.fill: parent
                prevPageBtn.onClicked: {
                  view.currentIndex = 0
                }
            }
        }
        Item {
            id: thirdPage
        }
    }

    // PageIndicator {
    //     id: indicator

    //     count: view.count
    //     currentIndex: view.currentIndex

    //     anchors.bottom: view.bottom
    //     anchors.horizontalCenter: parent.horizontalCenter
    // }

    // ColumnLayout{
    //   anchors.fill: parent
    //   spacing: 0

    //   MenuRec{
    //     Layout.fillWidth: true
    //     Layout.preferredHeight: 40
    //     z: 10
    //   }

    //   RowLayout{
    //     // anchors.fill: parent
    //     spacing: 0
    //     RelicPanel{
    //       Layout.fillHeight: true
    //       Layout.preferredWidth: 320
    //       z: 10
    //     }
    //     BufferPage{
    //       Layout.fillHeight: true
    //       Layout.fillWidth: true
    //     }
    //   }
    // }
}
