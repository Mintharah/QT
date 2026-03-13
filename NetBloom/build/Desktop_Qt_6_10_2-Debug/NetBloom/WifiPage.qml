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
                text: wifiManager.status
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
                    text: "WiFi"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#8B3A62"
                }

                Item {
                    Layout.fillWidth: true
                }

                Switch {
                    id: wifiSwitch
                    checked: wifiManager.wifiEnabled
                    onToggled: {
                        if (checked)
                            wifiManager.turnOn()
                        else
                            wifiManager.turnOff()
                    }

                    indicator: Rectangle {
                        width: 50
                        height: 26
                        radius: 13
                        color: wifiSwitch.checked ? "#E91E8C" : "#FFD6E7"

                        Rectangle {
                            x: wifiSwitch.checked ? parent.width - width - 3 : 3
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
            color: wifiManager.wifiEnabled ? "#E91E8C" : "#FFD6E7"

            Text {
                anchors.centerIn: parent
                text: "Scan for Networks"
                font.pixelSize: 14
                font.bold: true
                color: wifiManager.wifiEnabled ? "white" : "#C06080"
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: wifiManager.wifiEnabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                onClicked: {
                    if (wifiManager.wifiEnabled)
                        wifiManager.scan()
                }
            }
        }

        Text {
            visible: wifiManager.networks.length > 0
            text: "Available Networks"
            font.pixelSize: 13
            font.bold: true
            color: "#8B3A62"
            Layout.topMargin: 4
        }

        Rectangle {
            Layout.fillWidth: true
            height: Math.min(wifiManager.networks.length * 54, 216)
            radius: 16
            color: "white"
            border.color: "#FFD6E7"
            border.width: 2
            visible: wifiManager.networks.length > 0
            clip: true

            ListView {
                id: networkList
                anchors.fill: parent
                anchors.margins: 8
                model: wifiManager.networks
                spacing: 4
                clip: true

                delegate: Rectangle {
                    width: networkList.width
                    height: 46
                    radius: 12
                    color: hovered ? "#FFF0F5" : "transparent"

                    property bool hovered: false

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12

                        Text {
                            text: "📶  " + modelData
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
                            ssidField.text = modelData
                            connectSheet.visible = true
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Rectangle {
        id: connectSheet
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 260
        radius: 24
        color: "white"
        visible: false

        layer.enabled: true

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 12

            Text {
                text: "Connect to Network"
                font.pixelSize: 16
                font.bold: true
                color: "#8B3A62"
            }

            TextField {
                id: ssidField
                Layout.fillWidth: true
                placeholderText: "Network name (SSID)"
                font.pixelSize: 14
                color: "#8B3A62"
                background: Rectangle {
                    radius: 12
                    color: "#FFF0F5"
                    border.color: "#FFD6E7"
                    border.width: 2
                }
            }

            TextField {
                id: passwordField
                Layout.fillWidth: true
                placeholderText: "Password"
                font.pixelSize: 14
                color: "#8B3A62"
                echoMode: TextInput.Password
                background: Rectangle {
                    radius: 12
                    color: "#FFF0F5"
                    border.color: "#FFD6E7"
                    border.width: 2
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Rectangle {
                    Layout.fillWidth: true
                    height: 44
                    radius: 22
                    color: "#FFD6E7"

                    Text {
                        anchors.centerIn: parent
                        text: "Cancel"
                        font.pixelSize: 14
                        font.bold: true
                        color: "#C06080"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            connectSheet.visible = false
                            passwordField.text = ""
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 44
                    radius: 22
                    color: "#E91E8C"

                    Text {
                        anchors.centerIn: parent
                        text: "Connect"
                        font.pixelSize: 14
                        font.bold: true
                        color: "white"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            wifiManager.connectToNetwork(ssidField.text,
                                                         passwordField.text)
                            connectSheet.visible = false
                            passwordField.text = ""
                        }
                    }
                }
            }
        }
    }
}
