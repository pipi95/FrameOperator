#ifndef DEVICEUTILITY_H
#define DEVICEUTILITY_H

#include "frameoperator_global.h"
#include <QFile>
#include <QIODevice>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QtSerialPort/QSerialPort>

class FRAMEOPERATORSHARED_EXPORT DeviceUtility {
public:
    static QIODevice* GetSerialPort(
        const QString& portName,
        qint32 baudRate = QSerialPort::Baud9600,
        QSerialPort::DataBits dataBits = QSerialPort::Data8,
        QSerialPort::Parity parity = QSerialPort::NoParity,
        QSerialPort::StopBits stopBits = QSerialPort::OneStop,
        QObject* parent = nullptr);
    static QIODevice* GetMultiCast(
        QHostAddress address,
        quint16 port,
        QObject* parent = nullptr); // 为了降低连接过程对UI的卡滞影响，可考虑使用MulticastGetter类进行多线程连接
    static QIODevice* GetTcpSocket(
        QHostAddress address,
        quint16 port,
        QObject* parent = nullptr); // 为了降低连接过程对UI的卡滞影响，可考虑使用TcpGetter类进行多线程连接
    static QIODevice* GetFile(const QString& filePath, QObject* parent = nullptr);
};

class FRAMEOPERATORSHARED_EXPORT MulticastGetter : public QObject {
    Q_OBJECT
public:
    MulticastGetter(QObject* parent = nullptr);
    ~MulticastGetter();
public slots:
    void doWork(QHostAddress address, quint16 port, QObject* invoker);
signals:
    void resultReady(QIODevice* udp, bool result);
};
class FRAMEOPERATORSHARED_EXPORT TcpGetter : public QObject {
    Q_OBJECT
public:
    TcpGetter(QObject* parent = nullptr);
    ~TcpGetter();
public slots:
    void doWork(QHostAddress address, quint16 port, QObject* invoker);
signals:
    void resultReady(QIODevice* tcp, bool result);
};
#endif // DEVICEUTILITY_H
