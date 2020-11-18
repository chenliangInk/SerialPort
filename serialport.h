#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtCore/QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QVariantList>

#include "debugsendmessage.h"


class SerialPort : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool debug READ debug WRITE setDebug NOTIFY debugChanged)
    Q_PROPERTY(QVariantList portNames READ portNames NOTIFY portNamesChanged)
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QVariantList baudRates READ baudRates NOTIFY baudRatesChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QVariantList allDataBits READ allDataBits NOTIFY allDataBitsChanged)
    Q_PROPERTY(int dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(QVariantList allStopBits READ allStopBits NOTIFY allStopBitsChanged)
    Q_PROPERTY(float stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(QVariantList parities READ parities NOTIFY paritiesChanged)
    Q_PROPERTY(QString parity READ parity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(QVariantList flowControls READ flowControls NOTIFY flowControlsChanged)
    Q_PROPERTY(QString flowControl READ flowControl WRITE setFlowControl NOTIFY flowControlChanged)

private:
//    bool m_isOpen;  // 串口是否已经开启
    bool m_debug;
    QSerialPort* m_serialPort;
    QSerialPortInfo* m_serialPortInfo;
    QString m_portName;
    QVariantList m_baudRates;
    int m_baudRate;
    QVariantList m_allDataBits;
    int m_dataBits;
    QVariantList m_allStopBits;
    float m_stopBits;
    QVariantList m_parities;
    QString m_parity;
    QVariantList m_flowControls;
    QString m_flowControl;

public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort(void);
    bool debug(void);
    void setDebug(bool debug);
    QVariantList portNames(void);
    QString portName(void);
    void setPortName(const QString&);
    QVariantList baudRates(void);
    int baudRate(void);
    void setBaudRate(const int&);
    QVariantList allDataBits(void);
    int dataBits(void);
    void setDataBits(const int&);
    QVariantList allStopBits(void);
    float stopBits(void);
    void setStopBits(const float&);
    QVariantList parities(void);
    QString parity(void);
    void setParity(const QString&);
    QVariantList flowControls(void);
    QString flowControl(void);
    void setFlowControl(const QString&);
    void readData(void);
    void debugAutoSend(const QByteArray&);

    Q_INVOKABLE void sendMessage(const QString&);
    Q_INVOKABLE bool open(const QString&, const int, const int, const float,
                             const QString&, const QString&);
    Q_INVOKABLE void close(void);
signals:
    void debugChanged(void);
    void portNamesChanged(void);
    void portNameChanged(void);
    void baudRatesChanged(void);
    void baudRateChanged(void);
    void allDataBitsChanged(void);
    void dataBitsChanged(void);
    void allStopBitsChanged(void);
    void stopBitsChanged(void);
    void paritiesChanged(void);
    void parityChanged(void);
    void flowControlChanged(void);
    void flowControlsChanged(void);
    void readDataChanged(const QString&);
};


#endif // SERIALPORT_H
