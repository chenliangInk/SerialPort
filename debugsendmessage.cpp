#include "debugsendmessage.h"


/**
 * @brief DebugSendMessage::DebugSendMessage : 初始化
 * @param parent
 */
DebugSendMessage::DebugSendMessage(QObject *parent) : QObject(parent)
{
    // 初始化通道的数据
    for (int i = 0; i < CHANNEL_SIZE; i++) {
        m_channelDatas[i][0] = 0;
        if (i % 2) {
            m_channelDatas[i][1] = 0x01;
        }
        else {
            m_channelDatas[i][1] = 0x00;
        }

        m_channelDatas[i][2] = i / 256;
        m_channelDatas[i][3] = i % 256;
        m_channelDatas[i][4] = 0;
        m_channelDatas[i][5] = 0;
    }

}


/**
 * @brief DebugSendMessage::~DebugSendMessage : 析构
 */
DebugSendMessage::~DebugSendMessage() {

}


QByteArray DebugSendMessage::parse(const QByteArray& bytesReceive) {
    unsigned int receiveLength = bytesReceive.length();
    QByteArray sendByte = QByteArray("");
    if (receiveLength != 8) {  // 如果长度不等于8， 那么返回空字节
        qDebug() << "数据长度不等于8";
    }
    else {
        QVector<unsigned char> crcs = crcDetect(bytesReceive);
        if (crcs[0] != (unsigned char)(bytesReceive[receiveLength - 2])
                || crcs[1] != (unsigned char)(bytesReceive[receiveLength - 1])) {
            qDebug() << "crc校验失败";
        }
        else {
            qDebug() << "crc校验成功";
            sendByte = simulateData(bytesReceive);
        }
    }

    return sendByte;
}


QVector<unsigned char> DebugSendMessage::crcDetect(const QByteArray& bytesReceive) {
    unsigned char crcHigh = 0xff;
    unsigned char crcLow = 0xff;
    unsigned int index;
    unsigned int length = bytesReceive.length();
    for (unsigned int i = 0; i < length - 2; i++) {
        // 必须强制转换成unsigned char其他int, unsigned int等都不行
        index = crcHigh ^ (unsigned char)(bytesReceive[i]);
        crcHigh = crcLow ^ m_auchCrcHigh[index];
        crcLow = m_auchCrcLow[index];
    }
    return { crcHigh, crcLow };
}


QByteArray DebugSendMessage::simulateData(const QByteArray& bytesReceive) {
    unsigned char controlAddress = bytesReceive[0];  // 获取控制器地址
    unsigned char functionCode = 0x04;  // 功能码
    unsigned int registerStartAddress = (unsigned char)(bytesReceive[2]) * 256
            + (unsigned char)(bytesReceive[3]);  // 寄存器开始地址
    unsigned int registerNumbers = (unsigned char)(bytesReceive[4]) * 256
            + (unsigned char)(bytesReceive[5]);  // 寄存器数量

    QByteArray byteChannelData = channelDataBytes(
                controlAddress,
                functionCode,
                registerStartAddress,
                registerNumbers);

    return byteChannelData;
}


QByteArray DebugSendMessage::channelDataBytes(
        const unsigned char controlAddress,
        const unsigned char functionCode,
        const unsigned char registerStartAddress,
        const unsigned char registerNumbers) {
    QByteArray byteArray;
    byteArray += controlAddress;
    byteArray += functionCode;
    // unsigned char byteSize = vector.size() * 5 + 5;
    unsigned char byteSize = registerNumbers * 2;
    byteArray += byteSize;
    qDebug() << "registerStart: " << registerStartAddress << "registerNumbers: " << registerNumbers;
    for (int i = 0; i < CHANNEL_SIZE; i++) {
        for (int j = 0; j < 6; j++) {
            int address = i * 6 + j;
            if ((address >= registerStartAddress)
                    && (address < registerStartAddress + registerNumbers * 2)) {
                byteArray.append(m_channelDatas[i][j]);
            }
        }
    }

    unsigned char tempNumber = 0;
    // 添加crc高位，低位
    byteArray.append(tempNumber).append(tempNumber);
    QVector<unsigned char> crcs = crcDetect(byteArray);
    byteArray[byteArray.size() - 2] = crcs[0];  // 高位
    byteArray[byteArray.size() - 1] = crcs[1];  // 低位
    qDebug() << "发送的字节: " <<  byteArray;
    qDebug() << "十六进制格式: " << byteArray.toHex(' ');

    return byteArray;
}
