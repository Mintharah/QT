import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: SwipeView.view ? SwipeView.view.width : 0
    height: SwipeView.view ? SwipeView.view.height : 0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 12

        Rectangle {
            Layout.fillWidth: true
            height: 36
            radius: 10
            color: "#FFE4EE"

            Text {
                anchors.centerIn: parent
                text: bluetoothManager.status
                font.pixelSize: 12
                color: "#C06080"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 64
            radius: 16
            color: "white"
            border.color: "#FFD6E7"
            border.width: 2

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16

                Text {
                    text: "Bluetooth"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#8B3A62"
                }

                Item {
                    Layout.fillWidth: true
                }

                Switch {
                    id: btSwitch
                    checked: bluetoothManager.bluetoothEnabled
                    onToggled: {
                        if (checked)
                            bluetoothManager.turnOn()
                        else
                            bluetoothManager.turnOff()
                    }

                    indicator: Rectangle {
                        width: 50
                        height: 26
                        radius: 13
                        color: btSwitch.checked ? "#E91E8C" : "#FFD6E7"

                        Rectangle {
                            x: btSwitch.checked ? parent.width - width - 3 : 3
                            y: 3
                            width: 20
                            height: 20
                            radius: 10
                            color: "white"

                            Behavior on x {
                                NumberAnimation {
                                    duration: 150
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 46
            radius: 23
            color: bluetoothManager.bluetoothEnabled ? "#E91E8C" : "#FFD6E7"

            Text {
                anchors.centerIn: parent
                text: "Scan for Devices"
                font.pixelSize: 14
                font.bold: true
                color: bluetoothManager.bluetoothEnabled ? "white" : "#C06080"
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: bluetoothManager.bluetoothEnabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                onClicked: {
                    if (bluetoothManager.bluetoothEnabled)
                        bluetoothManager.scan()
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 36
            radius: 10
            color: "#FFF0F5"
            visible: bluetoothManager.status === "Scanning for devices..."

            RowLayout {
                anchors.centerIn: parent
                spacing: 8

                BusyIndicator {
                    width: 20
                    height: 20
                    running: bluetoothManager.status === "Scanning for devices..."
                }

                Text {
                    text: "Scanning... please wait"
                    font.pixelSize: 12
                    color: "#C06080"
                }
            }
        }

        Text {
            visible: bluetoothManager.devices.length > 0
            text: "Nearby Devices"
            font.pixelSize: 13
            font.bold: true
            color: "#8B3A62"
            Layout.topMargin: 4
        }

        Rectangle {
            Layout.fillWidth: true
            height: Math.min(bluetoothManager.devices.length * 54, 216)
            radius: 16
            color: "white"
            border.color: "#FFD6E7"
            border.width: 2
            visible: bluetoothManager.devices.length > 0
            clip: true

            ListView {
                id: deviceList
                anchors.fill: parent
                anchors.margins: 8
                model: bluetoothManager.devices
                spacing: 4
                clip: true

                delegate: Rectangle {
                    width: deviceList.width
                    height: 46
                    radius: 12
                    color: hovered ? "#FFF0F5" : "transparent"

                    property bool hovered: false

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12

                        Text {
                            text: "🔵  " + modelData
                            font.pixelSize: 13
                            color: "#8B3A62"
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        Text {
                            text: "›"
                            font.pixelSize: 20
                            color: "#E91E8C"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: parent.hovered = true
                        onExited: parent.hovered = false
                        onClicked: {
                            var raw = modelData
                            var start = raw.lastIndexOf("(")
                            var end = raw.lastIndexOf(")")
                            var address = raw.substring(start + 1, end)
                            bluetoothManager.connectToDevice(address)
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 120
            radius: 16
            color: "white"
            border.color: "#FFD6E7"
            border.width: 2
            visible: bluetoothManager.bluetoothEnabled
                     && bluetoothManager.devices.length === 0
                     && bluetoothManager.status !== "Scanning for devices..."

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 6

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "🔍"
                    font.pixelSize: 30
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Press scan to find nearby devices"
                    font.pixelSize: 12
                    color: "#C06080"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 120
            radius: 16
            color: "white"
            border.color: "#FFD6E7"
            border.width: 2
            visible: !bluetoothManager.bluetoothEnabled

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 6

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "💤"
                    font.pixelSize: 30
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Turn on Bluetooth to get started"
                    font.pixelSize: 12
                    color: "#C06080"
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
