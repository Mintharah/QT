import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 400
    height: 700
    visible: true
    title: "Weather App"
    color: "#E8F4FD"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "🌤️ Weather"
            font.pixelSize: 28
            font.bold: true
            color: "#1A5276"
        }

        Rectangle {
            Layout.fillWidth: true
            height: 48
            radius: 24
            color: "white"
            border.color: "#AED6F1"
            border.width: 2

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8

                TextField {
                    id: searchField
                    Layout.fillWidth: true
                    placeholderText: "Search city..."
                    font.pixelSize: 14
                    color: "#1A5276"
                    background: Item {}
                    onAccepted: weatherManager.searchCity(searchField.text)
                }

                Text {
                    text: "🔍"
                    font.pixelSize: 18

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: weatherManager.searchCity(searchField.text)
                    }
                }
            }
        }

        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
            running: weatherManager.loading
            visible: weatherManager.loading
            width: 48
            height: 48
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: weatherManager.statusMsg
            font.pixelSize: 13
            color: "#C0392B"
            visible: weatherManager.statusMsg !== ""
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 220
            radius: 24
            color: "#2E86C1"
            visible: weatherManager.cityName !== ""

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 4

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: weatherManager.cityName
                    font.pixelSize: 22
                    font.bold: true
                    color: "white"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: weatherManager.weatherIcon
                    font.pixelSize: 56
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: weatherManager.temperature
                    font.pixelSize: 48
                    font.bold: true
                    color: "white"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: weatherManager.condition
                    font.pixelSize: 15
                    color: "#AED6F1"
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 24

                    Text {
                        text: "💧 " + weatherManager.humidity
                        font.pixelSize: 14
                        color: "white"
                    }

                    Text {
                        text: "💨 " + weatherManager.windSpeed
                        font.pixelSize: 14
                        color: "white"
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 160
            radius: 20
            color: "white"
            border.color: "#AED6F1"
            border.width: 2
            visible: weatherManager.forecast.length > 0

            Text {
                id: forecastTitle
                text: "5-Day Forecast"
                font.pixelSize: 20
                font.bold: true
                color: "#1A5276"
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 16
            }

            Row {
                anchors.top: forecastTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 8

                Repeater {
                    model: weatherManager.forecast

                    Item {
                        width: parent.width / 5
                        height: parent.height

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 4

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: modelData.day
                                font.pixelSize: 20
                                color: "#5D6D7E"
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: modelData.icon
                                font.pixelSize: 30
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: modelData.temp
                                font.pixelSize: 20
                                font.bold: true
                                color: "#1A5276"
                            }
                        }
                    }
                }
            }
        }
    }
}
