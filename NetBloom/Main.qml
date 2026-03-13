import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 420
    height: 700
    visible: true
    title: "NetBloom"
    color: "#FFF0F5"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 80
            color: "#FFF0F5"

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 2

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "🌸 NetBloom"
                    font.pixelSize: 26
                    font.bold: true
                    color: "#8B3A62"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Network Manager"
                    font.pixelSize: 12
                    color: "#C06080"
                }
            }
        }

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 260
            height: 44
            radius: 22
            color: "#FFD6E7"
            Layout.bottomMargin: 16

            Rectangle {
                x: tabBar.currentIndex === 0 ? 4 : parent.width / 2
                y: 4
                width: parent.width / 2 - 4
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
                        text: "📶  WiFi"
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
                        text: "🔵  Bluetooth"
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

            WifiPage {}
            BluetoothPage {}
        }
    }
}
