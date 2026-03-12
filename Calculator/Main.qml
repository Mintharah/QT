import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 360
    height: 580
    visible: true
    title: "Calculator"
    color: "#FFF0F5"

    //calculator logic
    property string currentInput: "0"
    property string firstNumber: ""
    property string operator: ""
    property bool freshInput: false //are we starting a new number?

    //called when a button is pressed
    function numberPressed(num) {
        if (freshInput) {
            currentInput = num
            freshInput = false
        } else {
            if (currentInput === "0") {
                currentInput = num
            } else {
                currentInput = currentInput + num
            }
        }
        displayText.text = currentInput
    }

    //called when an operator is pressed
    function operatorPressed(op) {
        firstNumber = currentInput
        operator = op
        freshInput = true
    }

    //called when = is pressed
    function equalsPressed() {
        var num1 = parseFloat(firstNumber)
        var num2 = parseFloat(currentInput)
        var result = 0

        if (operator === "+")
            result = num1 + num2
        if (operator === "-")
            result = num1 - num2
        if (operator === "×")
            result = num1 * num2
        if (operator === "÷") {
            if (num2 === 0) {
                displayText.text = "Error"
                currentInput = "0"
                return
            }
            result = num1 / num2
        }
        currentInput = String(result)
        displayText.text = currentInput
        freshInput = true
    }

    //called when c is pressed
    function clearPressed() {
        currentInput = "0"
        firstNumber = ""
        operator = ""
        freshInput = false
        displayText.text = "0"
    }

    //display box
    Rectangle {
        id: displayBox
        width: 340
        height: 120
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        color: "#FFB6C1"
        radius: 16

        Text {
            id: displayText
            text: "0"
            font.pixelSize: 52
            font.bold: true
            color: "#8B3A62"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 16
        }
    }

    //button grid
    GridLayout {
        id: buttonGrid
        columns: 4
        width: 340
        anchors.top: displayBox.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        rowSpacing: 12
        columnSpacing: 12

        //reusable button component
        component CalcButton: Rectangle {
            property string label: ""
            property color btnColor: "#FFD6E7"
            signal tapped()

            width: 72
            height: 72
            radius: 36
            color: btnColor

            Text {
                text: label
                font.pixelSize: 22
                font.bold: true
                color: "#8B3A62"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: tapped()

                //darken button when pressed
                onPressed: parent.color = Qt.darker(btnColor, 1.2)
                onReleased: parent.color = btnColor
            }
        }

        //row 1
        CalcButton {
            label: "C"
            btnColor: "#FFB3C6"
            onTapped: clearPressed()
        }
        CalcButton {
            label: "±"
            btnColor: "#FFB3C6"
            onTapped: {
                currentInput = String(parseFloat(currentInput) * -1)
                displayText.text = currentInput
            }
        }
        CalcButton {
            label: "%"
            btnColor: "#FFB3C6"
            onTapped: {
                currentInput = String(parseFloat(currentInput) / 100)
                displayText.text = currentInput
            }
        }
        CalcButton {
            label: "÷"
            btnColor: "#E91E8C"
            onTapped: operatorPressed("÷")
        }

        //row 2
        CalcButton {
            label: "7"
            onTapped: numberPressed("7")
        }
        CalcButton {
            label: "8"
            onTapped: numberPressed("8")
        }
        CalcButton {
            label: "9"
            onTapped: numberPressed("9")
        }
        CalcButton {
            label: "×"
            btnColor: "#E91E8C"
            onTapped: operatorPressed("×")
        }

        // row 3
        CalcButton {
            label: "4"
            onTapped: numberPressed("4")
        }
        CalcButton {
            label: "5"
            onTapped: numberPressed("5")
        }
        CalcButton {
            label: "6"
            onTapped: numberPressed("6")
        }
        CalcButton {
            label: "-"
            btnColor: "#E91E8C"
            onTapped: operatorPressed("-")
        }

        // row 4
        CalcButton {
            label: "1"
            onTapped: numberPressed("1")
        }
        CalcButton {
            label: "2"
            onTapped: numberPressed("2")
        }
        CalcButton {
            label: "3"
            onTapped: numberPressed("3")
        }
        CalcButton {
            label: "+"
            btnColor: "#E91E8C"
            onTapped: operatorPressed("+")
        }

        //row 5
        CalcButton {
            label: "0"
            Layout.columnSpan: 2
            Layout.preferredWidth: 156
            onTapped: numberPressed("0")
        }

        CalcButton {
            label: "."
            onTapped: {
                if (!currentInput.includes(".")) {
                    currentInput = currentInput + "."
                    displayText.text = currentInput
                }
            }
        }
        CalcButton {
            label: "="
            btnColor: "#E91E8C"
            onTapped: equalsPressed()
        }
    }
}
