import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 480
    height: 800
    visible: true
    title: "🌸 Media Player"
    color: "#FFF0F5"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 70
            color: "#FFB6C1"

            Text {
                anchors.centerIn: parent
                text: "🌸 Multi Media Player"
                font.pixelSize: 24
                font.bold: true
                color: "#8B3A62"
            }
        }

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 340
            height: 44
            radius: 22
            color: "#FFD6E7"
            Layout.topMargin: 16
            Layout.bottomMargin: 16

            Rectangle {
                x: tabBar.currentIndex === 0 ? 4 :
                   tabBar.currentIndex === 1 ? parent.width / 3 :
                   parent.width / 3 * 2
                y: 4
                width: parent.width / 3 - 4
                height: parent.height - 8
                radius: 18
                color: "white"

                Behavior on x {
                    NumberAnimation { duration: 200; easing.type: Easing.OutQuart }
                }
            }

            RowLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillWidth: true
                    height: parent.height

                    Text {
                        anchors.centerIn: parent
                        text: "📻"
                        font.pixelSize: 14
                        font.bold: tabBar.currentIndex === 0
                        color: tabBar.currentIndex === 0 ? "#8B3A62" : "#C06080"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tabBar.currentIndex = 0
                    }
                }

                Item {
                    Layout.fillWidth: true
                    height: parent.height

                    Text {
                        anchors.centerIn: parent
                        text: "🎵"
                        font.pixelSize: 14
                        font.bold: tabBar.currentIndex === 1
                        color: tabBar.currentIndex === 1 ? "#8B3A62" : "#C06080"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tabBar.currentIndex = 1
                    }
                }

                Item {
                    Layout.fillWidth: true
                    height: parent.height

                    Text {
                        anchors.centerIn: parent
                        text: "🎬"
                        font.pixelSize: 14
                        font.bold: tabBar.currentIndex === 2
                        color: tabBar.currentIndex === 2 ? "#8B3A62" : "#C06080"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tabBar.currentIndex = 2
                    }
                }
            }
        }

        TabBar {
            id: tabBar
            visible: false
            currentIndex: 0
        }

        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex
            interactive: false
            onCurrentIndexChanged: tabBar.currentIndex = currentIndex

            RadioPage {}
            AudioPage {}
            VideoPage {}
        }
    }
}
