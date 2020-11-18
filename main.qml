import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import liang 1.0


ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    minimumWidth: 400
    minimumHeight: 300
    title: qsTr("SerialPort")

    property bool isHex: false;  // 是否以16进制形式显示


    // 切换Hex按钮状态
    function switchHexChanged() {
        if (iSwitchHex.checked) {
            root.isHex = true;
        }
        else {
            root.isHex = false;
        }
    }

    // 清空接收的信息
    function clearReceiveMessage() {
        iTextAreaReceiveMessage.clear();
    }


    // 切换端口连接状态
    function switchSerialClicked() {
        if (iSwitchSerial.checked) {
            let portName = iportNameComboBox.currentText;
            let baudRate = ibaudRateComboBox.currentText;
            let dataBits = iDataBitsComboBox.currentText;
            let stopBits = iStopBitsComboBox.currentText;
            let parity = iParityComboBox.currentText;
            let flowControl = iFlowControlComboBox.currentText;
            let isOpen = SerialPort.open(portName, baudRate, dataBits, stopBits, parity, flowControl);
            if (!isOpen) {
                serialOpenDialog.open();
                iSwitchSerial.checked = false;
            }
        }
        else {
            SerialPort.close();
        }
    }


    // 更改调试模式
    function switchDebugChanged() {
        if (iswitchDebug.checked) {
            SerialPort.debug = true;
        }
        else {
            SerialPort.debug = false;
        }
    }


    // 清空发送的信息
    function clearSendMessage() {
        iTextAreaSendMessage.clear();
    }


    // 发送信息
    function sendMessage() {
        let text = iTextAreaSendMessage.text;
        SerialPort.sendMessage(text);
    }


    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 上半部分
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            // 收到的信息
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                TextArea {
                    id: iTextAreaReceiveMessage
                    wrapMode: Text.WrapAnywhere
                    selectByMouse: true
                    readOnly: true
                }
            }

            // 清屏和HEX切换按钮
            ColumnLayout {
                Layout.preferredWidth: 100
                Layout.maximumWidth: 100
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignBottom
                spacing: 10

                Button {
                    Layout.preferredWidth: 80
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("清屏")
                    onClicked: clearReceiveMessage()
                }

                Switch {
                    id: iSwitchHex
                    Layout.fillWidth: true
                    text: qsTr("HEX")
                    onToggled: switchHexChanged()
                }
            }
        }

        // 下半部分
        RowLayout {

            Layout.preferredHeight: 350
            Layout.maximumHeight: 350
            Layout.fillWidth: true

            // 左侧串口设置
            ColumnLayout {
                Layout.preferredWidth: 210
                Layout.maximumWidth: 210
                Layout.fillHeight: true

                RowLayout {
                    Layout.preferredHeight: 48
                    Layout.maximumHeight: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 60
                        text: qsTr("串口号:")
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox {
                        id: iportNameComboBox
                        enabled: !iSwitchSerial.checked
                    }
                }

                RowLayout {
                    Layout.preferredHeight: 48
                    Layout.maximumHeight: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 60
                        text: qsTr("波特率:")
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox {
                        id: ibaudRateComboBox
                        enabled: !iSwitchSerial.checked
                    }
                }
                RowLayout {
                    Layout.preferredHeight: 48
                    Layout.maximumHeight: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 60
                        text: qsTr("数据位:")
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox {
                        id: iDataBitsComboBox
                        enabled: !iSwitchSerial.checked
                    }
                }
                RowLayout {
                    Layout.preferredHeight: 48
                    Layout.maximumHeight: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 60
                        text: qsTr("停止位:")
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox {
                        id: iStopBitsComboBox
                        enabled: !iSwitchSerial.checked
                    }
                }
                RowLayout {
                    Layout.preferredHeight: 48
                    Layout.maximumHeight: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 60
                        text: qsTr("校验位:")
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox {
                        id: iParityComboBox
                        enabled: !iSwitchSerial.checked
                    }
                }
                RowLayout {
                    Layout.preferredHeight: 48
                    Layout.maximumHeight: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter

                    Label {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 60
                        text: qsTr("流控制:")
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox {
                        id: iFlowControlComboBox
                        enabled: !iSwitchSerial.checked
                    }
                }
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: 10

                // 端口开关
                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    Switch {
                        id: iSwitchSerial
                        text: iSwitchSerial.checked ? qsTr("打开端口") : qsTr("关闭端口")
                        onClicked: switchSerialClicked()
                    }
                }

                // 发送信息输入
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true

                        TextArea {
                            id: iTextAreaSendMessage
                            wrapMode: TextArea.Wrap
                            selectByMouse: true

                            background: Rectangle {
                                anchors.fill: parent
                                color: iTextAreaSendMessage.enabled ? "transparent" : "#353637"
                                border.color: iTextAreaSendMessage.enabled
                                              ? iTextAreaSendMessage.activeFocus ? Material.color(Material.Teal) : Material.color(Material.Grey)
                                              : Material.color(Material.Grey)
                            }
                        }
                    }
                }
            }

            // 清除和发送按钮
            ColumnLayout {
                Layout.preferredWidth: 100
                Layout.maximumWidth: 100
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignBottom
                spacing: 10

                Switch {
                    id: iswitchDebug
                    text: "调试"
                    onToggled: switchDebugChanged()

                }
                Button {
                    id: iButtonClear

                    Layout.preferredWidth: 80
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("清除")

                    onClicked: clearSendMessage()
                }
                Button {
                    id: iButtonSend

                    Layout.preferredWidth: 80
                    Layout.alignment: Qt.AlignHCenter
                    text: qsTr("发送")

                    onClicked: sendMessage()
                }
            }
        }
    }

    // 退出程序询问对话框
    Dialog {
        id: exitDialog

        anchors.centerIn: parent
        title: "提示：确认退出么？"
        modal: true
        contentWidth: 140
        contentHeight: 50

        contentItem: Row {
            spacing: 20
            Button {
                text: "取消"
                onClicked: {
                    exitDialog.reject();
                }
            }
            Button {
                text: "确认"
                onClicked: {
                    exitDialog.accept();
                }
            }
        }

        onAccepted: {
            SerialPort.close();
            Qt.quit();
        }
    }

    Dialog {
        id: serialOpenDialog

        anchors.centerIn: parent
        title: "提示：串口打开失败"
        modal: true
        contentWidth: 140
        contentHeight: 50

        contentItem: Row {
            Layout.alignment: Qt.AlignRight
            Button {
                text: "确认"
                onClicked: {
                    serialOpenDialog.accept();
                }
            }
        }

        onAccepted: {
            serialOpenDialog.close();
        }
    }

    // 每隔一秒获取当前所有的端口并刷新
    Timer {
        interval: 1000;
        running: true;
        repeat: true;

        onTriggered: {
            iportNameComboBox.model = SerialPort.portNames;
        }
    }

    // 关闭事件
    onClosing: {
        close.accepted = false;
        exitDialog.open();
    }

    Connections {
        target: SerialPort
        function onReadDataChanged(string) {
            let stringResult = "";
            if (!root.isHex) {
                let array = string.split(" ");
                for (let i = 0; i < array.length; i++) {
                    stringResult += String.fromCharCode(parseInt(array[i], 16));
                }

                iTextAreaReceiveMessage.append(stringResult);
            }
            else {
                iTextAreaReceiveMessage.append(string);
            }
        }
    }

    // 控件加载完成初始化
    Component.onCompleted: {
        iportNameComboBox.model = SerialPort.portNames;
        ibaudRateComboBox.model = SerialPort.baudRates;
        iDataBitsComboBox.model = SerialPort.allDataBits;
        iStopBitsComboBox.model = SerialPort.allStopBits;
        iParityComboBox.model = SerialPort.parities;
        iFlowControlComboBox.model = SerialPort.flowControls;

        ibaudRateComboBox.currentIndex = 5;
        iDataBitsComboBox.currentIndex = 3;
    }
}
