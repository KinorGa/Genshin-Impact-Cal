import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    
    ListModel{
      id: basicAttrModel
      ListElement { name: "基础攻击力"; key: 0 }
      ListElement { name: "额外攻击力"; key: 1 }
      ListElement { name: "基础生命值"; key: 2 }
      ListElement { name: "额外生命值"; key: 3 }
      ListElement { name: "基础防御力"; key: 4 }
      ListElement { name: "额外防御力"; key: 5 }
    }

    ListModel{
      id: percentAttrModel
      ListElement { name: "攻击力百分比"; key: 6 }
      ListElement { name: "生命值百分比"; key: 7 }
      ListElement { name: "防御力百分比"; key: 8 }
      ListElement { name: "暴击率"; key: 9 }
      ListElement { name: "暴击伤害"; key: 10 }
      ListElement { name: "元素充能效率"; key: 11 }
    }

    ListModel{
      id: reactAttrModel
      ListElement { name: "元素精通"; key: 12 }
      ListElement { name: "反应系数提升"; key: 13 }
    }

    ListModel{
      id: skillAttrModel
      ListElement { name: "攻击倍率"; key: 14 }
      ListElement { name: "生命倍率"; key: 15 }
      ListElement { name: "防御倍率"; key: 16 }
      ListElement { name: "额外倍率"; key: 17 }
    }

    ListModel{
      id: elementalAttrModel
      ListElement { name: "风元素伤害加成"; key: 18 }
      ListElement { name: "火元素伤害加成"; key: 19 }
      ListElement { name: "水元素伤害加成"; key: 20 }
      ListElement { name: "岩元素伤害加成"; key: 21 }
      ListElement { name: "雷元素伤害加成"; key: 22 }
      ListElement { name: "草元素伤害加成"; key: 23 }
      ListElement { name: "冰元素伤害加成"; key: 24 }
    }

    ListModel{
      id: dmgAttrModel
      ListElement { name: "伤害加成"; key: 25 }
    }

    ListModel{
      id: shieldAttrModel
      ListElement { name: "降低防御"; key: 26 }
      ListElement { name: "无视防御"; key: 27 }
    }

    ListModel{
      id: resAttrModel
      ListElement { name: "元素抗性"; key: 28 }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 滑动页面容器（同级页面切换）
        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.fillHeight: true

            Repeater {
                model: [
                    basicAttrModel,
                    percentAttrModel,
                    reactAttrModel,
                    skillAttrModel,
                    elementalAttrModel,
                    dmgAttrModel,
                    shieldAttrModel,
                    resAttrModel
                ]

                delegate: Buffer {
                    required property var modelData

                    Layout.fillWidth: true
                    dynmodel: modelData
                }
            }
        }

        // 底部标签栏
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            background: Rectangle {
                color: "white"
                border.color: "#dddddd"
            }
            currentIndex: swipeView.currentIndex // 与SwipeView联动
            onCurrentIndexChanged: {
                swipeView.currentIndex = currentIndex;
            }

            TabButton {
                text: "基础属性"
            }

            TabButton {
                text: "百分比属性"
            }

            TabButton {
                text: "反应属性"
            }

            TabButton{
                text: "倍率"
            }

            TabButton {
                text: "元素伤害加成"
            }

            TabButton {
                text: "伤害加成"
            }

            TabButton {
                text: "防御"
            }

            TabButton {
                text: "抗性"
            }
        }
    }
}
