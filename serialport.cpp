#include "serialport.h"


/**
 * @brief SerialPort::SerialPort : 初始化
 * @param parent
 */
SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    m_debug = false;
    m_serialPort = new QSerialPort();
    if(m_serialPort->isOpen())  //如果串口已经打开了 先给他关闭了
    {
        m_serialPort->clear();
        m_serialPort->close();
    }
    m_serialPortInfo = new QSerialPortInfo();

    m_baudRates = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };
    m_allDataBits = { 5, 6, 7, 8 };
    m_allStopBits = { 1, 1.5, 2 };
    m_parities = { "None", "Even", "Odd", "Space", "Mark" };
    m_flowControls = { "None", "Hardware", "Software" };

    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPort::readData);
}


/**
 * @brief SerialPort::~SerialPort : 析构
 */
SerialPort::~SerialPort() {
    delete m_serialPort;
    delete m_serialPortInfo;
}


/**
 * @brief SerialPort::debug : 当返回前调试模式的状态
 * @return
 */
bool SerialPort::debug(void) {
    return m_debug;
}


/**
 * @brief SerialPort::setDebug : 设置当前调试模式状态
 * @param debug : 调试模式状态
 */
void SerialPort::setDebug(bool debug) {
    if (m_debug != debug) {
        m_debug = debug;
        emit debugChanged();
    }
}

/**
 * @brief SerialPort::portNames : 可选的所有的端口
 * @return 可用端口的列表
 */
QVariantList SerialPort::portNames(void) {
    QVariantList portNamesList;
    QList<QSerialPortInfo> serialPortInfoList;
    serialPortInfoList = m_serialPortInfo->availablePorts();
    for (QSerialPortInfo i : serialPortInfoList) {
        portNamesList << i.portName();
    }
    return portNamesList;
}


/**
 * @brief SerialPort::portName : 当前端口号
 * @return
 */
QString SerialPort::portName(void) {
    return m_portName;
}


/**
 * @brief SerialPort::setPortName : 设置当前端口号
 * @param portName
 */
void SerialPort::setPortName(const QString& portName) {
    if (m_portName != portName) {
        m_portName = portName;
        emit portNameChanged();
    }
}


/**
 * @brief SerialPort::baudRates : 可选的所有波特率
 * @return
 */
QVariantList SerialPort::baudRates(void) {
    return m_baudRates;
}


/**
 * @brief SerialPort::baudRate : 当前波特率
 * @return
 */
int SerialPort::baudRate(void) {
    return m_baudRate;
}


/**
 * @brief SerialPort::setBaudRate : 设置当前波特率
 * @param baudRate
 */
void SerialPort::setBaudRate(const int& baudRate) {
    if (m_baudRate != baudRate) {
        m_baudRate = baudRate;
        emit baudRateChanged();
    }
}


/**
 * @brief SerialPort::allDataBits : 可选的所有数据位
 * @return
 */
QVariantList SerialPort::allDataBits(void) {
    return m_allDataBits;
}


/**
 * @brief SerialPort::dataBits : 当前数据位
 * @return
 */
int SerialPort::dataBits(void) {
    return m_dataBits;
}


/**
 * @brief SerialPort::setDataBits : 设置数据位
 * @param dataBits
 */
void SerialPort::setDataBits(const int& dataBits) {
    if (m_dataBits != dataBits) {
        m_dataBits = dataBits;
        emit dataBitsChanged();
    }
}


/**
 * @brief SerialPort::allStopBits : 可选的所有停止位
 * @return
 */
QVariantList SerialPort::allStopBits(void) {
    return m_allStopBits;
}


/**
 * @brief SerialPort::stopBits : 当前停止位
 * @return
 */
float SerialPort::stopBits(void) {
    return m_stopBits;
}


/**
 * @brief SerialPort::setStopBits : 设置停止位
 * @param stopBits
 */
void SerialPort::setStopBits(const float& stopBits) {
    if (m_stopBits != stopBits) {
        m_stopBits = stopBits;
        emit stopBitsChanged();
    }
}


/**
 * @brief SerialPort::parities : 可选的所有校验位
 * @return
 */
QVariantList SerialPort::parities(void) {
    return m_parities;
}


/**
 * @brief SerialPort::parity : 当前校验位
 * @return
 */
QString SerialPort::parity(void) {
    return m_parity;
}


/**
 * @brief SerialPort::setParity : 设置校验位
 * @param parity
 */
void SerialPort::setParity(const QString& parity) {
    if (m_parity != parity) {
        m_parity = parity;
        emit parityChanged();
    }
}


/**
 * @brief SerialPort::flowControls : 可选的所有流控制
 * @return
 */
QVariantList SerialPort::flowControls(void) {
    return m_flowControls;
}


/**
 * @brief SerialPort::flowControl : 当前流控制
 * @return
 */
QString SerialPort::flowControl(void) {
    return m_flowControl;
}


/**
 * @brief SerialPort::setFlowControl : 设置流控制
 * @param flowControl
 */
void SerialPort::setFlowControl(const QString& flowControl) {
    if (m_flowControl != flowControl) {
        m_flowControl = flowControl;
        emit flowControlChanged();
    }
}


/**
 * @brief SerialPort::readData : 收到数据时读取数据,并发射包含数据的信号到界面
 */
void SerialPort::readData() {
    const QByteArray byteData = m_serialPort->readAll();
    if (m_debug) {  // 如果处于调试模式下，那么自动发送对应数据用来测试
        debugAutoSend(byteData);
    }
    QString data = byteData.toHex(' ');  // 转成十六进制中间以空格相隔
    emit readDataChanged(data);
}


/**
 * @brief SerialPort::debugAutoSend : 调试模式下发送消息
 * @param receiveByte : 接收到的字节型数据
 */
void SerialPort::debugAutoSend(const QByteArray& receiveBytes) {
    DebugSendMessage* debugSendMessage = new DebugSendMessage();
    QByteArray sendBytes = debugSendMessage->parse(receiveBytes);
    if (!m_serialPort->waitForBytesWritten()) {
        m_serialPort->write(sendBytes);
    }
    delete debugSendMessage;
}


/**
 * @brief SerialPort::sendMessage : 发送消息
 * @param message
 */
void SerialPort::sendMessage(const QString& message) {
    if (!m_serialPort->waitForBytesWritten()) {
        m_serialPort->write(message.toUtf8());
    }
}


/**
 * @brief SerialPort::open : 端口打开
 * @param argPortName : 端口号
 * @param argBaudRate : 波特率
 * @param argDataBits : 数据位
 * @param argStopBits : 停止位
 * @param argParity : 校验位
 * @param argFlowControl : 流控制
 * @return : 是否打开成功
 */
bool SerialPort::open(
        const QString& argPortName,
        const int argBaudRate,
        const int argDataBits,
        const float argStopBits,
        const QString& argParity,
        const QString& argFlowControl) {

    // 设置端口

    m_serialPort->setPortName(argPortName);
    // 设置波特率
    if (argBaudRate == m_baudRates[0].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud1200);
    }
    else if (argBaudRate == m_baudRates[1].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud2400);
    }
    else if (argBaudRate == m_baudRates[2].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud4800);
    }
    else if (argBaudRate == m_baudRates[3].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud9600);
    }
    else if (argBaudRate == m_baudRates[4].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud19200);
    }
    else if (argBaudRate == m_baudRates[5].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud38400);
    }
    else if (argBaudRate == m_baudRates[6].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud57600);
    }
    else if (argBaudRate == m_baudRates[7].toInt()) {
        m_serialPort->setBaudRate(QSerialPort::Baud115200);
    }

    // 设置数据位
    if (argDataBits == m_allDataBits[0].toInt()) {
        m_serialPort->setDataBits(QSerialPort::Data5);
    }
    else if (argDataBits == m_allDataBits[1].toInt()) {
        m_serialPort->setDataBits(QSerialPort::Data6);
    }
    else if (argDataBits == m_allDataBits[2].toInt()) {
        m_serialPort->setDataBits(QSerialPort::Data7);
    }
    else if (argDataBits == m_allDataBits[3].toInt()) {
        m_serialPort->setDataBits(QSerialPort::Data8);
    }

    // 设置停止位
    if (argStopBits == m_allStopBits[0].toFloat()) {
        m_serialPort->setStopBits(QSerialPort::OneStop);
    }
    else if (argStopBits == m_allStopBits[1].toFloat()) {
        m_serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if (argStopBits == m_allDataBits[2].toFloat()) {
        m_serialPort->setStopBits(QSerialPort::TwoStop);
    }

    // 设置校验位
    if (argParity == m_parities[0].toString()) {
        m_serialPort->setParity(QSerialPort::NoParity);
    }
    else if (argParity == m_parities[1].toString()) {
        m_serialPort->setParity(QSerialPort::EvenParity);
    }
    else if (argParity == m_parities[2].toString()) {
        m_serialPort->setParity(QSerialPort::OddParity);
    }
    else if (argParity == m_parities[3].toString()) {
        m_serialPort->setParity(QSerialPort::SpaceParity);
    }
    else if (argParity == m_parities[4].toString()) {
        m_serialPort->setParity(QSerialPort::MarkParity);
    }

    // 流控制
    if (argFlowControl == m_flowControls[0].toString()) {
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }
    else if (argFlowControl == m_flowControls[1].toString()) {
        m_serialPort->setFlowControl(QSerialPort::HardwareControl);
    }
    else if (argFlowControl == m_flowControls[2].toString()) {
        m_serialPort->setFlowControl(QSerialPort::SoftwareControl);
    }

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << argPortName << "已打开";
        return true;
    }
    else {
        qDebug() << argPortName << "打开失败";
        return false;
    }
}


/**
 * @brief SerialPort::close : 关闭端口
 */
void SerialPort::close(void) {
    qDebug() << "断开";
    if (m_serialPort->isOpen()) {
        m_serialPort->clear();
        m_serialPort->close();
    }
}
