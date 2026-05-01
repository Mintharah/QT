import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Window {
    id: root
    width: 500
    height: 600
    visible: true
    title: "HVAC Control"

    Rectangle {
        anchors.fill: parent

        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: 0.0
                color: "#ffd6e7"
            }
            GradientStop {
                position: 0.5
                color: "#ffb3d1"
            }
            GradientStop {
                position: 1.0
                color: "#fce4ec"
            }
        }

        Rectangle {
            width: 220
            height: 220
            x: -60
            y: -60
            radius: 110
            color: "#44ffb6c1"
            layer.enabled: true
            layer.effect: MultiEffect {
                blurEnabled: true
                blur: 1.0
                blurMax: 40
            }
        }

        Rectangle {
            width: 200
            height: 200
            x: parent.width - 140
            y: parent.height - 140
            radius: 100
            color: "#33ff69b4"
            layer.enabled: true
            layer.effect: MultiEffect {
                blurEnabled: true
                blur: 1.0
                blurMax: 40
            }
        }

        // glass panel
        Rectangle {
            id: glassPanel
            anchors.centerIn: parent
            width: 360
            height: 550
            radius: 28
            color: "#47ffffff"
            border.color: "#99ffffff"
            border.width: 1.5
            opacity: hvac.power ? 1.0 : 0.55

            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 14

                //header
                RowLayout {
                    Layout.fillWidth: true

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2

                        Text {
                            text: "Climate Control"
                            font.pixelSize: 20
                            font.family: "Georgia"
                            color: "#a03c64"
                        }

                        Text {
                            text: "Front Cabin · " + hvac.mode.charAt(0).toUpperCase() + hvac.mode.slice(1) + " Mode"
                            font.pixelSize: 11
                            color: "#99b45078"
                        }
                    }
                }

                // temp arc
                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 160

                    Item {
                        anchors.centerIn: parent
                        width: 160
                        height: 160

                        Canvas {
                            id: arcCanvas
                            anchors.fill: parent

                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.clearRect(0, 0, width, height);

                                var cx = width / 2;
                                var cy = height / 2;
                                var r = 70;
                                var startAngle = (-130 - 90) * Math.PI / 180;
                                var fullAngle = 260 * Math.PI / 180;

                                ctx.beginPath();
                                ctx.arc(cx, cy, r, startAngle, startAngle + fullAngle);
                                ctx.strokeStyle = "#33ffffff";
                                ctx.lineWidth = 8;
                                ctx.lineCap = "round";
                                ctx.stroke();

                                var pct = (hvac.temperature - 16) / (30 - 16);
                                if (pct > 0) {
                                    ctx.beginPath();
                                    ctx.arc(cx, cy, r, startAngle, startAngle + pct * fullAngle);
                                    ctx.strokeStyle = "#ff69b4";
                                    ctx.lineWidth = 8;
                                    ctx.lineCap = "round";
                                    ctx.stroke();
                                }
                            }

                            Connections {
                                target: hvac
                                function onTemperatureChanged() {
                                    arcCanvas.requestPaint();
                                }
                            }
                        }

                        Column {
                            anchors.centerIn: parent
                            spacing: 2

                            Text {
                                text: hvac.temperature
                                font.pixelSize: 48
                                font.family: "Georgia"
                                color: "#a03c64"
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Text {
                                text: "°C"
                                font.pixelSize: 13
                                color: "#99b45078"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }

                //temp buttons
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 24

                    Rectangle {
                        width: 40
                        height: 40
                        radius: 20
                        color: "#55ffffff"
                        border.color: "#99ffffff"
                        border.width: 1.5

                        Text {
                            anchors.centerIn: parent
                            text: "−"
                            font.pixelSize: 24
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: if (hvac.power)
                                hvac.setTemperature(hvac.temperature - 1)
                        }
                    }

                    Text {
                        text: "Temperature"
                        font.pixelSize: 11
                        color: "#99b45078"
                    }

                    Rectangle {
                        width: 40
                        height: 40
                        radius: 20
                        color: "#55ffffff"
                        border.color: "#99ffffff"
                        border.width: 1.5

                        Text {
                            anchors.centerIn: parent
                            text: "+"
                            font.pixelSize: 24
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: if (hvac.power)
                                hvac.setTemperature(hvac.temperature + 1)
                        }
                    }
                }

                // divider
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop {
                            position: 0.0
                            color: "transparent"
                        }
                        GradientStop {
                            position: 0.5
                            color: "#44ff69b4"
                        }
                        GradientStop {
                            position: 1.0
                            color: "transparent"
                        }
                    }
                }

                // mode buttons
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Repeater {
                        model: [
                            {
                                label: "Cool",
                                icon: "❄️",
                                value: "cool"
                            },
                            {
                                label: "Heat",
                                icon: "🔆",
                                value: "heat"
                            },
                            {
                                label: "Auto",
                                icon: "♻️",
                                value: "auto"
                            },
                            {
                                label: "Off",
                                icon: "🚫",
                                value: "off"
                            }
                        ]

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 60
                            radius: 14

                            color: hvac.mode === modelData.value ? "#44ff69b4" : "#44ffffff"
                            border.color: hvac.mode === modelData.value ? "#99ff69b4" : "#99ffffff"
                            border.width: 1.5

                            Column {
                                anchors.centerIn: parent
                                spacing: 4

                                Text {
                                    text: modelData.icon
                                    font.pixelSize: 18
                                }
                                Text {
                                    text: modelData.label
                                    font.pixelSize: 10
                                    color: "#a03c64"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: if (hvac.power)
                                    hvac.setMode(modelData.value)
                            }
                        }
                    }
                }
                //fan text
                Text {
                    text: "Fan Speed"
                    font.pixelSize: 11
                    color: "#99b45078"
                }

                // fan slider
                RowLayout {
                    Text {
                        text: "Low"
                        font.pixelSize: 11
                        color: "#99b45078"
                    }
                    Slider {
                        id: fanSlider
                        Layout.fillWidth: true

                        from: 0
                        to: 5
                        stepSize: 1
                        value: hvac.fanSpeed
                        enabled: hvac.power

                        onValueChanged: hvac.setFanSpeed(value)

                        // track
                        background: Item {
                            implicitHeight: 8

                            Rectangle {
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width
                                height: 6
                                radius: 3
                                color: "#22ffffff"

                                border.color: "#33ffffff"
                            }

                            // filled part
                            Rectangle {
                                anchors.verticalCenter: parent.verticalCenter
                                width: fanSlider.visualPosition * parent.width
                                height: 6
                                radius: 3

                                gradient: Gradient {
                                    GradientStop {
                                        position: 0.0
                                        color: "#ffb6c1"
                                    }
                                    GradientStop {
                                        position: 1.0
                                        color: "#ff69b4"
                                    }
                                }
                            }
                        }

                        //handle
                        handle: Rectangle {
                            width: 20
                            height: 20
                            radius: 10

                            x: fanSlider.visualPosition * (fanSlider.availableWidth - width)
                            y: (fanSlider.height - height) / 2

                            color: "#ffffff"
                            border.color: "#ff69b4"
                            border.width: 2

                            layer.enabled: true
                            layer.effect: MultiEffect {
                                shadowEnabled: true
                                shadowBlur: 0.6
                                shadowColor: "#66000000"
                            }

                            // press feedback
                            scale: fanSlider.pressed ? 1.15 : 1.0
                            Behavior on scale {
                                NumberAnimation {
                                    duration: 120
                                }
                            }
                        }
                    }
                    Text {
                        text: "High"
                        font.pixelSize: 11
                        color: "#99b45078"
                    }
                }

                // power button
                RowLayout {
                    Layout.fillWidth: true
                    Rectangle {
                        width: 48
                        height: 48
                        radius: 24
                        color: hvac.power ? "#55ff69b4" : "#44ffffff"
                        border.color: hvac.power ? "#99ff69b4" : "#99ffffff"
                        border.width: 1.5

                        Text {
                            anchors.centerIn: parent
                            text: "⏻"
                            font.pixelSize: 20
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: hvac.setPower(!hvac.power)
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
