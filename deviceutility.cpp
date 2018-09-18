#include "deviceutility.h"
#include <QThread>
#include <QTimer>

QIODevice* DeviceUtility::GetSerialPort(
    const QString& portName,
    qint32 baudRate,
    QSerialPort::DataBits dataBits,
    QSerialPort::Parity parity,
    QSerialPort::StopBits stopBits,
    QObject* parent)
{
    QSerialPort* serialport = new QSerialPort(parent);
    serialport->setBaudRate(baudRate);
    serialport->setParity(parity);
    serialport->setStopBits(stopBits);
    serialport->setDataBits(dataBits);
    serialport->setPortName(portName);
    if (serialport->open(QSerialPort::ReadWrite)) {
        return serialport;
    }
    if (nullptr == parent) {
        delete serialport;
    }
    return nullptr;
}

QIODevice* DeviceUtility::GetMultiCast(
    QHostAddress address,
    quint16 port,
    QObject* parent)
{
    QUdpSocket* udp = new QUdpSocket(parent);
    if (udp->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint)) {
        if (udp->joinMulticastGroup(address)) {
            return udp;
        }
    }
    if (nullptr == parent) {
        delete udp;
    }
    return nullptr;
}

QIODevice* DeviceUtility::GetTcpSocket(
    QHostAddress address,
    quint16 port,
    QObject* parent)
{
    QTcpSocket* tcp = new QTcpSocket(parent);
    tcp->connectToHost(address, port, QTcpSocket::ReadWrite);
    if (tcp->isValid()) {
        return tcp;
    }
    if (nullptr == parent) {
        delete tcp;
    }
    return nullptr;
}

QIODevice* DeviceUtility::GetFile(const QString& filePath, QObject* parent)
{
    if (QFile::exists(filePath)) {
        return new QFile(filePath, parent);
    } else {
        return nullptr;
    }
}

MulticastGetter::MulticastGetter(QObject* parent)
    : QObject(parent)
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
}

MulticastGetter::~MulticastGetter()
{
}
void MulticastGetter::doWork(QHostAddress address, quint16 port, QObject* invoker)
{
    bool result = true;
    QIODevice* udp = nullptr;
    try {
        udp = DeviceUtility::GetMultiCast(address, port);
    } catch (...) {
        result = false;
    }
    if (result && udp != nullptr) { // udp 地址不合法时返回的udp为空

        udp->moveToThread(invoker->thread());
    }
    emit resultReady(udp, result);
}

TcpGetter::TcpGetter(QObject* parent)
    : QObject(parent)
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
}

TcpGetter::~TcpGetter()
{
}
void TcpGetter::doWork(QHostAddress address, quint16 port, QObject* invoker)
{
    bool result = true;
    QIODevice* tcp = nullptr;
    try {
        tcp = DeviceUtility::GetTcpSocket(address, port);
    } catch (...) {
        result = false;
    }
    if (result && tcp != nullptr) {
        tcp->moveToThread(invoker->thread());
    }
    emit resultReady(tcp, result);
}
